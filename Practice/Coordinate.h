#pragma once
#include <DirectXMath.h>
#include "vector3D.h"

namespace World
{
namespace Object::Information
{
    class Coordinate
    {
    public:
        using vector3D = gp::math::vector3D;

        Coordinate() noexcept;
        Coordinate(float x, float y, float z) noexcept;
        Coordinate(float x, float y, float z, float pitch, float yaw,
                   float roll) noexcept;

        void Translate(const vector3D& dpos) noexcept;
        void SetPosition(const vector3D& pos) noexcept;
        void SetPositionX(float x) noexcept;
        void SetPositionY(float y) noexcept;
        void SetPositionZ(float z) noexcept;

        void Rotate(const vector3D& dori) noexcept;
        void SetOrientation(const vector3D& ori) noexcept;
        void SetOrientationX(float x) noexcept;
        void SetOrientationY(float y) noexcept;
        void SetOrientationZ(float z) noexcept;

        void GoRight(float x) noexcept;
        void GoLeft(float minus_x) noexcept;
        void GoUp(float y) noexcept;
        void GoDown(float minus_y) noexcept;
        void GoForward(float z) noexcept;
        void GoBackward(float minus_z) noexcept;

        vector3D&       GetPosition() noexcept;
        const vector3D& GetPosition() const noexcept;
        float           GetPositionX() const noexcept;
        float           GetPositionY() const noexcept;
        float           GetPositionZ() const noexcept;

        vector3D&       GetOrientation() noexcept;
        const vector3D& GetOrientation() const noexcept;
        float           GetOrientationX() const noexcept;
        float           GetOrientationY() const noexcept;
        float           GetOrientationZ() const noexcept;

        vector3D DirRight() const noexcept;
        vector3D DirLeft() const noexcept;
        vector3D DirUp() const noexcept;
        vector3D DirDown() const noexcept;
        vector3D DirForward() const noexcept;
        vector3D DirBackward() const noexcept;

        DirectX::XMMATRIX GetRotation() const noexcept;
    
    private:
        void LocalTranslate(int index, float scale) noexcept;
        DirectX::XMVECTOR LocalDirection(int index) const noexcept;

        // global position
        vector3D m_pos;
        float&   m_x;
        float&   m_y;
        float&   m_z;

        // orientation
        vector3D m_ori;
        float&   m_pitch; // around the x-axis
        float&   m_yaw;   // around the y-axis
        float&   m_roll;  // around the z-axis
    };
}
}
