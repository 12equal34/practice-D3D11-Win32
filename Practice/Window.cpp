#include "Window.h"

LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);

    case WM_CLOSE:
        if (MessageBox(m_hwnd, L"Really quit?", L"*QUIT", MB_OKCANCEL) ==
            IDOK) {
            DestroyWindow(m_hwnd);
        }
        return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }

    return TRUE;
}

Window::Window() : m_hIcon(NULL)
{
    WCHAR szExePath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szExePath, MAX_PATH);

    // If the icon is nullptr, then use the first one found in the exe
    if (m_hIcon == NULL) {
        m_hIcon = ExtractIcon(GetModuleHandle(NULL), szExePath, 0);
        if (m_hIcon == (HICON)1) {
            m_hIcon = NULL;
        }
    }
}

Window::~Window() { DestroyIcon(m_hIcon); }
