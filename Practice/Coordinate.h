#pragma once
#include <DirectXMath.h>
#include "Vector3D.h"

namespace World
{
namespace Object::Information
{
    class Coordinate
    {
    public:
        using Vector3D = gp::math::Vector3D;

        Coordinate() noexcept;
        Coordinate(float x, float y, float z) noexcept;
        Coordinate(float x, float y, float z, float pitch, float yaw,
                   float roll) noexcept;

        void Translate(const Vector3D& dpos) noexcept;
        void SetPosition(const Vector3D& pos) noexcept;
        void SetPositionX(float x) noexcept;
        void SetPositionY(float y) noexcept;
        void SetPositionZ(float z) noexcept;

        void Rotate(const Vector3D& dori) noexcept;
        void SetOrientation(const Vector3D& ori) noexcept;
        void SetOrientationX(float x) noexcept;
        void SetOrientationY(float y) noexcept;
        void SetOrientationZ(float z) noexcept;

        void GoRight(float x) noexcept;
        void GoLeft(float minus_x) noexcept;
        void GoUp(float y) noexcept;
        void GoDown(float minus_y) noexcept;
        void GoForward(float z) noexcept;
        void GoBackward(float minus_z) noexcept;

        Vector3D&       GetPosition() noexcept;
        const Vector3D& GetPosition() const noexcept;
        float           GetPositionX() const noexcept;
        float           GetPositionY() const noexcept;
        float           GetPositionZ() const noexcept;

        Vector3D&       GetOrientation() noexcept;
        const Vector3D& GetOrientation() const noexcept;
        float           GetOrientationX() const noexcept;
        float           GetOrientationY() const noexcept;
        float           GetOrientationZ() const noexcept;

        Vector3D DirRight() const noexcept;
        Vector3D DirLeft() const noexcept;
        Vector3D DirUp() const noexcept;
        Vector3D DirDown() const noexcept;
        Vector3D DirForward() const noexcept;
        Vector3D DirBackward() const noexcept;

        DirectX::XMMATRIX GetModelRotation() const noexcept;
        DirectX::XMMATRIX GetModelTranslation() const noexcept;
        DirectX::XMMATRIX GetModelMatrix() const noexcept;
    private:
        void LocalTranslate(int index, float scale) noexcept;
        DirectX::XMVECTOR LocalDirection(int index) const noexcept;

        // global position
        Vector3D m_pos;
        float&   m_x;
        float&   m_y;
        float&   m_z;

        // orientation
        Vector3D m_ori;
        float&   m_pitch; // around the x-axis
        float&   m_yaw;   // around the y-axis
        float&   m_roll;  // around the z-axis
    };
}
}
