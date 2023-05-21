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
        } else {
            float dt = static_cast<float>(m_mainTimer.Mark());
            RunFrame(dt);
        }
    }

    if (bGotMsg == -1) {
        throw Hardware::Window::Exception(__LINE__, __FILE__, GetLastError());
    }
    return static_cast<int>(msg.wParam);
}

void App::RunFrame(float dt)
{
    // test code
    static float           time     = 0.0f;
    static int             i        = 0;
    static constexpr float interval = 0.1f;
    time += dt;
    if (time > interval * i) {
        std::wostringstream wo;
        wo << L"Timer : " << time << L"s";
        m_mainWindow.SetTitle(wo.str());
        ++i;
    }

    // Update the scene.

    // Render the frame.

    // Present the frame.
}
