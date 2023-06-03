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

void Hardware::DX::Renderer::DrawTestSurface(const Camera& camera, float x,
                                             float z, float angle)
{
    constexpr float pi = 3.14159265f;
    Surface         surface(200, 200);
    surface.Bind(m_dx, *this);

    // constant buffer for vs
    {

        struct Transform {
            XMMATRIX model;
            XMMATRIX modelView;
            XMMATRIX modelViewProj;
        };

        XMMATRIX model = XMMatrixIdentity();
        XMMATRIX view   = camera.GetView();
        XMMATRIX proj   = camera.GetProjection();

        const Transform transBufData = {XMMatrixTranspose(model),
                                        XMMatrixTranspose(model * view),
                                        XMMatrixTranspose(model * view * proj)};

        ComPtr<ID3D11Buffer> pTransBuf;
        D3D11_BUFFER_DESC    cbd   = {};
        cbd.Usage                  = D3D11_USAGE_DYNAMIC;
        cbd.BindFlags              = D3D11_BIND_CONSTANT_BUFFER;
        cbd.CPUAccessFlags         = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags              = 0u;
        cbd.ByteWidth              = sizeof(transBufData);
        cbd.StructureByteStride    = 0u;
        D3D11_SUBRESOURCE_DATA csd = {};
        csd.pSysMem                = &transBufData;
        ThrowIfFailed(m_dx.Device->CreateBuffer(&cbd, &csd, &pTransBuf));

        m_dx.Context->VSSetConstantBuffers(0u, 1u, pTransBuf.GetAddressOf());
    }

    // constant buffer for ps
    {
        struct FaceColor {
            struct {
                float r;
                float g;
                float b;
                float a;
            } faceColor[2];
        };
        const FaceColor faceColorBufData = {
            {
             {0.3f, 0.3f, 1.0f, 1.0f},
             {0.2f, 0.2f, 0.8f, 1.0f},
             }
        };

        ComPtr<ID3D11Buffer> pfaceColorBuf;
        D3D11_BUFFER_DESC    cbd2   = {};
        cbd2.Usage                  = D3D11_USAGE_DEFAULT;
        cbd2.BindFlags              = D3D11_BIND_CONSTANT_BUFFER;
        cbd2.CPUAccessFlags         = 0u;
        cbd2.MiscFlags              = 0u;
        cbd2.ByteWidth              = sizeof(faceColorBufData);
        cbd2.StructureByteStride    = 0u;
        D3D11_SUBRESOURCE_DATA csd2 = {};
        csd2.pSysMem                = &faceColorBufData;
        ThrowIfFailed(m_dx.Device->CreateBuffer(&cbd2, &csd2, &pfaceColorBuf));

        m_dx.Context->PSSetConstantBuffers(0u, 1u,
                                           pfaceColorBuf.GetAddressOf());
    }

    m_dx.Context->DrawIndexed(surface.GetIndexCount(), 0u, 0u);

    // constant buffer for ps
    {
        struct LineColor {
            struct {
                float r;
                float g;
                float b;
                float a;
            } faceColor[2];
        };
        const LineColor lineColorBufData = {
            {
             {0.0f, 0.0f, 0.0f, 1.0f},
             {0.0f, 0.0f, 0.0f, 1.0f},
             }
        };

        ComPtr<ID3D11Buffer> plineColorBuf;
        D3D11_BUFFER_DESC    cbd2   = {};
        cbd2.Usage                  = D3D11_USAGE_DEFAULT;
        cbd2.BindFlags              = D3D11_BIND_CONSTANT_BUFFER;
        cbd2.CPUAccessFlags         = 0u;
        cbd2.MiscFlags              = 0u;
        cbd2.ByteWidth              = sizeof(lineColorBufData);
        cbd2.StructureByteStride    = 0u;
        D3D11_SUBRESOURCE_DATA csd2 = {};
        csd2.pSysMem                = &lineColorBufData;
        ThrowIfFailed(m_dx.Device->CreateBuffer(&cbd2, &csd2, &plineColorBuf));

        m_dx.Context->PSSetConstantBuffers(0u, 1u,
                                           plineColorBuf.GetAddressOf());
    }

    m_dx.Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    m_dx.Context->DrawIndexed(surface.GetIndexCount(), 0u, 0u);
}
