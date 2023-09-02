#include "Coordinate.h"
#include <algorithm>

using namespace DirectX;
namespace woi = World::Object::Information;

using Point3D       = woi::Coordinate::Point3D;
using Direction3D   = woi::Coordinate::Direction3D;
using Orientation3D = woi::Coordinate::Orientation3D;

woi::Coordinate::Coordinate() noexcept
    : m_modelMatrix {
          // clang-format off
          1.f, 0.f, 0.f, 0.f, 
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1.f, 0.f, 
          0.f, 0.f, 0.f, 1.f,
          // clang-format on
      }
{ }

woi::Coordinate::Coordinate(float x, float y, float z) noexcept
    : m_modelMatrix {
          // clang-format off
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
          x,   y,   z, 1.f,
          // clang-format on
      }
{ }

woi::Coordinate::Coordinate(float x, float y, float z, float pitch, float yaw,
                            float roll) noexcept
    : Coordinate(x, y, z)
{
    SetOrientation(pitch, yaw, roll);
}

woi::Coordinate::Coordinate(const Point3D&       pos,
                            const Orientation3D& ori) noexcept
    : Coordinate(pos.x, pos.y, pos.z, ori.x, ori.y, ori.z)
{ }

void woi::Coordinate::SetPosition(const Point3D& pos) noexcept { m_pos = pos; }
void World::Object::Information::Coordinate::SetPosition(float x, float y,
                                                         float z) noexcept
{
    m_pos.x = x;
    m_pos.y = y;
    m_pos.z = z;
}
void woi::Coordinate::SetPositionX(float x) noexcept { m_pos.x = x; }
void woi::Coordinate::SetPositionY(float y) noexcept { m_pos.y = y; }
void woi::Coordinate::SetPositionZ(float z) noexcept { m_pos.z = z; }

void woi::Coordinate::Translate(const Direction3D& dpos) noexcept
{
    m_pos += dpos;
}

void woi::Coordinate::Rotate(DirectX::FXMVECTOR eulerAngles) noexcept
{
    auto newModelMatrix = XMLoadFloat4x4(&m_modelMatrix);
    newModelMatrix      *= XMMatrixRotationRollPitchYawFromVector(eulerAngles);

    XMStoreFloat4x4(&m_modelMatrix, newModelMatrix);
}

void woi::Coordinate::Rotate(const Orientation3D& ori) noexcept
{
    Rotate(ori.Load());
}

void woi::Coordinate::Rotate(float pitch, float yaw, float roll) noexcept
{
    Rotate(Orientation3D {pitch, yaw, roll});
}

void woi::Coordinate::RotateAbout(const Direction3D& axis, float angle) noexcept
{
    auto newModelMatrix = XMLoadFloat4x4(&m_modelMatrix);
    newModelMatrix      *= XMMatrixRotationAxis(axis.Load(), angle);

    XMStoreFloat4x4(&m_modelMatrix, newModelMatrix);
}

void woi::Coordinate::SetOrientation(DirectX::FXMVECTOR eulerAngles) noexcept
{
    auto newModelMatrix = XMMatrixRotationRollPitchYawFromVector(eulerAngles) *
                          GetTranslationMatrix();

    XMStoreFloat4x4(&m_modelMatrix, newModelMatrix);
}

void woi::Coordinate::SetOrientation(const Orientation3D& ori) noexcept
{
    SetOrientation(ori.Load());
}

void woi::Coordinate::SetOrientation(float pitch, float yaw,
                                     float roll) noexcept
{
    auto newModelMatrix =
        XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * GetTranslationMatrix();
    XMStoreFloat4x4(&m_modelMatrix, newModelMatrix);
}

void woi::Coordinate::GoRight(float dx) noexcept { m_pos += DirRight() * dx; }
void woi::Coordinate::GoLeft(float dx) noexcept { GoRight(-dx); }
void woi::Coordinate::GoUp(float dy) noexcept { m_pos += DirUp() * dy; }
void woi::Coordinate::GoDown(float dy) noexcept { GoUp(-dy); }
void woi::Coordinate::GoForward(float dz) noexcept
{
    m_pos += DirForward() * dz;
}
void woi::Coordinate::GoBackward(float dz) noexcept { GoForward(-dz); }

Point3D& woi::Coordinate::GetPosition() noexcept
{
    return const_cast<Point3D&>(
        static_cast<const woi::Coordinate*>(this)->GetPosition());
}
const Point3D& woi::Coordinate::GetPosition() const noexcept { return m_pos; }
float woi::Coordinate::GetPositionX() const noexcept { return m_pos.x; }
float woi::Coordinate::GetPositionY() const noexcept { return m_pos.y; }
float woi::Coordinate::GetPositionZ() const noexcept { return m_pos.z; }

Direction3D woi::Coordinate::DirRight() const noexcept { return m_right; }
Direction3D woi::Coordinate::DirUp() const noexcept { return m_up; }
Direction3D woi::Coordinate::DirForward() const noexcept { return m_forward; }
Direction3D woi::Coordinate::DirLeft() const noexcept { return -DirRight(); }
Direction3D woi::Coordinate::DirDown() const noexcept { return -DirUp(); }
Direction3D woi::Coordinate::DirBackward() const noexcept
{
    return -DirForward();
}

XMMATRIX woi::Coordinate::GetRotationMatrix() const noexcept
{
    XMFLOAT3X3 rotationMatrix {
        m_modelMatrix._11, m_modelMatrix._12, m_modelMatrix._13,
        m_modelMatrix._21, m_modelMatrix._22, m_modelMatrix._23,
        m_modelMatrix._31, m_modelMatrix._32, m_modelMatrix._33,
    };
    return XMLoadFloat3x3(&rotationMatrix);
}

XMMATRIX
woi::Coordinate::GetTranslationMatrix() const noexcept
{
    return XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

XMMATRIX
woi::Coordinate::GetModelMatrix() const noexcept
{
    return XMLoadFloat4x4(&m_modelMatrix);
}
