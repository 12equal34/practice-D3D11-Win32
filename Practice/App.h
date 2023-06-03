#pragma once
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timer.h"
#include "Camera.h"

class App
{
public:
    App();
    ~App();
    App(const App&)            = delete;
    App& operator=(const App&) = delete;

    int Run();
private:
    void HandleInput(float dt);
    void RunFrame(float dt);

private:
    Hardware::Window   m_window;
    Hardware::Keyboard m_keyboard;
    Hardware::Mouse    m_mouse;
    Timer              m_mainTimer;
    Camera             m_camera;
};
