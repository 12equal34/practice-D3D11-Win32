#include "DXResource.h"
#include "DXExceptionMacro.h"

namespace wrl = Microsoft::WRL;
namespace HDX = Hardware::DX;

HDX::DXResource HDX::DXResource::Instance;

void HDX::DXResource::Initialize(HWND hwnd)
{
    CreateDeviceAndSwapChain(hwnd);
    CreateRTV();
    CreateDSV();
}

void HDX::DXResource::CreateDeviceAndSwapChain(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC scDesc               = {};
    scDesc.BufferDesc.Width                   = 0;
    scDesc.BufferDesc.Height                  = 0;
    scDesc.BufferDesc.RefreshRate.Denominator = 0;
    scDesc.BufferDesc.RefreshRate.Numerator   = 0;
    scDesc.BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM;
    scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scDesc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;
    scDesc.SampleDesc.Count            = 1;
    scDesc.SampleDesc.Quality          = 0;
    scDesc.BufferUsage                 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.BufferCount                 = 1;
    scDesc.OutputWindow                = hwnd;
    scDesc.Windowed                    = TRUE;
    scDesc.SwapEffect                  = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.Flags                       = 0;

    UINT swapCreateFlags = 0u;
#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // create device and front/back buffers, and swap chain and rendering
    // context
    ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, swapCreateFlags, nullptr, 0,
        D3D11_SDK_VERSION, &scDesc, &GetSwapchain(), &GetDevice(), nullptr,
        &GetContext()));
}

void HDX::DXResource::CreateRTV()
{
    // gain access to texture subresource in swap chain (back buffer)
    wrl::ComPtr<ID3D11Resource> pBackBuffer;
    ThrowIfFailed(
        GetSwapchain()->GetBuffer(0u, __uuidof(ID3D11Resource), &pBackBuffer));
    ThrowIfFailed(GetDevice()->CreateRenderTargetView(pBackBuffer.Get(),
                                                      nullptr, &GetRTV()));
}

void HDX::DXResource::CreateDSV()
{
    // create depth stencil desc
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable              = TRUE;
    dsDesc.DepthFunc                = D3D11_COMPARISON_LESS;
    dsDesc.DepthWriteMask           = D3D11_DEPTH_WRITE_MASK_ALL;
    wrl::ComPtr<ID3D11DepthStencilState> pDSstate;
    ThrowIfFailed(GetDevice()->CreateDepthStencilState(&dsDesc, &pDSstate));

    // bind depth stencil state
    GetContext()->OMSetDepthStencilState(pDSstate.Get(), 1u);

    // create depth stencil texture
    wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
    D3D11_TEXTURE2D_DESC         txDesc = {};
    txDesc.Width                        = GetClientRectangle().right;
    txDesc.Height                       = GetClientRectangle().bottom;
    txDesc.ArraySize                    = 1u;
    txDesc.MipLevels                    = 1u;
    txDesc.SampleDesc.Count             = 1u;
    txDesc.SampleDesc.Quality           = 0u;
    txDesc.CPUAccessFlags               = 0u;
    txDesc.Usage                        = D3D11_USAGE_DEFAULT;
    txDesc.Format                       = DXGI_FORMAT_D32_FLOAT;
    txDesc.BindFlags                    = D3D11_BIND_DEPTH_STENCIL;
    ThrowIfFailed(
        GetDevice()->CreateTexture2D(&txDesc, nullptr, &pDepthStencil));

    // create view of depth stencil texture
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format                        = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension                 = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice            = 0u;
    ThrowIfFailed(GetDevice()->CreateDepthStencilView(pDepthStencil.Get(),
                                                      &dsvDesc, &GetDSV()));
}

wrl::ComPtr<ID3D11Device>& HDX::DXResource::GetDevice() noexcept
{
    return Instance.m_device;
}

wrl::ComPtr<ID3D11DeviceContext>& HDX::DXResource::GetContext() noexcept
{
    return Instance.m_context;
}

wrl::ComPtr<IDXGISwapChain>& HDX::DXResource::GetSwapchain() noexcept
{
    return Instance.m_swapchain;
}

wrl::ComPtr<ID3D11RenderTargetView>& HDX::DXResource::GetRTV() noexcept
{
    return Instance.m_rtv;
}

wrl::ComPtr<ID3D11DepthStencilView>& HDX::DXResource::GetDSV() noexcept
{
    return Instance.m_dsv;
}

RECT& HDX::DXResource::GetClientRectangle() noexcept
{
    return Instance.m_clientRect;
}