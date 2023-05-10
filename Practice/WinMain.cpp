#include <Windows.h>
#include <string>

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------------------
// Entry point
//-----------------------------------------------------------------------------

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    std::wstring       windowName      = L"Not Defined Window Name";
    const std::wstring windowClassName = L"Sample Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = windowClassName.c_str();

    RegisterClass(&wc);

    // Create the window.
    HWND hWnd = CreateWindowEx(0, windowClassName.c_str(), windowName.c_str(),
                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               nullptr, nullptr, hInstance, nullptr);

    if (hWnd == nullptr) {
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);

    // message pump
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Window procedure
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_DESTROY: PostQuitMessage(0); return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC         hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            EndPaint(hwnd, &ps);
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
