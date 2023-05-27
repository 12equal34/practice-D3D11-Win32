#include <sstream>
#include "App.h"

App::App()
    : m_window(1000, 600, L"Window Sample"),
      m_keyboard(&m_window),
      m_mouse(&m_window),
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
    double                  totalTime = m_mainTimer.TimeSinceStart();
    static float            time      = 0;
    static int              i         = 0;
    static constexpr double interval  = 0.1;
    static int              frames    = 0;
    ++frames;
    time += dt;
    if (totalTime > interval * i) {
        double fps = frames / interval;
        frames     = 0;
        ++i;

        std::wostringstream wo;
        wo << L"Timer : " << std::left << std::setw(16) << totalTime << L"fps: "
           << std::left << std::setw(16) << fps;
        wo << L"Mouse : (" << m_mouse.GetNormalizedX() << ','
           << m_mouse.GetNormalizedY() << ')';
        m_window.SetTitle(wo.str());
    }

    // test code
    const float color = sinf(time) * 0.5f + 0.5f;

    auto& renderer = m_window.Renderer();

    renderer.ClearBuffer(color, color, 1.0f);
    renderer.DrawTest(time, m_mouse.GetNormalizedX(), m_mouse.GetNormalizedY());
    renderer.EndFrame();
}