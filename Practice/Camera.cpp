#include "Camera.h"

using namespace Hardware::DX;
using namespace DirectX;
namespace WO = World::Object;

WO::Camera::Camera(float viewAspectRatio) noexcept
    : m_viewAspectRatio(viewAspectRatio),
      m_cbuf(sizeof(XMFLOAT4X4))
{
    const auto& cr = DXResource::GetClientRectangle();
    SetViewport(static_cast<FLOAT>(cr.right), static_cast<FLOAT>(cr.bottom));
}

XMMATRIX WO::Camera::GetView() const noexcept
{
    auto forwardDir = m_coordinate.DirForward();

    XMVECTOR eyePos = XMLoadFloat3(&m_coordinate.GetPosition().r);
    XMVECTOR eyeDir = XMLoadFloat3(&forwardDir.r);
    XMVECTOR headUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    return XMMatrixLookToLH(eyePos, eyeDir, headUp);
}

XMMATRIX WO::Camera::GetProjection() const noexcept
{
    return XMMatrixPerspectiveLH(1.0f, m_viewAspectRatio, 0.5f, 100.0f);
}

void WO::Camera::SetViewport(FLOAT width, FLOAT height) noexcept
{
    // configure a viewport
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_viewport.Width    = width;
    m_viewport.Height   = height;
    m_viewport.MinDepth = 0;
    m_viewport.MaxDepth = 1;
}

void WO::Camera::Bind() noexcept
{
    DXResource::GetContext()->RSSetViewports(1u, &m_viewport);

    const auto cameraRotation = XMMatrixTranspose(m_coordinate.GetModelRotation());
    m_cbuf.Update(&cameraRotation);
    m_cbuf.SetToVertexShader(1u);
}
