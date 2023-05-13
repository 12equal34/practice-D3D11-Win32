#include <Windows.h>
#include "Window.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    Window mainWindow;
    if (!mainWindow.Create(L"Window Sample", WS_OVERLAPPEDWINDOW)) {
        return 0;
    }

    ShowWindow(mainWindow.GetWindowHandle(), nCmdShow);

    // message pump
    MSG  msg = {};
    BOOL msgResult;
    while ((msgResult = GetMessage(&msg, NULL, 0, 0)) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msgResult == -1) {
        return -1;
    }
    else {
        return msg.wParam;
    }
}