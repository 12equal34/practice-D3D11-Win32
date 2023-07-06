#pragma once
#include <DirectXMath.h>

namespace gp::math
{

struct vector3D {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        DirectX::XMFLOAT3 r;
    };

    vector3D() noexcept = default;

    vector3D(const vector3D&) noexcept            = default;
    vector3D& operator=(const vector3D&) noexcept = default;

    vector3D(vector3D&&) noexcept            = default;
    vector3D& operator=(vector3D&&) noexcept = default;

    constexpr vector3D(float _x, float _y, float _z) noexcept
        : r {_x, _y, _z}
    { }
    vector3D(DirectX::FXMVECTOR _v) noexcept { DirectX::XMStoreFloat3(&r, _v); }

    operator DirectX::XMFLOAT3() const noexcept { return {x, y, z}; }

    template <class T>
    vector3D& operator+=(const T& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    template <class T>
    vector3D& operator-=(const T& rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
    template <class T>
    vector3D operator+(const T& rhs) const noexcept
    {
        auto result   = *this;
        return result += rhs;
    }
    template <class T>
    vector3D operator-(const T& rhs) const noexcept
    {
        auto result   = *this;
        return result -= rhs;
    }

    vector3D operator-() const noexcept { return vector3D {-x, -y, -z}; }
};
}
