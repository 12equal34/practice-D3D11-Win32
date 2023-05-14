#pragma once
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timer.h"

class App
{
public:
    App() noexcept;
    ~App();
    App(const App&)                  = delete;
    const App& operator=(const App&) = delete;

    int Run();

private:
    Window   mainWindow;
    Keyboard keyboard;
    Mouse    mouse;
    Timer    mainTimer;
};
