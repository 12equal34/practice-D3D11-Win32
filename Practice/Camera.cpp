#include "Camera.h"

Camera::Camera(float viewAspectRatio)
    : m_position(),
      m_viewAspectRatio(viewAspectRatio)
{ }

void Camera::SetPosition(DirectX::XMFLOAT3 position) noexcept
{
    m_position = position;
}

void Camera::SetRotation(float pitch, float yaw, float roll) noexcept
{
    m_pitch = pitch;
    m_yaw   = yaw;
    m_roll  = roll;
}

void Camera::Translate(DirectX::XMFLOAT3 dposition) noexcept
{
    using namespace DirectX;
    const auto dpos = XMVector3Transform(
        XMLoadFloat3(&dposition),
        XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll));
    const auto pos  = XMLoadFloat3(&m_position);
    XMStoreFloat3(&m_position, pos + dpos);
}

void Camera::Rotate(float dpitch, float dyaw, float droll) noexcept
{
    using namespace DirectX;
    static auto ParallelTransform = [](float& theta, float period) {
        if (theta >= period) {
            theta -= period;
        } else if (theta < 0) {
            theta += period;
        }
    };

    m_pitch += dpitch;
    m_yaw   += dyaw;
    m_roll  += droll;

    if (m_pitch > XM_PIDIV2 || m_pitch < -XM_PIDIV2) {
        m_pitch -= dpitch;
    }
    ParallelTransform(m_yaw, XM_2PI);
    ParallelTransform(m_roll, XM_2PI);
}

DirectX::XMFLOAT3 Camera::GetPosition() const noexcept { return m_position; }

DirectX::XMFLOAT3 Camera::GetOrientation() const noexcept
{
    return DirectX::XMFLOAT3(m_pitch, m_yaw, m_roll);
}

DirectX::XMMATRIX Camera::GetView() const noexcept
{
    using namespace DirectX;
    const auto look = XMVector3Transform(
        XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
        XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll));
    const auto eyePos = XMLoadFloat3(&m_position);
    const auto eyeDir = eyePos + look;
    const auto up     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    return XMMatrixLookAtLH(eyePos, eyeDir, up);
}

DirectX::XMMATRIX Camera::GetProjection() const noexcept
{
    return DirectX::XMMatrixPerspectiveLH(1.0f, m_viewAspectRatio, 0.5f,
                                          100.0f);
}
