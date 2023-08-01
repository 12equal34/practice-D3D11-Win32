#include <string_view>
#include "Camera.h"
#include "numeric_utility_functions.h"

using namespace Hardware::DX;
using namespace DirectX;

World::Object::Camera::Camera(float viewAspectRatio) noexcept
    : m_viewAspectRatio(viewAspectRatio)
{
    const auto& cr = DXResource::GetClientRectangle();
    SetViewport(static_cast<FLOAT>(cr.right), static_cast<FLOAT>(cr.bottom));
}

XMMATRIX World::Object::Camera::GetView() const noexcept
{
    const auto eyeDir = m_coord.DirForward().Load();
    const auto eyePos = m_coord.GetPosition().Load();
    const auto headUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    return XMMatrixLookToLH(eyePos, eyeDir, headUp);
}

XMMATRIX World::Object::Camera::GetProjection() const noexcept
{
    return XMMatrixPerspectiveLH(1.0f, m_viewAspectRatio, 0.5f, 100.0f);
}

float World::Object::Camera::GetPitch() const noexcept { return m_pitch; }

float World::Object::Camera::GetYaw() const noexcept { return m_yaw; }

void World::Object::Camera::SetViewport(FLOAT width, FLOAT height) noexcept
{
    // configure a viewport
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_viewport.Width    = width;
    m_viewport.Height   = height;
    m_viewport.MinDepth = 0;
    m_viewport.MaxDepth = 1;
}

void World::Object::Camera::SetPitchYaw(float pitch, float yaw) noexcept
{
    gp::math::map_on_range(pitch, 0.0f, XM_2PI);
    if (XM_PI <= pitch) {
        yaw   += XM_PI;
        pitch -= XM_PI;
    }
    pitch -= XM_PIDIV2;

    gp::math::map_on_range(yaw, -XM_PI, XM_PI);

    m_pitch = pitch;
    m_yaw   = yaw;

    m_coord.SetOrientation(m_pitch, m_yaw, 0.0f);
}

void World::Object::Camera::SetMoveSpeed(float moveSpeed) noexcept
{
    m_moveSpeed = moveSpeed;
}

void World::Object::Camera::SetRotationSpeed(float rotationSpeed) noexcept
{
    m_rotationSpeed = rotationSpeed;
}

void World::Object::Camera::HandleInputFromKeyboard(
    const Hardware::Keyboard& keyboard, float dt) noexcept
{
    int forward = 0;
    int up      = 0;
    int right   = 0;

    // Forward-Back Key (WS)를 동시에 누르지 않았다면,
    if (!keyboard.KeyIsPressed(u8"WS")) {
        if (keyboard.KeyIsPressed('W')) forward = 1;
        if (keyboard.KeyIsPressed('S')) forward = -1;
    }

    // Right-Left Key (DA)를 동시에 누르지 않았다면,
    if (!keyboard.KeyIsPressed(u8"DA")) {
        if (keyboard.KeyIsPressed('D')) right = 1;
        if (keyboard.KeyIsPressed('A')) right = -1;
    }

    // Up-Down Key (EQ)를 동시에 누르지 않았다면,
    if (!keyboard.KeyIsPressed(u8"EQ")) {
        if (keyboard.KeyIsPressed('E')) up = 1;
        if (keyboard.KeyIsPressed('Q')) up = -1;
    }

    auto step = m_moveSpeed * dt;

    if (forward != 0 && up == 0 && right == 0) {
        m_coord.GoForward(step * forward);
    } else if (forward == 0 && up != 0 && right == 0) {
        m_coord.GoUp(step * up);
    } else if (forward == 0 && up == 0 && right != 0) {
        m_coord.GoRight(step * right);
    } else if (forward == 0 && up != 0 && right != 0) {
        step *= 0.70710678f;
        m_coord.GoUp(step * up);
        m_coord.GoRight(step * right);
    } else if (forward != 0 && up == 0 && right != 0) {
        step *= 0.70710678f;
        m_coord.GoForward(step * forward);
        m_coord.GoRight(step * right);
    } else if (forward != 0 && up != 0 && right == 0) {
        step *= 0.70710678f;
        m_coord.GoForward(step * forward);
        m_coord.GoUp(step * up);
    } else if (forward != 0 && up != 0 && right != 0) {
        step *= 0.57735026f;
        m_coord.GoForward(step * forward);
        m_coord.GoUp(step * up);
        m_coord.GoRight(step * right);
    }
}

void World::Object::Camera::HandleInputFromMouseMovement(
    int deltaMouseX, int deltaMouseY) noexcept
{
    const auto dyaw =
        m_rotationSpeed * deltaMouseX / DXResource::GetWindowWidth();
    const auto dpitch =
        m_rotationSpeed * deltaMouseY / DXResource::GetWindowHeight();

    m_pitch += dpitch;
    if (m_pitch < -XM_PIDIV2 || XM_PIDIV2 <= m_pitch) {
        m_pitch -= dpitch;
    }

    m_yaw += dyaw;
    if (m_yaw < -XM_PI) {
        m_yaw += XM_2PI;
    } else if (XM_PI <= m_yaw) {
        m_yaw -= XM_2PI;
    }

    m_coord.SetOrientation(m_pitch, m_yaw, 0.0f);
}

void World::Object::Camera::Bind() noexcept
{
    DXResource::GetContext()->RSSetViewports(1u, &m_viewport);
}
