#include "pch.h"
#include "App.h"

#include "Timer.h"
#include "Window.h"
#include "WindowClass.h"

#include <assert.h>
#include <iostream>

Windows::App::App()
	: _appState(Windows::App::AppState::Init)
	, _windowsMsg()
{
	RECT windowRect{};
	windowRect.left = 100;
	windowRect.top = 100;
	windowRect.right = 300;
	windowRect.bottom = 300;
	_mainWindowClass = std::make_unique<Windows::WindowClass>(L"main_window");
	_mainWindow = std::make_unique<Windows::Window>(windowRect, L"window title", _mainWindowClass.get());
	_timer = std::make_unique<Windows::Timer>();
}

Windows::App::~App()
{
}

int Windows::App::Entry()
{
	return static_cast<int>(MessagePump());
}

WPARAM Windows::App::MessagePump()
{
	while (true)
	{
		BOOL bGotMsg = PeekMessageW(&_windowsMsg, NULL, 0, 0, PM_REMOVE);
		
		if (bGotMsg == 0)
		{
			switch (_appState)
			{
			case AppState::Init:
			{
				Init();
				_appState = AppState::Run;
				break;
			}
			case AppState::Run:
			{
				double dt = _timer->ElapsedSeconds();
				Run(dt);
				break;
			}
			case AppState::Reset:
			{
				Reset();
				_appState = AppState::Run;
				break;
			}
			default:
			{
				assert(false);
				break;
			}
			}
		}
		else if (bGotMsg != -1)
		{
			TranslateMessage(&_windowsMsg);
			DispatchMessageW(&_windowsMsg);

			if (_windowsMsg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			DWORD error = GetLastError();
			std::cerr << "PeekMessage failed with error: " << error << std::endl;
		}
	}
	return _windowsMsg.wParam;
}

void Windows::App::Init()
{

}

void Windows::App::Run(double dt)
{

}

void Windows::App::Reset()
{

}