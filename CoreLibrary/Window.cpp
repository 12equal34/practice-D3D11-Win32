#include "pch.h"
#include "Window.h"

#include "Windows.h"
#include "WindowClass.h"

#include <exception>
#include <assert.h>

#define ThrowIfNull(x) if ((x) == NULL) { throw std::exception{}; }

Windows::Window::Window(RECT& windowRect, const wchar_t* name, WindowClass* windowClass)
	: _name(name)
	, _class(windowClass)
{
	DWORD dwStyle = WS_CAPTION | WS_MINIMIZE | WS_SYSMENU;

	ThrowIfNull(AdjustWindowRect(&windowRect, dwStyle, false));
	_width = windowRect.right - windowRect.left;
	_height = windowRect.bottom - windowRect.top;

	int cursorPosX = windowRect.left + _width / 2;
	int cursorPosY = windowRect.top + _height / 2;
	ThrowIfNull(SetCursorPos(cursorPosX, cursorPosY));

	_hwnd = CreateWindowExW(0L, _class->GetName(), GetName(), dwStyle,
		windowRect.left, windowRect.top, _width, _height, nullptr, nullptr, 
		_class->_hInst, this);

	RAWINPUTDEVICE rawInputDevice{};
	rawInputDevice.usUsagePage = 0x01; // 일반적인 데스크톱 컨트롤들
	rawInputDevice.usUsage = 0x02; // 마우스 사용
	rawInputDevice.dwFlags = 0;
	rawInputDevice.hwndTarget = _hwnd;
	ThrowIfNull(RegisterRawInputDevices(&rawInputDevice, 1u, sizeof(rawInputDevice)));

	ShowWindow(_hwnd, SW_SHOWDEFAULT); // 만약 이전에 윈도우가 숨겨졌다면, 0을 리턴
}

Windows::Window::~Window()
{
	DestroyWindow(_hwnd);
}

const wchar_t* Windows::Window::GetName() const
{
	return _name.c_str();
}

LRESULT Windows::Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}