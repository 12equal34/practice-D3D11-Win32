#pragma once

#include "Windows.h"
#include <memory>
#include <exception>
#include <iostream>

namespace Windows
{
class Timer;
class Window;
class WindowClass;

class App
{
public:
	App();
	~App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;
public:
	enum class AppState { Init, Run, Reset };
public:
	int Entry();
	WPARAM MessagePump();
	void Run(double deltaSeconds);
	void Init();
	void Reset();
private:
	std::unique_ptr<WindowClass> _mainWindowClass;
private:
	AppState _appState;
	MSG _windowsMsg;
	std::unique_ptr<Timer> _timer;
	std::unique_ptr<Window> _mainWindow;
};
}

#define WindowsAppEntryPoint \
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)\
{\
	try\
	{\
		return Windows::App{}.Entry();\
	}\
	catch (const std::exception& ex)\
	{\
		std::cerr << ex.what() << std::endl;\
	}\
	return -1;\
}