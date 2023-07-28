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
        using Point3D = gp::math::Point3D;
        using Direction3D = gp::math::Direction3D;

        Coordinate() noexcept;
        Coordinate(float x, float y, float z) noexcept;
        Coordinate(float x, float y, float z, float pitch, float yaw,
                   float roll) noexcept;

        void Translate(const Direction3D& dpos) noexcept;
        void SetPosition(const Point3D& pos) noexcept;
        void SetPositionX(float x) noexcept;
        void SetPositionY(float y) noexcept;
        void SetPositionZ(float z) noexcept;

        void Rotate(const Point3D& dori) noexcept;
        void SetOrientation(const Point3D& ori) noexcept;
        void SetOrientationX(float x) noexcept;
        void SetOrientationY(float y) noexcept;
        void SetOrientationZ(float z) noexcept;

        void GoRight(float x) noexcept;
        void GoLeft(float minus_x) noexcept;
        void GoUp(float y) noexcept;
        void GoDown(float minus_y) noexcept;
        void GoForward(float z) noexcept;
        void GoBackward(float minus_z) noexcept;

        Point3D&       GetPosition() noexcept;
        const Point3D& GetPosition() const noexcept;
        float           GetPositionX() const noexcept;
        float           GetPositionY() const noexcept;
        float           GetPositionZ() const noexcept;

        Point3D&       GetOrientation() noexcept;
        const Point3D& GetOrientation() const noexcept;
        float           GetOrientationX() const noexcept;
        float           GetOrientationY() const noexcept;
        float           GetOrientationZ() const noexcept;

        Direction3D DirRight() const noexcept;
        Direction3D DirLeft() const noexcept;
        Direction3D DirUp() const noexcept;
        Direction3D DirDown() const noexcept;
        Direction3D DirForward() const noexcept;
        Direction3D DirBackward() const noexcept;

        DirectX::XMMATRIX GetModelRotation() const noexcept;
        DirectX::XMMATRIX GetModelTranslation() const noexcept;
        DirectX::XMMATRIX GetModelMatrix() const noexcept;
    private:
        void LocalTranslate(int index, float scale) noexcept;
        DirectX::XMVECTOR LocalDirection(int index) const noexcept;

        // global position
        Point3D m_pos;
        float&   m_x;
        float&   m_y;
        float&   m_z;

        // orientation
        Point3D m_ori;
        float&   m_pitch; // around the x-axis
        float&   m_yaw;   // around the y-axis
        float&   m_roll;  // around the z-axis
    };
}
}
