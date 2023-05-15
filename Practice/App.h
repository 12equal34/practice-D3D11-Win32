#pragma once
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timer.h"
#include "Renderer.h"

class App
{
public:
    App() noexcept;
    ~App();
    App(const App&)                  = delete;
    const App& operator=(const App&) = delete;

    int Run();

private:
    Window   m_mainWindow;
    Keyboard m_keyboard;
    Mouse    m_mouse;
    Timer    m_mainTimer;
    Renderer m_renderer;
};
