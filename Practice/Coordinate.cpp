#include "Coordinate.h"
#include <algorithm>
#include "numeric_utility_functions.h"

using namespace DirectX;
namespace woi = World::Object::Information;

using Vector3D = woi::Coordinate::Vector3D;

woi::Coordinate::Coordinate() noexcept
    : Coordinate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)
{ }

woi::Coordinate::Coordinate(float x, float y, float z) noexcept
    : Coordinate(x, y, z, 0.0f, 0.0f, 0.0f)
{ }

woi::Coordinate::Coordinate(float x, float y, float z, float pitch, float yaw,
                            float roll) noexcept
    : m_pos(x, y, z),
      m_x(m_pos.x),
      m_y(m_pos.y),
      m_z(m_pos.z),
      m_ori(pitch, yaw, roll),
      m_pitch(m_ori.x),
      m_yaw(m_ori.y),
      m_roll(m_ori.z)
{ }

void woi::Coordinate::Translate(const Vector3D& dpos) noexcept
{
    m_pos += dpos;
}
void woi::Coordinate::SetPosition(const Vector3D& pos) noexcept { m_pos = pos; }
void woi::Coordinate::SetPositionX(float x) noexcept { m_x = x; }
void woi::Coordinate::SetPositionY(float y) noexcept { m_y = y; }
void woi::Coordinate::SetPositionZ(float z) noexcept { m_z = z; }

void woi::Coordinate::Rotate(const Vector3D& dori) noexcept
{
    gp::math::add_or_leave(m_pitch, dori.x, -XM_PIDIV2, XM_PIDIV2);

    m_yaw += dori.y;
    gp::math::mod_2PI(m_yaw);

    m_roll += dori.z;
    gp::math::mod_2PI(m_roll);
}
void woi::Coordinate::SetOrientation(const Vector3D& ori) noexcept
{
    m_ori = ori;
}
void woi::Coordinate::SetOrientationX(float pitch) noexcept { m_pitch = pitch; }
void woi::Coordinate::SetOrientationY(float yaw) noexcept { m_yaw = yaw; }
void woi::Coordinate::SetOrientationZ(float roll) noexcept { m_roll = roll; }

void woi::Coordinate::LocalTranslate(int index, float scale) noexcept
{
    const auto dir          = LocalDirection(index);
    const auto displacement = XMVectorScale(dir, scale);

    XMFLOAT3 dpos;
    XMStoreFloat3(&dpos, displacement);
    m_pos += dpos;
}

DirectX::XMVECTOR woi::Coordinate::LocalDirection(int index) const noexcept
{
    const auto matrix = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll);
    return matrix.r[index];
}

void woi::Coordinate::GoRight(float x) noexcept { LocalTranslate(0, x); }
void woi::Coordinate::GoLeft(float minus_x) noexcept { GoRight(-minus_x); }
void woi::Coordinate::GoUp(float y) noexcept { LocalTranslate(1, y); }
void woi::Coordinate::GoDown(float minus_y) noexcept { GoUp(-minus_y); }
void woi::Coordinate::GoForward(float z) noexcept { LocalTranslate(2, z); }
void woi::Coordinate::GoBackward(float minus_z) noexcept
{
    GoForward(-minus_z);
}

Vector3D& woi::Coordinate::GetPosition() noexcept
{
    return const_cast<Vector3D&>(
        static_cast<const woi::Coordinate*>(this)->GetPosition());
}
const Vector3D& woi::Coordinate::GetPosition() const noexcept { return m_pos; }
float           woi::Coordinate::GetPositionX() const noexcept { return m_x; }
float           woi::Coordinate::GetPositionY() const noexcept { return m_y; }
float           woi::Coordinate::GetPositionZ() const noexcept { return m_z; }

Vector3D& woi::Coordinate::GetOrientation() noexcept
{
    return const_cast<Vector3D&>(
        static_cast<const woi::Coordinate*>(this)->GetOrientation());
}
const Vector3D& woi::Coordinate::GetOrientation() const noexcept
{
    return m_ori;
}
float woi::Coordinate::GetOrientationX() const noexcept { return m_pitch; }
float woi::Coordinate::GetOrientationY() const noexcept { return m_yaw; }
float woi::Coordinate::GetOrientationZ() const noexcept { return m_roll; }

Vector3D woi::Coordinate::DirRight() const noexcept
{
    return Vector3D {LocalDirection(0)};
}
Vector3D woi::Coordinate::DirLeft() const noexcept
{
    return Vector3D {-LocalDirection(0)};
}
Vector3D woi::Coordinate::DirUp() const noexcept
{
    return Vector3D {LocalDirection(1)};
}
Vector3D woi::Coordinate::DirDown() const noexcept
{
    return Vector3D {-LocalDirection(1)};
}
Vector3D woi::Coordinate::DirForward() const noexcept
{
    return Vector3D {LocalDirection(2)};
}
Vector3D woi::Coordinate::DirBackward() const noexcept
{
    return Vector3D {-LocalDirection(2)};
}

XMMATRIX woi::Coordinate::GetModelRotation() const noexcept
{
    return XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll);
}

XMMATRIX
woi::Coordinate::GetModelTranslation() const noexcept
{
    return XMMatrixTranslation(m_x, m_y, m_z);
}

XMMATRIX
woi::Coordinate::GetModelMatrix() const noexcept
{
    return GetModelRotation() * GetModelTranslation();
}
