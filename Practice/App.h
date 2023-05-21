#pragma once
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timer.h"
#include "Renderer.h"

class App
{
public:
    App();
    ~App();
    App(const App&)                  = delete;
    const App& operator=(const App&) = delete;

    int Run();
private:
    void RunFrame(float dt);

private:
    Hardware::Window   m_mainWindow;
    Hardware::Keyboard m_keyboard;
    Hardware::Mouse    m_mouse;
    Timer    m_mainTimer;
    Renderer m_renderer;
};
