#include "pch.h"
#include "Window.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    Window mainWindow(1000, 600, L"Window Sample");
    
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
    
    return msg.wParam;
}