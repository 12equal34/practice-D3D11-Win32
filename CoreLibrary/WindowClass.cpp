#include "pch.h"
#include "WindowClass.h"

#include "Window.h"

#include <exception>

#define ThrowIfNull(x) if ((x) == NULL) { throw std::exception{}; }

Windows::WindowClass::WindowClass(const wchar_t* name)
	: _hInst(GetModuleHandleW(nullptr))
    , _name(name)
{
	WNDCLASSEX wndClassEx{};
    wndClassEx.cbSize = sizeof(wndClassEx);
    wndClassEx.style = CS_OWNDC;
    wndClassEx.lpfnWndProc = HandleMsgSetup;
    wndClassEx.cbClsExtra = 0;
    wndClassEx.cbWndExtra = 0;
    wndClassEx.hInstance = _hInst;
    wndClassEx.hIcon = icon32();
    wndClassEx.hCursor = nullptr;
    wndClassEx.hbrBackground = nullptr;
    wndClassEx.lpszMenuName = nullptr;
    wndClassEx.lpszClassName = GetName();
    wndClassEx.hIconSm = icon16();
    ThrowIfNull(RegisterClassEx(&wndClassEx));
}

Windows::WindowClass::~WindowClass()
{
    UnregisterClass(GetName(), _hInst);
}

LRESULT Windows::WindowClass::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const window = static_cast<Window*>(createStruct->lpCreateParams);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowClass::HandleMsgAdapter));
        return window->HandleMsg(hwnd, msg, wParam, lParam);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Windows::WindowClass::HandleMsgAdapter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* const window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    return window->HandleMsg(hwnd, msg, wParam, lParam);
}

const wchar_t* Windows::WindowClass::GetName() const
{
    return _name.c_str();
}

HICON Windows::WindowClass::icon32() const
{
    //return static_cast<HICON>(LoadImage(_hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    return nullptr;
}
HICON Windows::WindowClass::icon16() const
{
    //return static_cast<HICON>(LoadImage(_hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
    return nullptr;
}