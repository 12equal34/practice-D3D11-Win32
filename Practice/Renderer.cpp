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

void Hardware::DX::Renderer::DrawTestSurface(const Camera& camera, float x,
                                             float z, float time)
{
    constexpr float pi = 3.14159265f;
    Surface         surface(*this, 150, 150);
    surface.Bind(*this);

    auto pViewport = std::make_unique<Viewport>(
        *this, static_cast<FLOAT>(m_dx.ClientRect.right),
        static_cast<FLOAT>(m_dx.ClientRect.bottom));
    pViewport->Bind(*this);

    // constant buffer for transform in VS
    {
        struct Transform {
            XMMATRIX model;
            XMMATRIX modelView;
            XMMATRIX modelViewProj;
            XMMATRIX modelRotation;
            XMMATRIX cameraRotation;
        };

        XMMATRIX    model         = XMMatrixIdentity();
        XMMATRIX    view          = camera.GetView();
        XMMATRIX    proj          = camera.GetProjection();
        XMMATRIX    modelRotation = XMMatrixIdentity();
        const auto& camOri        = camera.GetOrientation();
        XMMATRIX    cameraRotation =
            XMMatrixRotationRollPitchYaw(camOri.x, camOri.y, camOri.z);

        const Transform transBufData = {
            XMMatrixTranspose(model),
            XMMatrixTranspose(model * view),
            XMMatrixTranspose(model * view * proj),
            XMMatrixTranspose(modelRotation),
            cameraRotation,
        };

        ConstantBuffer transformCbuf(*this, sizeof(transBufData),
                                     &transBufData);
        transformCbuf.SetToVertexShader(*this, 0u);
    }

    // constant buffer for global parameters in VS
    {
        struct GlobalCbuf {
            float time;
            float _1;
            float _2;
            float _3;
        };
        const GlobalCbuf globalCbufData {time, 0, 0, 0};

        ConstantBuffer globalCbuf(*this, sizeof(globalCbufData),
                                  &globalCbufData);
        globalCbuf.SetToVertexShader(*this, 1u);
    }

    // constant buffer for wave parameters in VS
    {
        struct Wave {
            float wave_number_x;
            float wave_number_z;
            float wave_number;
            float wave_angular_frequency;

            float wave_amplitude;
            float wave_phase;
            float _1, _2;
        };

        std::vector<std::tuple<float, float, float, float>> init {
            {0.3f,   -0.46f, 0.2f,  0.0f },
            {-0.33f, 0.40f,  0.12f, 0.0f },
            {0.0f,   0.56f, 0.2f,  0.2f },
            {-0.33f, 0.0f,  0.12f, 0.3f },
            {0.3f,   -0.26f, 0.2f,  0.7f },
            {-0.33f, 0.20f,  0.12f, 0.9f },
            {0.17f,  0.3f,   0.3f,  -1.0f},
            {-0.15f, -0.1f,  0.7f,  0.6f },
            {-0.15f, 0.1f,   0.6f,  0.5f },
            {0.15f,  -0.1f,  0.8f,  0.4f },
            {0.15f,  0.1f,   0.3f,  0.3f },
            {-0.25f, -0.1f,  0.2f,  0.2f },
            {-0.15f, -0.25f, 0.4f,  0.1f },
        };

        std::vector<Wave> waves;
        waves.reserve(init.size());
        for (auto [kx, kz, a, phi] : init) {
            auto k = std::sqrtf(kx * kx + kz * kz);
            auto w = std::sqrtf(9.8f * k);
            waves.push_back(Wave(kx, kz, k, w, a, phi, 0, 0));
        }

        auto byteWidth = static_cast<UINT>(sizeof(Wave) * std::size(waves));
        ConstantBuffer waveParameterCbuf(*this, byteWidth, waves.data());
        waveParameterCbuf.SetToVertexShader(*this, 2u);
    }

    // constant buffer for face color in PS
    {
        struct Light {
            XMFLOAT4 light_ambient;
            XMFLOAT4 light_color;
            XMFLOAT3 light_direction;
            float    _1;
        };

        XMVECTOR lightDir =
            XMVector4Normalize(XMVectorSet(0.3f, 1.0f, 0.2f, 0.0f));
        XMFLOAT3 light_direction {};
        XMStoreFloat3(&light_direction, lightDir);
        const Light lightCbufData = {
            {0.3f, 0.2f, 0.3f, 1.0f},
            {1.0f, 0.8f, 0.8f, 1.0f},
            light_direction,
            0.0f
        };

        ConstantBuffer lightCbuf(*this, sizeof(lightCbufData), &lightCbufData);
        lightCbuf.SetToPixelShader(*this, 0u);
    }

    m_dx.Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_dx.Context->DrawIndexed(surface.GetIndexCount(), 0u, 0u);
}
