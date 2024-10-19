#pragma once

#include "Windows.h"
#include <string>

namespace Windows
{
class WindowClass;
class Window
{
public:
	Window(RECT& windowRect, const std::wstring& name, WindowClass* windowClass);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
public:
	LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool HandleMsg_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool HandleMsg_Keyboard(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool HandleMsg_Mouse(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool HandleMsg_RawInput(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	const wchar_t* GetName() const;
	FORCEINLINE HWND GetHwnd() const { return _hwnd; }
private:
	int _width;
	int _height;
	std::wstring _name;
	HWND _hwnd;
	WindowClass* _class;
};
}


