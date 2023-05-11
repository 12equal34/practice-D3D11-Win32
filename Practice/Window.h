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

    BaseWindow() : m_hwnd(nullptr) { }

    BOOL Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0,
                int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
                int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
                HWND hWndParent = 0, HMENU hMenu = 0)
    {
        WNDCLASS wc = {0};

        wc.lpfnWndProc   = DerivedType::WindowProc;
        wc.hInstance     = GetModuleHandle(nullptr);
        wc.lpszClassName = ClassName();

        RegisterClass(&wc);

        m_hwnd = CreateWindowEx(dwExStyle, ClassName(), lpWindowName, dwStyle,
                                x, y, nWidth, nHeight, hWndParent, hMenu,
                                GetModuleHandle(nullptr), this);

        return m_hwnd ? TRUE : FALSE;
    }

    HWND GetWindowHandle() const { return m_hwnd; }

protected:
    virtual PCWSTR  ClassName() const                                      = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
};

class Window : public BaseWindow<Window>
{
public:
    PCWSTR ClassName() const { return sWindowClassName; }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    static constexpr const wchar_t* sWindowClassName = L"Sample Window Class";
};
