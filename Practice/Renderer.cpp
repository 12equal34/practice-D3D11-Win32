#include "Renderer.h"
#include <sstream>
#include <DirectXMath.h>
#include "DXExceptionMacro.h"
#include "WinExceptionHelper.h"
#include "BindableHeader.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

using namespace Hardware;
using namespace Hardware::DX;
using namespace Microsoft::WRL;
using namespace DirectX;

Renderer::Renderer(HWND hwnd)
{
    ThrowIfNull(GetClientRect(hwnd, &m_dx.ClientRect));

    m_dx.CreateDeviceAndSwapChain(hwnd);
    m_dx.CreateRTV();
    m_dx.CreateDSV();

    // bind depth stencil view to OM
    m_dx.Context->OMSetRenderTargets(1u, m_dx.RTV.GetAddressOf(),
                                     m_dx.DSV.Get());
}
void Renderer::EndFrame() { ThrowIfFailed(m_dx.SwapChain->Present(0u, 0u)); }
void Renderer::ClearBuffer(float r, float g, float b) noexcept
{
    const float color[] = {r, g, b, 1.0f};
    m_dx.Context->ClearRenderTargetView(m_dx.RTV.Get(), color);
    m_dx.Context->ClearDepthStencilView(m_dx.DSV.Get(), D3D11_CLEAR_DEPTH, 1.0f,
                                        0u);
}

// test code
void Renderer::DrawTest(float dt, float x, float z)
{
    std::vector<std::unique_ptr<Bindable>> bindings;

    bindings.push_back(std::move(std::make_unique<VertexBuffer>(*this)));
    bindings.push_back(std::move(std::make_unique<IndexBuffer>(*this)));
    bindings.push_back(std::move(std::make_unique<Viewport>(
        *this, static_cast<FLOAT>(m_dx.ClientRect.right),
        static_cast<FLOAT>(m_dx.ClientRect.bottom))));
    bindings.push_back(std::move(
        std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)));

    auto pVertexShader =
        std::make_unique<VertexShader>(*this, L"Shaders/VertexShader.cso");
    bindings.push_back(
        std::move(std::make_unique<InputLayout>(*this, *pVertexShader.get())));
    bindings.push_back(std::move(pVertexShader));
    bindings.push_back(std::move(
        std::make_unique<PixelShader>(*this, L"Shaders/PixelShader.cso")));

    for (auto& binding : bindings) {
        binding->Bind(*this);
    }

    // cube 1
    auto pConstantBuffer = std::make_unique<ConstantBuffer>(*this, dt, x, z);
    pConstantBuffer->Bind(*this);
    m_dx.Context->DrawIndexed(36u, 0, 0);

    // cube 2
    pConstantBuffer = std::make_unique<ConstantBuffer>(*this, -dt, 0.0f, 0.0f);
    pConstantBuffer->Bind(*this);
    m_dx.Context->DrawIndexed(36u, 0, 0);
}