#include "pch.h"
#include "Window.h"

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
    wc.hIcon         = nullptr;
    wc.hCursor       = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName  = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm       = nullptr;
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(GetName(), GetInstance());
}

//-----------------------------------------------------------------------------
// App Window
//-----------------------------------------------------------------------------

Window::Window(int width, int height, std::wstring_view titleName) noexcept
    : m_width(width),
      m_height(height),
      m_titleName(titleName),
      m_hwnd(nullptr)
{
    DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

    // calculate window size based on desired client region size
    RECT wr;
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

//-----------------------------------------------------------------------------
// Message Handlers
//-----------------------------------------------------------------------------

LRESULT Window::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam,
                               LPARAM lParam) noexcept
{
    // use create parameter passed in from CreateWindow() to store window class
    // pointer at WinAPI side
    if (msg == WM_NCCREATE) {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate =
            reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window class
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is
        // finished
        SetWindowLongPtr(hwnd, GWLP_WNDPROC,
                         reinterpret_cast<LONG_PTR>(&Window::HandleMsgAdapter));
        // forward message to window class handler
        return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default
    // handler
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgAdapter(HWND hwnd, UINT msg, WPARAM wParam,
                                 LPARAM lParam) noexcept
{
    // retrieve ptr to window class
    Window* const pWnd =
        reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    // forward message to window class handler
    return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam,
                          LPARAM lParam) noexcept
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
