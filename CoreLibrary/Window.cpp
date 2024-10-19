#include "pch.h"
#include "Window.h"

#include "Windows.h"
#include "WindowClass.h"

#include <exception>
#include <assert.h>

#define ThrowIfNull(x) if ((x) == NULL) { throw std::exception{}; }

Windows::Window::Window(RECT& windowRect, const std::wstring& name, WindowClass* windowClass)
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
	if (HandleMsg_Window(hwnd, msg, wParam, lParam))   return 0;
	if (HandleMsg_Keyboard(hwnd, msg, wParam, lParam)) return 0;
	if (HandleMsg_Mouse(hwnd, msg, wParam, lParam))    return 0;
	if (HandleMsg_RawInput(hwnd, msg, wParam, lParam)) return 0;
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool Windows::Window::HandleMsg_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return true;
	}
#pragma region 사용하지 않는 메시지들
	/*case WM_ACTIVATEAPP:
	case WM_CANCELMODE:
	case WM_CHILDACTIVATE:
	case WM_COMPACTING:
	case WM_CREATE:
	case WM_DESTROY:
	case WM_DPICHANGED:
	case WM_ENABLE:
	case WM_ENTERSIZEMOVE:
	case WM_EXITSIZEMOVE:
	case WM_GETICON:
	case WM_GETMINMAXINFO:
	case WM_INPUTLANGCHANGE:
	case WM_INPUTLANGCHANGEREQUEST:
	case WM_MOVE:
	case WM_MOVING:
	case WM_NCACTIVATE:
	case WM_NCCALCSIZE:
	case WM_NCCREATE:
	case WM_NCDESTROY:
	case WM_NULL:
	case WM_QUERYDRAGICON:
	case WM_QUERYOPEN:
	case WM_QUIT:
	case WM_SHOWWINDOW:
	case WM_SIZE:
	case WM_SIZING:
	case WM_STYLECHANGED:
	case WM_STYLECHANGING:
	case WM_THEMECHANGED:
	case WM_USERCHANGED:
	case WM_WINDOWPOSCHANGED:
	case WM_WINDOWPOSCHANGING:
		return true;*/
#pragma endregion
	default:
		return false;
	}
}

bool Windows::Window::HandleMsg_Keyboard(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
#pragma region 사용하지 않는 메시지들
	/*case WM_ACTIVATE:
	case WM_APPCOMMAND:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_HOTKEY:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_KILLFOCUS:
	case WM_SETFOCUS:
	case WM_SYSDEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_UNICHAR:
		return true;*/
#pragma endregion
	default:
		return false;
	}
}

bool Windows::Window::HandleMsg_Mouse(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
#pragma region 사용하지 않는 메시지들
	/*case WM_CAPTURECHANGED:
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEACTIVATE:
	case WM_MOUSEHOVER:
	case WM_MOUSEHWHEEL:
	case WM_MOUSELEAVE:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_NCHITTEST:
	case WM_NCLBUTTONDBLCLK:
	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_NCMBUTTONDBLCLK:
	case WM_NCMBUTTONDOWN:
	case WM_NCMBUTTONUP:
	case WM_NCMOUSEHOVER:
	case WM_NCMOUSELEAVE:
	case WM_NCMOUSEMOVE:
	case WM_NCRBUTTONDBLCLK:
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONUP:
	case WM_NCXBUTTONDBLCLK:
	case WM_NCXBUTTONDOWN:
	case WM_NCXBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_XBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		return true;*/
#pragma endregion
	default:
		return false;
	}
}

bool Windows::Window::HandleMsg_RawInput(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
#pragma region 사용하지 않는 메시지들
	/*case WM_INPUT:
	case WM_INPUT_DEVICE_CHANGE:
		return true;*/
#pragma endregion
	default:
		return false;
	}
}