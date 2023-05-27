#include <sstream>
#include "App.h"

App::App()
    : m_window(1000, 600, L"Window Sample"),
      m_keyboard(),
      m_mouse(),
      m_mainTimer()
{
    m_window.SetKeyboard(&m_keyboard);
    m_window.SetMouse(&m_mouse);
    m_window.SetTimer(&m_mainTimer);
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
        throw Hardware::Window::HrException(__LINE__, __FILE__, GetLastError());
    }
    return static_cast<int>(msg.wParam);
}

void App::RunFrame(float dt)
{
    // write times at the window title
    static float           time     = 0.0f;
    static int             i        = 0;
    static constexpr float interval = 0.1f;
    static int             frames   = 0;
    ++frames;
    time += dt;
    if (time > interval * i) {
        float fps = frames / interval;
        frames    = 0;
        ++i;

        std::wostringstream wo;
        wo << L"Timer : " << time << L"s, fps: " << fps;
        m_window.SetTitle(wo.str());
    }

    // test code
    const float color = sinf(time) * 0.5f + 0.5f;

    auto& renderer = m_window.Renderer();

    renderer.ClearBuffer(color, color, 1.0f);
    renderer.DrawTest(time);
    renderer.EndFrame();
}
