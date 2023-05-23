#pragma once
#include "WindowsHeader.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <string>
#include "BaseException.h"

namespace Hardware
{
class Renderer
{
public:
    class Exception : public BaseException
    {
        using BaseException::BaseException;
    };
    class InfoException : public Exception
    {
    public:
        InfoException(int line, const char* file,
                      std::vector<std::string> infoMsgs) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        std::string GetErrorInfo() const noexcept;
    private:
        std::string m_info;
    };
    class HrException : public Exception
    {
    public:
        HrException(int line, const char* file, HRESULT hr,
                    std::vector<std::string> infoMsgs = {}) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        HRESULT     GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
        std::string GetErrorDescription() const noexcept;
        std::string GetErrorInfo() const noexcept;
    private:
        HRESULT     m_hr;
        std::string m_info;
    };
    class DeviceRemovedException : public HrException
    {
        using HrException::HrException;
    public:
        const char* GetType() const noexcept override;
    private:
        std::string m_reason;
    };

public:
    Renderer(HWND hwnd);
    ~Renderer()                          = default;
    Renderer(const Renderer&)            = delete;
    Renderer& operator=(const Renderer&) = delete;

    void EndFrame() noexcept;
    void ClearBuffer(float r, float g, float b) noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D11Device>           m_pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_pContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain>         m_pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
};
}
