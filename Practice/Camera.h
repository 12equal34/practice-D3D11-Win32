#pragma once
#include <DirectXMath.h>

class Camera
{
public:
    Camera(float viewAspectRatio = 1.0f);

    void SetPosition(DirectX::XMFLOAT3 position) noexcept;
    void SetRotation(float pitch, float yaw, float roll) noexcept;
    void Translate(DirectX::XMFLOAT3 dposition) noexcept;
    void Rotate(float dpitch, float dyaw, float droll) noexcept;

    DirectX::XMFLOAT3 GetPosition() const noexcept;
    DirectX::XMFLOAT3 GetOrientation() const noexcept;
    DirectX::XMMATRIX GetView() const noexcept;
    DirectX::XMMATRIX GetProjection() const noexcept;
private:
    DirectX::XMFLOAT3 m_position;

    // clipping
    float m_viewAspectRatio;

    // orientation
    float m_pitch = 0.0f; // x-axis
    float m_yaw   = 0.0f; // y-axis
    float m_roll  = 0.0f; // z-axis
};
