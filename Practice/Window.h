#pragma once
#include <Windows.h>
#include <string>

class Window
{
public:
    Window(int width, int height, std::wstring_view titleName) noexcept;
    ~Window();
    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

private:
    class WindowClass
    {
    public:
        static const wchar_t* GetName() noexcept;
        static HINSTANCE      GetInstance() noexcept;

    private:
        WindowClass() noexcept;
        ~WindowClass();
        WindowClass(const WindowClass&)            = delete;
        WindowClass& operator=(const WindowClass&) = delete;

        static constexpr const wchar_t* WndClassName =
            L"practice-D3D11 Win32 Class";
        static WindowClass WndClass;
        HINSTANCE          m_hInst;
    };

    static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam,
                                           LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgAdapter(HWND hwnd, UINT msg, WPARAM wParam,
                                             LPARAM lParam) noexcept;
    LRESULT                 HandleMsg(HWND hwnd, UINT msg, WPARAM wParam,
                                      LPARAM lParam) noexcept;

private:
    int  m_width;
    int  m_height;

    std::wstring m_titleName;

    HWND m_hwnd;
};
