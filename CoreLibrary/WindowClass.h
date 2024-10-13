#pragma once

#include <string>

namespace Windows
{
class WindowClass
{
	friend class Window;
public:
	WindowClass(const wchar_t* name);
	~WindowClass();
	WindowClass(const WindowClass&) = delete;
	WindowClass& operator=(const WindowClass&) = delete;
public:
	const wchar_t* GetName() const;
private:
	HICON icon32() const;
	HICON icon16() const;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgAdapter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	HINSTANCE _hInst;
	std::wstring _name;
};
}

