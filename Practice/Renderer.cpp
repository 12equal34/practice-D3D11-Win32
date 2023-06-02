#include "Renderer.h"
#include <sstream>
#include <DirectXMath.h>
#include "DXExceptionMacro.h"
#include "WinExceptionHelper.h"
#include "BindableHeader.h"
#include "Surface.h"

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
    m_dx.Context->OMSetRenderTargets(1u, m_dx.RTV.GetAddressOf(), nullptr);
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
// void Renderer::DrawTest(float dt, float x, float z)
//{
//    std::vector<std::unique_ptr<Bindable>> bindings;
//
//    bindings.push_back(std::move(std::make_unique<VertexBuffer>(*this)));
//    bindings.push_back(std::move(std::make_unique<IndexBuffer>(*this)));
//    bindings.push_back(std::move(std::make_unique<Viewport>(
//        *this, static_cast<FLOAT>(m_dx.ClientRect.right),
//        static_cast<FLOAT>(m_dx.ClientRect.bottom))));
//    bindings.push_back(std::move(
//        std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)));
//
//    auto pVertexShader =
//        std::make_unique<VertexShader>(*this, L"Shaders/VertexShader.cso");
//    bindings.push_back(
//        std::move(std::make_unique<InputLayout>(*this,
//        *pVertexShader.get())));
//    bindings.push_back(std::move(pVertexShader));
//    bindings.push_back(std::move(
//        std::make_unique<PixelShader>(*this, L"Shaders/PixelShader.cso")));
//
//    for (auto& binding : bindings) {
//        binding->Bind(*this);
//    }
//
//    // cube 1
//    auto pConstantBuffer = std::make_unique<ConstantBuffer>(*this, dt, x, z);
//    pConstantBuffer->Bind(*this);
//    m_dx.Context->DrawIndexed(36u, 0, 0);
//
//    // cube 2
//    pConstantBuffer = std::make_unique<ConstantBuffer>(*this, -dt, 0.0f,
//    0.0f); pConstantBuffer->Bind(*this); m_dx.Context->DrawIndexed(36u, 0, 0);
//}

void Hardware::DX::Renderer::DrawTestSurface(float x, float z, float angle)
{
    constexpr float pi = 3.14159265f;
    Surface         surface(20, 20);
    surface.Bind(m_dx, *this);

    // constant buffer for vs
    {
        struct ConstantBuffer1 {
            XMMATRIX transform;
        };
        const ConstantBuffer1 cbuf1 = {XMMatrixTranspose(
            /* clang-format off */
            XMMatrixRotationX(x-pi * 0.3f) *
            XMMatrixRotationZ(z) *
            XMMatrixTranslation(0.0f, 0.0f, 20.0f) *
            XMMatrixPerspectiveLH(1.0f, 0.6f, 0.5f, 100.0f)
            /* clang-format on */
            )};

        ComPtr<ID3D11Buffer> pCBuf1;
        D3D11_BUFFER_DESC    cbd   = {};
        cbd.Usage                  = D3D11_USAGE_DYNAMIC;
        cbd.BindFlags              = D3D11_BIND_CONSTANT_BUFFER;
        cbd.CPUAccessFlags         = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags              = 0u;
        cbd.ByteWidth              = sizeof(cbuf1);
        cbd.StructureByteStride    = 0u;
        D3D11_SUBRESOURCE_DATA csd = {};
        csd.pSysMem                = &cbuf1;
        ThrowIfFailed(m_dx.Device->CreateBuffer(&cbd, &csd, &pCBuf1));

        m_dx.Context->VSSetConstantBuffers(0u, 1u, pCBuf1.GetAddressOf());
    }

    // constant buffer for ps
    {
        struct ConstantBuffer2 {
            struct {
                float r;
                float g;
                float b;
                float a;
            } faceColor[2];
        };
        const ConstantBuffer2 cbuf2 = {
            {
             {0.3f, 0.3f, 1.0f, 1.0f},
             {0.2f, 0.2f, 0.8f, 1.0f},
             }
        };

        ComPtr<ID3D11Buffer> pCBuf2;
        D3D11_BUFFER_DESC    cbd2   = {};
        cbd2.Usage                  = D3D11_USAGE_DEFAULT;
        cbd2.BindFlags              = D3D11_BIND_CONSTANT_BUFFER;
        cbd2.CPUAccessFlags         = 0u;
        cbd2.MiscFlags              = 0u;
        cbd2.ByteWidth              = sizeof(cbuf2);
        cbd2.StructureByteStride    = 0u;
        D3D11_SUBRESOURCE_DATA csd2 = {};
        csd2.pSysMem                = &cbuf2;
        ThrowIfFailed(m_dx.Device->CreateBuffer(&cbd2, &csd2, &pCBuf2));

        m_dx.Context->PSSetConstantBuffers(0u, 1u, pCBuf2.GetAddressOf());
    }

    m_dx.Context->DrawIndexed(surface.GetIndexCount(), 0u, 0u);

    // constant buffer for ps
    {
        struct ConstantBuffer2
        {
            struct
            {
                float r;
                float g;
                float b;
                float a;
            } faceColor[2];
        };
        const ConstantBuffer2 cbuf2 = {
            {
             {0.0f, 0.0f, 0.0f, 1.0f},
             {0.0f, 0.0f, 0.0f, 1.0f},
             }
        };

        ComPtr<ID3D11Buffer> pCBuf2;
        D3D11_BUFFER_DESC    cbd2 = {};
        cbd2.Usage = D3D11_USAGE_DEFAULT;
        cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd2.CPUAccessFlags = 0u;
        cbd2.MiscFlags = 0u;
        cbd2.ByteWidth = sizeof(cbuf2);
        cbd2.StructureByteStride = 0u;
        D3D11_SUBRESOURCE_DATA csd2 = {};
        csd2.pSysMem = &cbuf2;
        ThrowIfFailed(m_dx.Device->CreateBuffer(&cbd2, &csd2, &pCBuf2));

        m_dx.Context->PSSetConstantBuffers(0u, 1u, pCBuf2.GetAddressOf());
    }

    m_dx.Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    m_dx.Context->DrawIndexed(surface.GetIndexCount(), 0u, 0u);
}
