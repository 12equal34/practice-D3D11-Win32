#include <sstream>
#include "Window.h"
#include "resource.h"
#include "WinExceptionHelper.h"

// if you want to map windows messages to the output
// #define DEBUG_WINMSG

#if (!defined NDEBUG && defined DEBUG_WINMSG)
#include "WindowsMessageMap.h"
#endif

using namespace Hardware;
using namespace Hardware::Win;
//-----------------------------------------------------------------------------
// Singleton Window Class
//-----------------------------------------------------------------------------
Window::WindowClass Window::WindowClass::WndClass;
const wchar_t* Window::WindowClass::GetName() noexcept { return WndClassName; }
HINSTANCE      Window::WindowClass::GetInstance() noexcept
{
    return WndClass.m_hInst;
}
Window::WindowClass::WindowClass()
    : m_hInst(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc    = {0};
    wc.cbSize        = sizeof(wc);
    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = HandleMsgSetup;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetInstance();
    wc.hIcon         = static_cast<HICON>(LoadImage(
        GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    wc.hCursor       = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName  = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm       = static_cast<HICON>(LoadImage(
        GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
    ThrowIfNull(RegisterClassEx(&wc));
}
Window::WindowClass::~WindowClass()
{
    UnregisterClass(GetName(), GetInstance());
}
//-----------------------------------------------------------------------------
// window for App
//-----------------------------------------------------------------------------
Window::Window(int width, int height, std::wstring_view titleName)
    : m_width(width),
      m_height(height),
      m_titleName(titleName),
      m_hwnd(nullptr),
      m_pKeyboard(nullptr),
      m_pMouse(nullptr),
      m_pTimer(nullptr),
      m_pRenderer(nullptr)
{
    DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

    // calculate window size based on desired client region size
    RECT wr   = {0};
    wr.left   = 100;
    wr.top    = 100;
    wr.right  = wr.left + m_width;
    wr.bottom = wr.top + m_height;
    ThrowIfNull(AdjustWindowRect(&wr, dwStyle, false));

    width  = wr.right - wr.left;
    height = wr.bottom - wr.top;

    // create window & get hwnd
    ThrowIfNull(m_hwnd = CreateWindow(WindowClass::GetName(), titleName.data(),
                                      dwStyle, CW_USEDEFAULT, CW_USEDEFAULT,
                                      width, height, nullptr, nullptr,
                                      WindowClass::GetInstance(), this));

    // register mouse raw input device
    RAWINPUTDEVICE riDevice = {};
    riDevice.usUsagePage    = 0x01; // generic desktop controls
    riDevice.usUsage        = 0x02; // mouse usage
    riDevice.dwFlags        = 0;
    riDevice.hwndTarget     = m_hwnd;
    ThrowIfNull(RegisterRawInputDevices(&riDevice, 1u, sizeof(riDevice)));

    // show window
    ShowWindow(m_hwnd, SW_SHOWDEFAULT); // If the window was previously hidden,
                                        // the return value is zero.

    // create renderer
    m_pRenderer = std::make_unique<Hardware::DX::Renderer>(m_hwnd);
}
Window::~Window() { DestroyWindow(m_hwnd); }
void Window::SetKeyboard(Keyboard* pKeyboard) noexcept
{
    m_pKeyboard = pKeyboard;
}
void Window::SetMouse(Mouse* pMouse) noexcept { m_pMouse = pMouse; }
void Window::SetTimer(Timer* pTimer) noexcept { m_pTimer = pTimer; }
void Window::SetTitle(std::wstring_view titleName)
{
    ThrowIfNull(SetWindowTextW(m_hwnd, titleName.data()));
}
HWND          Window::GetHwnd() const noexcept { return m_hwnd; }
DX::Renderer& Window::GetRenderer() const noexcept { return *m_pRenderer; }
int           Window::GetWidth() const noexcept { return m_width; }
int           Window::GetHeight() const noexcept { return m_height; }
float Hardware::Window::GetAspectRatio() const noexcept
{
    return static_cast<float>(m_height) / m_width;
}
//-----------------------------------------------------------------------------
// Message Handlers
//-----------------------------------------------------------------------------
LRESULT Window::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam,
                               LPARAM lParam) noexcept
{
#if (!defined NDEBUG && defined DEBUG_WINMSG)
    MsgToOutputDebug(msg, wParam, lParam);
#endif
    // use create parameter passed in from CreateWindow() to store window
    // instance pointer at WinAPI side
    if (msg == WM_NCCREATE) {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate =
            reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window instance
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is
        // finished
        SetWindowLongPtr(hwnd, GWLP_WNDPROC,
                         reinterpret_cast<LONG_PTR>(&Window::HandleMsgAdapter));
        // forward message to window instance handler
        return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default
    // handler
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
LRESULT Window::HandleMsgAdapter(HWND hwnd, UINT msg, WPARAM wParam,
                                 LPARAM lParam) noexcept
{
#if (!defined NDEBUG && defined DEBUG_WINMSG)
    MsgToOutputDebug(msg, wParam, lParam);
#endif

    // retrieve ptr to window instance
    Window* const pWnd =
        reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    // forward message to window instance handler
    return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
}
// main message handler
LRESULT Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam,
                          LPARAM lParam) noexcept
{
    switch (msg) {
#pragma region Window Notification Handler
    case WM_ACTIVATEAPP:
    case WM_CANCELMODE:
    case WM_CHILDACTIVATE:
        break;
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_COMPACTING:
    case WM_CREATE:
    case WM_DESTROY:
    case WM_DPICHANGED:
    case WM_ENABLE:
    case WM_ENTERSIZEMOVE:
    case WM_EXITSIZEMOVE:
    case WM_GETICON:
    case WM_GETMINMAXINFO:
    case WM_INPUTLANGCHANGE:
    case WM_INPUTLANGCHANGEREQUEST:
    case WM_MOVE:
    case WM_MOVING:
    case WM_NCACTIVATE:
    case WM_NCCALCSIZE:
    case WM_NCCREATE:
    case WM_NCDESTROY:
    case WM_NULL:
    case WM_QUERYDRAGICON:
    case WM_QUERYOPEN:
    case WM_QUIT:
    case WM_SHOWWINDOW:
    case WM_SIZE:
    case WM_SIZING:
    case WM_STYLECHANGED:
    case WM_STYLECHANGING:
    case WM_THEMECHANGED:
    case WM_USERCHANGED:
    case WM_WINDOWPOSCHANGED:
    case WM_WINDOWPOSCHANGING:
#pragma endregion Window Notification Handler
#pragma region Keyboard Notification  Handler
    case WM_ACTIVATE:
    case WM_APPCOMMAND:
        break;
    case WM_CHAR:
    {
        if (m_pKeyboard)
            m_pKeyboard->OnChar(static_cast<unsigned char>(wParam));
        break;
    }
    case WM_DEADCHAR:
    case WM_HOTKEY:
        break;
    case WM_KEYDOWN:
    {
        if (m_pKeyboard)
            m_pKeyboard->OnKeyPressed(static_cast<unsigned char>(wParam));
        break;
    }
    case WM_KEYUP:
    {
        if (m_pKeyboard)
            m_pKeyboard->OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    }
    case WM_KILLFOCUS:
    {
        if (m_pKeyboard) m_pKeyboard->ClearState();
        break;
    }
    case WM_SETFOCUS:
    case WM_SYSDEADCHAR:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_UNICHAR:
#pragma endregion Keyboard Notification Handler
#pragma region Mouse Notification       Handler
    case WM_CAPTURECHANGED:
    case WM_LBUTTONDBLCLK:
        break;
    case WM_LBUTTONDOWN:
    {
        if (m_pMouse) m_pMouse->OnLeftButtonDown();
        // bring window to foreground on lclick client region
        SetForegroundWindow(hwnd);
        break;
    }
    case WM_LBUTTONUP:
    {
        if (m_pMouse) m_pMouse->OnLeftButtonUp();
        break;
    }
    case WM_MBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEACTIVATE:
    case WM_MOUSEHOVER:
        break;
    case WM_MOUSEHWHEEL:
    {
        if (!m_pMouse) break;

        const auto zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        m_pMouse->OnMouseWheelH(zDelta);
        break;
    }
    case WM_MOUSELEAVE:
        break;
    case WM_MOUSEMOVE:
    {
        if (!m_pMouse) break;

        const POINTS pt = MAKEPOINTS(lParam);
        int          x  = pt.x;
        int          y  = pt.y;
        if (0 <= x && x < m_width && 0 <= y && y < m_height) {
            m_pMouse->OnMouseMove(x, y);
            if (!m_pMouse->IsInWindow()) {
                SetCapture(hwnd);
                m_pMouse->OnMouseEnter();
            }
        } else {
            if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
                m_pMouse->OnMouseMove(x, y);
            } else {
                ReleaseCapture();
                m_pMouse->OnMouseLeave();
            }
        }
        break;
    }
    case WM_MOUSEWHEEL:
    {
        if (!m_pMouse) break;

        const auto zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        m_pMouse->OnMouseWheel(zDelta);
        break;
    }
    case WM_NCHITTEST:
    case WM_NCLBUTTONDBLCLK:
    case WM_NCLBUTTONDOWN:
    case WM_NCLBUTTONUP:
    case WM_NCMBUTTONDBLCLK:
    case WM_NCMBUTTONDOWN:
    case WM_NCMBUTTONUP:
    case WM_NCMOUSEHOVER:
    case WM_NCMOUSELEAVE:
    case WM_NCMOUSEMOVE:
    case WM_NCRBUTTONDBLCLK:
    case WM_NCRBUTTONDOWN:
    case WM_NCRBUTTONUP:
    case WM_NCXBUTTONDBLCLK:
    case WM_NCXBUTTONDOWN:
    case WM_NCXBUTTONUP:
    case WM_RBUTTONDBLCLK:
        break;
    case WM_RBUTTONDOWN:
    {
        if (m_pMouse) m_pMouse->OnRightButtonDown();
        break;
    }
    case WM_RBUTTONUP:
    {
        if (m_pMouse) m_pMouse->OnRightButtonUp();
        break;
    }
    case WM_XBUTTONDBLCLK:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
        break;
#pragma endregion Mouse Notification Handler

#pragma region Raw Input Notification Handler
    case WM_INPUT:
    {
        if (!m_pMouse) break;

        // first get the size of the raw input data
        UINT size = 0;
        if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
                            nullptr, &size, sizeof(RAWINPUTHEADER)) == -1) {
            // bail message processing if there is an error
            break;
        }

        // second read the raw input data
        std::vector<BYTE> rawBuffer;
        rawBuffer.reserve(size);
        if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
                            rawBuffer.data(), &size,
                            sizeof(RAWINPUTHEADER)) != size) {
            // bail message processing if there is an error
            break;
        }

        // process the raw input data
        auto& rawInput = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
        if (rawInput.header.dwType == RIM_TYPEMOUSE) {
            const auto& mouse = rawInput.data.mouse;
            const auto  dx    = mouse.lLastX;
            const auto  dy    = mouse.lLastY;
            if (dx != 0 || dy != 0) {
                m_pMouse->OnMouseMoveDelta(dx, dy);
            }
        }
        break;
    }
    case WM_INPUT_DEVICE_CHANGE:
        break;
#pragma endregion Raw Input Notification Handler
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
//-----------------------------------------------------------------------------
// Window Debugging
//-----------------------------------------------------------------------------
#if (!defined NDEBUG && defined DEBUG_WINMSG)
void Window::MsgToOutputDebug(UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    static WindowsMessageMap msgMap;
    OutputDebugStringA(msgMap(msg, wParam, lParam).c_str());
}
#endif
//-----------------------------------------------------------------------------
// Window Exceptions
//-----------------------------------------------------------------------------
std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
    char* pMsgBuf = nullptr;
    // windows will allocate memory for err string and make our pointer point to
    // it
    const DWORD nMsgLen = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);
    // 0 string length returned indicates a failure
    if (nMsgLen == 0) {
        return "Unidentified error code";
    }
    // copy error string from windows-allocated buffer to std::string
    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}
Window::HrException::HrException(int line, const char* file,
                                 HRESULT hr) noexcept
    : Exception(line, file),
      m_hr(hr)
{ }
const char* Window::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << '\n'
        << "[ErrorCode] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")\n"
        << "[Description] " << GetErrorDescription() << '\n'
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}
const char* Window::HrException::GetType() const noexcept
{
    return "Window::HrException";
}
HRESULT     Window::HrException::GetErrorCode() const noexcept { return m_hr; }
std::string Window::HrException::GetErrorDescription() const noexcept
{
    return Exception::TranslateErrorCode(GetErrorCode());
}
const char* Window::NoRendererException::GetType() const noexcept
{
    return "Window::NoRendererException";
}
