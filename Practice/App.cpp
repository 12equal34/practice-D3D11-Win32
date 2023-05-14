#include <Windows.h>
#include "App.h"

App::App() noexcept
	: mainWindow(1000, 600, L"Window Sample")
{
}

App::~App()
{
}

int App::Run()
{
    MSG msg {};
    msg.message = WM_NULL;
    BOOL bGotMsg;

    while (WM_QUIT != msg.message) {
        bGotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);

        if (bGotMsg) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // Update the scene.

            // Render the frame.

            // Present the frame.
        }
    }

    if (bGotMsg == -1) {
        throw Window::Exception(__LINE__, __FILE__, GetLastError());
    }
    return static_cast<int>(msg.wParam);
}
