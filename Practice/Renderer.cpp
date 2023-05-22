#include "Renderer.h"

#pragma comment(lib, "d3d11.lib")

using namespace Hardware;
using namespace Microsoft::WRL;

Renderer::Renderer(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC sd               = {0};
    sd.BufferDesc.Width                   = 0;
    sd.BufferDesc.Height                  = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.RefreshRate.Numerator   = 0;
    sd.BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count            = 1;
    sd.SampleDesc.Quality          = 0;
    sd.BufferUsage                 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount                 = 1;
    sd.OutputWindow                = hwnd;
    sd.Windowed                    = TRUE;
    sd.SwapEffect                  = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags                       = 0;

    // create device and front/back buffers, and swap chain and rendering
    // context
    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                                  nullptr, 0, D3D11_SDK_VERSION, &sd,
                                  &m_pSwapChain, &m_pDevice, nullptr,
                                  &m_pContext);

    ComPtr<ID3D11Resource> pResource;
    m_pSwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &pResource);
    m_pDevice->CreateRenderTargetView(pResource.Get(), nullptr,
                                      &m_pRenderTargetView);
}
void Hardware::Renderer::EndFrame() noexcept { m_pSwapChain->Present(1u, 0u); }
void Hardware::Renderer::ClearBuffer(float r, float g, float b) noexcept
{
    const float color[] = {r, g, b, 1.0f};
    m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
}
