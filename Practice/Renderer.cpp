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
                                             float z, float time)
{
    constexpr float pi = 3.14159265f;
    Surface         surface(50, 50);
    surface.Bind(m_dx, *this);

    // constant buffer for transform in VS
    {
        struct Transform {
            XMMATRIX model;
            XMMATRIX modelView;
            XMMATRIX modelViewProj;
        };

        XMMATRIX model = XMMatrixIdentity();
        XMMATRIX view  = camera.GetView();
        XMMATRIX proj  = camera.GetProjection();

        const Transform transBufData = {XMMatrixTranspose(model),
                                        XMMatrixTranspose(model * view),
                                        XMMatrixTranspose(model * view * proj)};

        ConstantBuffer transformCbuf(*this, sizeof(transBufData),
                                     &transBufData);
        transformCbuf.SetToVertexShader(*this, 0u);
    }

    // constant buffer for wave parameters in VS
    {
        struct WaveParameter {
            float time;
            float wave_amplitude;
            float wave_phase;
            struct {
                float x;
                float z;
            } wave_direction;
            float _1;
            float _2;
            float _3;
        };

        const WaveParameter wavePrameterData = {time, 2.0f, 1.0f, x, z};

        ConstantBuffer waveParameterCbuf(*this, sizeof(wavePrameterData),
                                        &wavePrameterData);
        waveParameterCbuf.SetToVertexShader(*this, 1u);
    }

    // constant buffer for face color in PS
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

        ConstantBuffer faceColorCbuf(*this, sizeof(faceColorBufData),
                                     &faceColorBufData);
        faceColorCbuf.SetToPixelShader(*this, 0u);
    }

    m_dx.Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_dx.Context->DrawIndexed(surface.GetIndexCount(), 0u, 0u);

    // constant buffer for line color in PS
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
        ConstantBuffer lineColorCbuf(*this, sizeof(lineColorBufData),
                                     &lineColorBufData);
        lineColorCbuf.SetToPixelShader(*this, 0u);
    }

    m_dx.Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    m_dx.Context->DrawIndexed(surface.GetIndexCount(), 0u, 0u);
}
