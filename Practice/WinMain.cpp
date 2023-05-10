#include <Windows.h>
#include <string>

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void             OnSize(HWND hwnd, UINT flag, int width, int height);

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
        // calls the window procedure of the window that is the target of the
        // message.
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Window procedure
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // hwnd is a handle to the window.
    // uMsg is the message code
    // wParam and lParam contain additional data that pertains to the message.
    // The meaning of each depends on the message code (uMsg). For each message,
    // you will need to look up the message code on MSDN and cast the parameters
    // to the correct data type.
    //
    // Return value, LRESULT, is an integer value that
    // its program returns to Windows.

    switch (uMsg) {
    case WM_DESTROY: PostQuitMessage(0); break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC         hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            EndPaint(hwnd, &ps);
        }
        break;

    case WM_CLOSE:
        if (MessageBox(hwnd, L"Really quit?", L"*QUIT", MB_OKCANCEL) == IDOK) {
            DestroyWindow(hwnd);
        }
        break;

    case WM_SIZE:
        {
            int width = LOWORD(lParam); // Macro to get the 16-bit
            // low-order word from lParam.
            int height = HIWORD(lParam); // Macro to get the 16-bit
            // hight-order word from lParam.

            OnSize(hwnd, (UINT)wParam, width, height);
        }
        break;
    }

    // perform the default actions for the other messages.
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void OnSize(HWND hwnd, UINT flag, int width, int height)
{
    // Handle resizing
}