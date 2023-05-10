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
    case WM_DESTROY:
        {
            PostQuitMessage(0);
            // puts a WM_QUIT message on the message queue, causing the message
            // loop to end.
        }
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC         hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.

            // The surrounding frame is automatically painted by the operating
            // sys.
            // The portion of the window that must be painted is called the
            // update region.
            // The current update region is given in the rcPaint of ps.

            // In the painting code, there is two basic options:
            // 1. Paint the entire client area regardless of the size of the
            // update region.
            // 2. Optimize by painting just the portion of the window inside the
            // update region. (If complicated painting, more efficient)

            // Just fill the entire client area with a solid color.
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            // The FillRect function is part of GDI (the Graphics Device
            // Interface). In the versions above Windows Vista, Direct2D is
            // used. (GDI is still fully supported.)

            // Clear the update region, which signals to Windows that the window
            // has completed painting itself.
            EndPaint(hwnd, &ps);
        }
        return 0;

    case WM_CLOSE:
        if (MessageBox(hwnd, L"Really quit?", L"*QUIT", MB_OKCANCEL) == IDOK) {
            DestroyWindow(hwnd);
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
