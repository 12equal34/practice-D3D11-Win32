#include "Renderer.h"

#include <sstream>
#include <DirectXMath.h>
#include "WinExceptionHelper.h"
#include "DXResource.h"
#include "Surface.h"
#include "DirectionalLight.h"

#ifndef NDEBUG
#include "DXExceptionMacro.h"
#endif

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace HDX = Hardware::DX;
using namespace DirectX;

HDX::Renderer::Renderer(HWND hwnd)
{
#ifndef NDEBUG
    Hardware::DX::DxgiInfoManager::Initialize();
#endif

    ThrowIfNull(GetClientRect(hwnd, &DXResource::GetClientRectangle()));

    DXResource::Initialize(hwnd);

    // bind depth stencil view to OM
    DXResource::GetContext()->OMSetRenderTargets(
        1u, DXResource::GetRTV().GetAddressOf(), DXResource::GetDSV().Get());
}
void HDX::Renderer::EndFrame()
{
    ThrowIfFailed(DXResource::GetSwapchain()->Present(0u, 0u));
}
void HDX::Renderer::ClearBuffer(float r, float g, float b) noexcept
{
    const float color[] = {r, g, b, 1.0f};
    DXResource::GetContext()->ClearRenderTargetView(DXResource::GetRTV().Get(),
                                                    color);
    DXResource::GetContext()->ClearDepthStencilView(
        DXResource::GetDSV().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void HDX::Renderer::DrawTestSurface()
{
    World::Object::Surface surface { 150, 150 };
    surface.Bind();

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
            {0.0f,   0.56f,  0.2f,  0.2f },
            {-0.33f, 0.0f,   0.12f, 0.3f },
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
        ConstantBuffer waveParameterCbuf(byteWidth, waves.data());
        waveParameterCbuf.SetToVertexShader(2u);
    }

    World::Object::DirectionalLight directionalLight;
    directionalLight.SetLightColor(0.6f, 0.7f, 0.7f, 1.0f);
    directionalLight.GetCoordinate().SetOrientation({-XM_PIDIV2, 0.0f, 0.0f});
    directionalLight.Bind();

    DXResource::GetContext()->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DXResource::GetContext()->DrawIndexed(surface.GetIndexCount(), 0u, 0u);
}
