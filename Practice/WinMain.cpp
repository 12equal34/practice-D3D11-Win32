#include <Windows.h>
#include <string>
#include "Window.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    Window win1, win2;

    std::wstring windowName = L"win1";
    if (!win1.Create(windowName.c_str(), WS_OVERLAPPEDWINDOW)) {
        return 0;
    }

    windowName = L"win2";
    if (!win2.Create(windowName.c_str(), WS_OVERLAPPEDWINDOW)) {
        return 0;
    }

    ShowWindow(win1.GetWindowHandle(), nCmdShow);
    ShowWindow(win2.GetWindowHandle(), nCmdShow);

    // message pump
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}