#pragma once
#include <Windows.h>

template <class DerivedType> class BaseWindow
{
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                       LPARAM lParam)
    {
        DerivedType* pThis = nullptr;

        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = reinterpret_cast<DerivedType*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA,
                             reinterpret_cast<LONG_PTR>(pThis));

            pThis->m_hwnd = hwnd;
        } else {
            pThis = reinterpret_cast<DerivedType*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        return pThis ? pThis->HandleMessage(uMsg, wParam, lParam)
                     : DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    BaseWindow() : m_hwnd(NULL), m_rc() { }

    BOOL Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0,
                int x = CW_USEDEFAULT, int y = CW_USEDEFAULT)
    {
        // register the windows class
        WNDCLASS wc      = {0};
        wc.style         = CS_DBLCLKS;
        wc.lpfnWndProc   = DerivedType::WindowProc;
        wc.hInstance     = GetModuleHandle(NULL);
        wc.hIcon         = dynamic_cast<DerivedType*>(this)->GetHIcon();
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszClassName = ClassName();

        if (!RegisterClass(&wc)) {
            DWORD dwError = GetLastError();
            if (dwError != ERROR_CLASS_ALREADY_EXISTS) return FALSE;
        }

        SetRect(&m_rc, 0, 0, nDefaultWidth, nDefaultHeight);
        AdjustWindowRect(&m_rc, dwStyle, false);

        m_hwnd =
            CreateWindowEx(dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
                           (m_rc.right - m_rc.left), (m_rc.bottom - m_rc.top),
                           NULL, NULL, GetModuleHandle(NULL), this);

        return m_hwnd ? TRUE : FALSE;
    }

    HWND GetWindowHandle() const { return m_hwnd; }

protected:
    virtual PCWSTR  ClassName() const                                      = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    int nDefaultWidth  = 640;
    int nDefaultHeight = 480;

    RECT m_rc;
    HWND m_hwnd;
};

class Window : public BaseWindow<Window>
{
public:
    PCWSTR ClassName() const { return sWindowClassName; }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    HICON GetHIcon() const { return m_hIcon; }

    Window();
    ~Window();

protected:
    static constexpr const wchar_t* sWindowClassName = L"Sample Window Class";

    HICON m_hIcon;
};
