#pragma once
#include "WindowsHeader.h"
#include <d3d11.h>
#include <wrl.h>
#include <memory>

namespace Hardware::DX
{
class DXResources final
{
public:
    DXResources();
    ~DXResources()                             = default;
    DXResources(const DXResources&)            = delete;
    DXResources& operator=(const DXResources&) = delete;

    void CreateDeviceAndSwapChain(HWND hwnd);
    void CreateRTV();
    void CreateDSV();

public:
    Microsoft::WRL::ComPtr<ID3D11Device>           Device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    Context;
    Microsoft::WRL::ComPtr<IDXGISwapChain>         SwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RTV;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSV;
    RECT                                           ClientRect;
};
}