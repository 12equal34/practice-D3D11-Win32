#pragma once
#include <DirectXMath.h>

namespace gp::math
{
struct Vector3D {
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;

    constexpr Vector3D() noexcept = default;

    constexpr Vector3D(const Vector3D&) noexcept            = default;
    constexpr Vector3D& operator=(const Vector3D&) noexcept = default;

    constexpr Vector3D(Vector3D&&) noexcept            = default;
    constexpr Vector3D& operator=(Vector3D&&) noexcept = default;

    constexpr Vector3D(float _x, float _y, float _z) noexcept
        : x(_x),
          y(_y),
          z(_z)
    { }

    template <class T>
    constexpr Vector3D& operator+=(const T& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    template <class T>
    constexpr Vector3D& operator-=(const T& rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
    template <class T>
    constexpr Vector3D& operator*=(const T& rhs) noexcept
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }
    template <class T>
    constexpr Vector3D& operator/=(const T& rhs) noexcept
    {
        // 가정: rhs의 모든 성분이 0.f가 아니다.
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }

    constexpr Vector3D& operator*=(float scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    constexpr Vector3D& operator/=(float scalar) noexcept
    {
        // 가정: scalar는 0.f가 아니다.
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    constexpr float Dot(const Vector3D& rhs) const noexcept
    {
        return Vector3D::Dot(*this, rhs);
    }

    constexpr Vector3D Cross(const Vector3D& rhs) const noexcept
    {
        return Vector3D::Cross(*this, rhs);
    }

    static constexpr float Dot(const Vector3D& a, const Vector3D& b) noexcept
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static constexpr Vector3D Cross(const Vector3D& a,
                                    const Vector3D& b) noexcept
    {
        return Vector3D {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                         a.x * b.y - a.y * b.x};
    }

    // for SIMD
    Vector3D(DirectX::FXMVECTOR v) noexcept
    {
        DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(this), v);
    }
    operator const DirectX::XMFLOAT3&() const noexcept
    {
        return reinterpret_cast<const DirectX::XMFLOAT3&>(*this);
    }
    operator DirectX::XMFLOAT3&() noexcept
    {
        return reinterpret_cast<DirectX::XMFLOAT3&>(*this);
    }
};

constexpr Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs) noexcept
{
    auto result {lhs};
    return result += rhs;
}

constexpr Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs) noexcept
{
    auto result {lhs};
    return result -= rhs;
}
constexpr Vector3D operator-(const Vector3D& v) noexcept
{
    return Vector3D {-v.x, -v.y, -v.z};
}

constexpr Vector3D operator*(const Vector3D& lhs, const Vector3D& rhs) noexcept
{
    auto result {lhs};
    return result *= rhs;
}
constexpr Vector3D operator*(const Vector3D& lhs, float scalar) noexcept
{
    auto result {lhs};
    return result *= scalar;
}
constexpr Vector3D operator*(float scalar, const Vector3D& rhs) noexcept
{
    return rhs * scalar;
}

constexpr Vector3D operator/(const Vector3D& lhs, const Vector3D& rhs) noexcept
{
    // 가정: rhs의 모든 성분이 0.f가 아니다.
    auto result {lhs};
    return result /= rhs;
}
constexpr Vector3D operator/(const Vector3D& lhs, float scalar) noexcept
{
    // 가정: scalar는 0.f가 아니다.
    auto result {lhs};
    return result /= scalar;
}
}
