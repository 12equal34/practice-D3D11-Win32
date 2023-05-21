#include <Windows.h>
#include <sstream>
#include "App.h"

App::App()
    : m_mainWindow(1000, 600, L"Window Sample"),
      m_keyboard(),
      m_mouse(),
      m_mainTimer(),
      m_renderer()
{
    m_mainWindow.SetKeyboard(&m_keyboard);
    m_mainWindow.SetMouse(&m_mouse);
    m_mainWindow.SetTimer(&m_mainTimer);
    m_mainWindow.SetRenderer(&m_renderer);
}

App::~App() { }

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

            // test code
            std::optional<Hardware::Mouse::Event> event;
            while ((event = m_mouse.Read()).has_value()) {
                static int i         = 0;
                using MouseEventType = Hardware::Mouse::Event::Type;

                switch (event.value().GetType()) {
                case MouseEventType::WheelUp:
                {
                    ++i;
                    std::wostringstream wo;
                    wo << L"wheel : " << i;
                    m_mainWindow.SetTitle(wo.str());
                    break;
                }
                case MouseEventType::WheelDown:
                {
                    --i;
                    std::wostringstream wo;
                    wo << L"wheel : " << i;
                    m_mainWindow.SetTitle(wo.str());
                    break;
                }
                }
            }

        } else {
            // Update the scene.

            // Render the frame.

            // Present the frame.
        }
    }

    if (bGotMsg == -1) {
        throw Hardware::Window::Exception(__LINE__, __FILE__, GetLastError());
    }
    return static_cast<int>(msg.wParam);
}
