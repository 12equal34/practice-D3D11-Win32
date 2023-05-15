#include "pch.h"
#include <sstream>
#include "Window.h"
#include "resource.h"

#ifdef _DEBUG
#include "WindowsMessageMap.h"
#endif

//-----------------------------------------------------------------------------
// Singleton Window Class
//-----------------------------------------------------------------------------
Window::WindowClass Window::WindowClass::WndClass;

const wchar_t* Window::WindowClass::GetName() noexcept { return WndClassName; }

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return WndClass.m_hInst;
}

Window::WindowClass::WindowClass() noexcept
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
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(GetName(), GetInstance());
}

//-----------------------------------------------------------------------------
// window for App
//-----------------------------------------------------------------------------

Window::Window(int width, int height, std::wstring_view titleName) noexcept
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
    AdjustWindowRect(&wr, dwStyle, false);
    width  = wr.right - wr.left;
    height = wr.bottom - wr.top;

    // create window & get hwnd
    m_hwnd = CreateWindow(WindowClass::GetName(), titleName.data(), dwStyle,
                          CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr,
                          nullptr, WindowClass::GetInstance(), this);

    // show window
    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
}

Window::~Window() { DestroyWindow(m_hwnd); }

void Window::SetKeyboard(Keyboard* pKeyboard) noexcept
{
    m_pKeyboard = pKeyboard;
}

void Window::SetMouse(Mouse* pMouse) noexcept { m_pMouse = pMouse; }

void Window::SetTimer(Timer* pTimer) noexcept { m_pTimer = pTimer; }

void Window::SetRenderer(Renderer* pRenderer) noexcept
{
    m_pRenderer = pRenderer;
}

//-----------------------------------------------------------------------------
// Message Handlers
//-----------------------------------------------------------------------------

LRESULT Window::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam,
                               LPARAM lParam) noexcept
{
#ifdef _DEBUG
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
#ifdef _DEBUG
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
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_KILLFOCUS:
        if (m_pKeyboard) m_pKeyboard->ClearState();
        break;
    /********** keyboard message **********/
    case WM_KEYDOWN:
        if (m_pKeyboard)
            m_pKeyboard->OnKeyPressed(static_cast<unsigned char>(wParam));
        break;
    case WM_KEYUP:
        if (m_pKeyboard)
            m_pKeyboard->OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    case WM_CHAR:
        if (m_pKeyboard)
            m_pKeyboard->OnChar(static_cast<unsigned char>(wParam));
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Window Debugging
//-----------------------------------------------------------------------------

#ifdef _DEBUG
void Window::MsgToOutputDebug(UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    static WindowsMessageMap msgMap;
    OutputDebugStringA(msgMap(msg, wParam, lParam).c_str());
}
#endif

//-----------------------------------------------------------------------------
// Window Exceptions
//-----------------------------------------------------------------------------

Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
    : BaseException(line, file),
      m_hr(hr)
{ }

const char* Window::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << '\n'
        << GetOriginString() << '\n'
        << "[ErrorCode] " << GetErrorCode() << '\n'
        << "[Description] " << GetErrorString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
    return "Window::Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);
    if (nMsgLen == 0) {
        return "Unidentified error code";
    }
    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept { return m_hr; }

std::string Window::Exception::GetErrorString() const noexcept
{
    return TranslateErrorCode(GetErrorCode());
}
