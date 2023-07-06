#include "Camera.h"

using namespace World::Object;

Camera::Camera(float viewAspectRatio)
    : m_viewAspectRatio(viewAspectRatio)
{ }

DirectX::XMMATRIX Camera::GetView() const noexcept
{
    using namespace DirectX;

    auto forwardDir = m_coordinate.DirForward();

    XMVECTOR eyePos = XMLoadFloat3(&m_coordinate.GetPosition().r);
    XMVECTOR eyeDir = XMLoadFloat3(&forwardDir.r);
    XMVECTOR headUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    return XMMatrixLookToLH(eyePos, eyeDir, headUp);
}

DirectX::XMMATRIX World::Object::Camera::GetProjection() const noexcept
{
    return DirectX::XMMatrixPerspectiveLH(1.0f, m_viewAspectRatio, 0.5f,
                                          100.0f);
}
