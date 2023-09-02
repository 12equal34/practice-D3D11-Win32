#pragma once
#include <DirectXMath.h>
#include "AffineSpaceTools.h"

namespace World
{
namespace Object::Information
{
    class Coordinate
    {
    public:
        using Point3D       = gp::math::Point3D;
        using Direction3D   = gp::math::Direction3D;
        using Orientation3D = gp::math::Orientation3D;

        Coordinate() noexcept;
        Coordinate(float x, float y, float z) noexcept;
        Coordinate(float x, float y, float z, float pitch, float yaw,
                   float roll) noexcept;
        Coordinate(const Point3D&       pos,
                   const Orientation3D& ori = Orientation3D {}) noexcept;

        void SetPosition(const Point3D& pos) noexcept;
        void SetPosition(float x, float y, float z) noexcept;
        void SetPositionX(float x) noexcept;
        void SetPositionY(float y) noexcept;
        void SetPositionZ(float z) noexcept;

        void SetOrientation(DirectX::FXMVECTOR eulerAngles) noexcept;
        void SetOrientation(const Orientation3D& ori) noexcept;
        void SetOrientation(float pitch, float yaw, float roll) noexcept;

        void Translate(const Direction3D& dpos) noexcept;

        void Rotate(DirectX::FXMVECTOR eulerAngles) noexcept;
        void Rotate(const Orientation3D& ori) noexcept;
        void Rotate(float pitch, float yaw, float roll) noexcept;
        void RotateAbout(const Direction3D& axis, float angle) noexcept;

        void GoRight(float dx) noexcept;
        void GoLeft(float dx) noexcept;
        void GoUp(float dy) noexcept;
        void GoDown(float dy) noexcept;
        void GoForward(float dz) noexcept;
        void GoBackward(float dz) noexcept;

        Point3D&       GetPosition() noexcept;
        const Point3D& GetPosition() const noexcept;
        float          GetPositionX() const noexcept;
        float          GetPositionY() const noexcept;
        float          GetPositionZ() const noexcept;

        Direction3D DirRight() const noexcept;
        Direction3D DirLeft() const noexcept;
        Direction3D DirUp() const noexcept;
        Direction3D DirDown() const noexcept;
        Direction3D DirForward() const noexcept;
        Direction3D DirBackward() const noexcept;

        DirectX::XMMATRIX GetRotationMatrix() const noexcept;
        DirectX::XMMATRIX GetTranslationMatrix() const noexcept;
        DirectX::XMMATRIX GetModelMatrix() const noexcept;
    private:
        union {
            DirectX::XMFLOAT4X4 m_modelMatrix;
            struct {
                // row 1
                Direction3D m_right;
                float       m_14;

                // row 2
                Direction3D m_up;
                float       m_24;

                // row 3
                Direction3D m_forward;
                float       m_34;

                // row 4
                Point3D m_pos;
                float   m_invScale;
            };
        };
    };
}
}
