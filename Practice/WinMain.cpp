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

    
    MSG  msg {};
    msg.message = WM_NULL;
    BOOL bGotMsg;

    while (WM_QUIT != msg.message)
    {
        bGotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);

        if (bGotMsg) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // Update the scene.

            // Render the frame.

            // Present the frame.
        }
    }

    if (bGotMsg == -1) {
        return -1;
    }
    else {
        return msg.wParam;
    }
}