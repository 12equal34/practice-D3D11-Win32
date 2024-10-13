#pragma once

#include "Windows.h"
#include <string>

namespace Windows
{
class WindowClass;
class Window
{
public:
	Window(RECT& windowRect, const wchar_t* name, WindowClass* windowClass);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
public:
	LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool HandleMsg_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool HandleMsg_Keyboard(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool HandleMsg_Mouse(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool HandleMsg_RawInput(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	const wchar_t* GetName() const;
private:
	int _width;
	int _height;
	std::wstring _name;
	HWND _hwnd;
	WindowClass* _class;
};
}


