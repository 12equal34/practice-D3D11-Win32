#pragma once
#include "WindowsHeader.h"
#include <string>
#include <memory>
#include "BaseException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timer.h"
#include "Renderer.h"

namespace Hardware
{
class Window
{
public:
    class Exception : public BaseException
    {
        using BaseException::BaseException;
    public:
        static std::string TranslateErrorCode(HRESULT hr) noexcept;
    };
    class HrException : public Exception
    {
    public:
        HrException(int line, const char* file, HRESULT hr) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        HRESULT     GetErrorCode() const noexcept;
        std::string GetErrorDescription() const noexcept;
    private:
        HRESULT m_hr;
    };
    class NoRendererException : public Exception
    {
    public:
        using Exception::Exception;
        const char* GetType() const noexcept override;
    };

public:
    Window(int width, int height, std::wstring_view titleName);
    ~Window();
    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

    void SetKeyboard(Hardware::Keyboard* pKeyboard) noexcept;
    void SetMouse(Hardware::Mouse* pMouse) noexcept;
    void SetTimer(Timer* pTimer) noexcept;

    void                SetTitle(std::wstring_view titleName);
    HWND                GetHwnd() const noexcept;
    Hardware::DX::Renderer& GetRenderer() const noexcept;

    int GetWidth() const noexcept;
    int GetHeight() const noexcept;
    float GetAspectRatio() const noexcept;

private:
    class WindowClass
    {
    public:
        static const wchar_t* GetName() noexcept;
        static HINSTANCE      GetInstance() noexcept;

    private:
        WindowClass();
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

#if (!defined NDEBUG && defined DEBUG_WINMSG)
    static void MsgToOutputDebug(UINT msg, WPARAM wParam,
                                 LPARAM lParam) noexcept;
#endif

private:
    int m_width;
    int m_height;

    std::wstring m_titleName;

    HWND m_hwnd;

    Hardware::Keyboard* m_pKeyboard;
    Hardware::Mouse*    m_pMouse;
    Timer*              m_pTimer;

    std::unique_ptr<Hardware::DX::Renderer> m_pRenderer;
};
}
