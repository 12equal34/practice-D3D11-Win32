#include "Renderer.h"

#include <sstream>
#include <DirectXMath.h>
#include "DXResource.h"
#include "WinExceptionHelper.h"

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