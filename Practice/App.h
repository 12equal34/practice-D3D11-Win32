#pragma once
#include "Window.h"

class App
{
public:
    App() noexcept;
    ~App();
    App(const App&)                  = delete;
    const App& operator=(const App&) = delete;

    int Run();

private:
    Window mainWindow;
};
