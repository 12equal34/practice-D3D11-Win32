#include "Renderer.h"

#include <sstream>
#include <DirectXMath.h>
#include "DXResource.h"
#include "WinExceptionHelper.h"
#include "ConstantBuffer.h"
#include "ConstantBuffers.h"

#ifndef NDEBUG
#include "DXExceptionMacro.h"
#endif

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace hdx = Hardware::DX;
using namespace DirectX;

hdx::Renderer::Renderer(HWND hwnd)
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
void hdx::Renderer::EndFrame()
{
    ThrowIfFailed(DXResource::GetSwapchain()->Present(0u, 0u));
}
void hdx::Renderer::ClearBuffer(float r, float g, float b) noexcept
{
    const float color[] = {r, g, b, 1.0f};
    DXResource::GetContext()->ClearRenderTargetView(DXResource::GetRTV().Get(),
                                                    color);
    DXResource::GetContext()->ClearDepthStencilView(
        DXResource::GetDSV().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void hdx::Renderer::DrawObjects(
    const World::Object::Camera&                               camera,
    const std::vector<std::unique_ptr<World::Object::Object>>& objects)
{
    XMMATRIX       proj[] = {XMMatrixTranspose(camera.GetProjection())};
    ConstantBuffer projCbuf(sizeof(proj), &proj);
    projCbuf.SetToVertexShader(1u);

    XMMATRIX       view[] = {XMMatrixTranspose(camera.GetView())};
    ConstantBuffer viewCbuf(sizeof(view), &view);
    viewCbuf.SetToVertexShader(2u);

    for (const auto& obj : objects) {
        XMFLOAT4X4 world;
        XMStoreFloat4x4(&world, XMMatrixTranspose(obj->GetModelMatrix()));

        ConstantBufferChangesEveryPrim worldCbufDatas = {
            world,
            {1.0f, 1.0f, 1.0f},
            {0.8f, 0.8f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            1.0f,
            1.0f,
        };

        ConstantBuffer worldCbuf(sizeof(worldCbufDatas), &worldCbufDatas);
        worldCbuf.SetToVertexShader(3u);
        worldCbuf.SetToPixelShader(3u);

        DXResource::GetContext()->DrawIndexed(obj->GetIndexCount(), 0u, 0u);
    }
}

void hdx::Renderer::DrawDynamicObjects(
    const World::Object::Camera&                                     camera,
    const std::vector<std::unique_ptr<World::Object::WaterSurface>>& objects)
{
    XMMATRIX       proj[] = {XMMatrixTranspose(camera.GetProjection())};
    ConstantBuffer projCbuf(sizeof(proj), &proj);
    projCbuf.SetToVertexShader(1u);

    XMMATRIX       view[] = {XMMatrixTranspose(camera.GetView())};
    ConstantBuffer viewCbuf(sizeof(view), &view);
    viewCbuf.SetToVertexShader(2u);

    for (const auto& obj : objects) {
        XMFLOAT4X4 world;
        XMStoreFloat4x4(&world, XMMatrixTranspose(obj->GetModelMatrix()));

        ConstantBufferChangesEveryPrim worldCbufDatas = {
            world,
            {1.0f, 1.0f, 1.0f},
            {0.8f, 0.8f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            1.0f,
            1.0f,
        };

        ConstantBuffer worldCbuf(sizeof(worldCbufDatas), &worldCbufDatas);
        worldCbuf.SetToVertexShader(3u);
        worldCbuf.SetToPixelShader(3u);

        DXResource::GetContext()->DrawIndexed(obj->GetIndexCount(), 0u, 0u);
    }
}

void hdx::Renderer::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
    m_topology = topology;
    DXResource::GetContext()->IASetPrimitiveTopology(m_topology);
}

D3D11_PRIMITIVE_TOPOLOGY
hdx::Renderer::GetPrimitiveTopology() const noexcept { return m_topology; }
