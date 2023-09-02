#pragma once
#include "DXResource.h"
#include <DirectXMath.h>
#include "Coordinatable.h"
#include "IBindable.h"
#include "Keyboard.h"

namespace World::Object
{
class Camera : public Hardware::DX::IBindable, public Information::Coordinatable
{
public:
    Camera(float viewAspectRatio = 1.0f) noexcept;
    DirectX::XMMATRIX GetView() const noexcept;
    DirectX::XMMATRIX GetProjection() const noexcept;

    void SetNearZ(float zValue) noexcept;
    void SetFarZ(float zValue) noexcept;
    void SetNearFarZ(float nearZ, float farZ) noexcept;

    float GetPitch() const noexcept;
    float GetYaw() const noexcept;
    bool  IsBoundMouseDeltaInput() const noexcept;

    void SetViewport(FLOAT width, FLOAT height) noexcept;
    void SetPitchYaw(float pitch, float yaw) noexcept;
    void SetMoveSpeed(float moveSpeed) noexcept;
    void SetRotationSpeed(float rotationSpeed) noexcept;
    void SetBindingMouseDeltaInput(bool canMouseDeltaInput = true) noexcept;

    void HandleInputFromKeyboard(const Hardware::Keyboard& keyboard,
                                 float                     dt) noexcept;
    void HandleInputFromMouseMovement(int deltaMouseX,
                                      int deltaMouseY) noexcept;

    void Bind() override;

private:
    D3D11_VIEWPORT m_viewport;
    float          m_viewAspectRatio;
    float          m_nearZ = 0.5f;
    float          m_farZ  = 100.0f;

    float m_pitch = 0.f;
    float m_yaw   = 0.f;

    float m_moveSpeed     = 10.0f;
    float m_rotationSpeed = 10.0f;

    bool m_isBoundMouseDeltaInput = true;
};
}
