#include "DXResources.h"
#include "DXExceptionMacro.h"

using namespace Microsoft::WRL;
using namespace Hardware::DX;

DXResources::DXResources()
    : ClientRect({})
{ }

void DXResources::CreateDeviceAndSwapChain(HWND hwnd)
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
        D3D11_SDK_VERSION, &scDesc, &SwapChain, &Device, nullptr, &Context));
}

void DXResources::CreateRTV()
{
    // gain access to texture subresource in swap chain (back buffer)
    ComPtr<ID3D11Resource> pBackBuffer;
    ThrowIfFailed(
        SwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &pBackBuffer));
    ThrowIfFailed(
        Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &RTV));
}

void DXResources::CreateDSV()
{
    // create depth stencil desc
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable              = TRUE;
    dsDesc.DepthFunc                = D3D11_COMPARISON_LESS;
    dsDesc.DepthWriteMask           = D3D11_DEPTH_WRITE_MASK_ALL;
    ComPtr<ID3D11DepthStencilState> pDSstate;
    ThrowIfFailed(Device->CreateDepthStencilState(&dsDesc, &pDSstate));

    // bind depth stencil state
    Context->OMSetDepthStencilState(pDSstate.Get(), 1u);

    // create depth stencil texture
    ComPtr<ID3D11Texture2D> pDepthStencil;
    D3D11_TEXTURE2D_DESC    txDesc = {};
    txDesc.Width                   = ClientRect.right;
    txDesc.Height                  = ClientRect.bottom;
    txDesc.ArraySize               = 1u;
    txDesc.MipLevels               = 1u;
    txDesc.SampleDesc.Count        = 1u;
    txDesc.SampleDesc.Quality      = 0u;
    txDesc.CPUAccessFlags          = 0u;
    txDesc.Usage                   = D3D11_USAGE_DEFAULT;
    txDesc.Format                  = DXGI_FORMAT_D32_FLOAT;
    txDesc.BindFlags               = D3D11_BIND_DEPTH_STENCIL;
    ThrowIfFailed(Device->CreateTexture2D(&txDesc, nullptr, &pDepthStencil));

    // create view of depth stencil texture
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format                        = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension                 = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice            = 0u;
    ThrowIfFailed(
        Device->CreateDepthStencilView(pDepthStencil.Get(), &dsvDesc, &DSV));
}

