#pragma once
#include "WindowsHeader.h"
#include <d3d11.h>
#include <wrl.h>

namespace Hardware::DX
{
class DXResource final
{
private:
    static DXResource s_instance;

public:
    static void Initialize(HWND hwnd);

    static void CreateDeviceAndSwapChain(HWND hwnd);
    static void CreateRTV();
    static void CreateDSV();

    static Microsoft::WRL::ComPtr<ID3D11Device>&        GetDevice() noexcept;
    static Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetContext() noexcept;
    static Microsoft::WRL::ComPtr<IDXGISwapChain>&      GetSwapchain() noexcept;
    static Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetRTV() noexcept;
    static Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDSV() noexcept;
    static RECT& GetClientRectangle() noexcept;
    static LONG GetWindowWidth() noexcept;
    static LONG GetWindowHeight() noexcept;
private:
    DXResource() noexcept                    = default;
    ~DXResource()                            = default;
    DXResource(const DXResource&)            = delete;
    DXResource& operator=(const DXResource&) = delete;
private:
    Microsoft::WRL::ComPtr<ID3D11Device>           m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_context;
    Microsoft::WRL::ComPtr<IDXGISwapChain>         m_swapchain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;
    RECT                                           m_clientRect;
};
}