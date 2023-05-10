#include <Windows.h>
#include <string>
#include <memory>
#include <sstream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// A window procedure is just a function that gets invoked for every message,
// so it is inherently stateless. Therefore, you need a way to track the state
// of your application from one function call to the next.
// (The simplest approach is to put everything in global variables.)

// Define a structure to hold some state information.
struct StateInfo {
    int          someInt;
    std::wstring someWstr;
};

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

    // Define a structure to hold some state information.
    auto pState = std::make_unique<StateInfo>(7, L"Lucky Seven");

    // create the window. (Look at the last parameter.)
    HWND hWnd = CreateWindowEx(0, windowClassName.c_str(), windowName.c_str(),
                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               nullptr, nullptr, hInstance, pState.get());
    // sends two messages to its window procedure:
    // WM_NCCREATE (none-client), WM_CREATE
    // are sent before the window becomes visible.
    // It can extract the last parameter of CreateWindowEx, pState, from the
    // message data.

    // When you the WM_NCCREATE and WM_CREATE messages, the lParam is a pointer
    // to a CREATESTRUCT structure contains the pointer that you passed into
    // CreateWindowEx: lpCreateParams

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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    StateInfo* pState;
    if (uMsg == WM_CREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pState = reinterpret_cast<StateInfo*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pState);

        std::wostringstream wss;
        wss << pState->someInt << L" : " << pState->someWstr
            << L" <--- This datas are the members of a StateInfo structure.";
        SetWindowText(hwnd, wss.str().c_str());
    } else {
        LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        pState       = reinterpret_cast<StateInfo*>(ptr);
    }

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
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}