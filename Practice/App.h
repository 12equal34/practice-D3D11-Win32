#pragma once
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timer.h"
#include "Camera.h"
#include "Map.h"

class App
{
public:
    enum class AppState { Run, Initialize, Reset };
public:
    App();
    ~App();
    App(const App&)            = delete;
    App& operator=(const App&) = delete;

    int Run();
private:
    void HandleInput(float dt);
    void RunFrame(float dt);

    void DebugHelpWindowTitle(float dt);

    void InitializeApp();
    void ResetApp();

private:
    Hardware::Window      m_window;
    Hardware::Keyboard    m_keyboard;
    Hardware::Mouse       m_mouse;
    Timer                 m_mainTimer;
    World::Object::Camera m_camera;
    World::Map            m_map;

    AppState m_appState;
};
