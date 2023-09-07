#pragma once
#include "Data3D.h"

namespace gp::math
{
struct Vector3D;

template <class L, class R, class ResultVec = Vector3D>
constexpr ResultVec Vector3DAddition(const L& a, const R& b) noexcept
{
    return ResultVec {a.x + b.x, a.y + b.y, a.z + b.z};
}

template <class L, class R, class ResultVec = Vector3D>
constexpr ResultVec Vector3DSubtraction(const L& a, const R& b) noexcept
{
    return ResultVec {a.x - b.x, a.y - b.y, a.z - b.z};
}

template <class L, class R, class ResultVec = Vector3D>
constexpr ResultVec Vector3DMultiplication(const L& a, const R& b) noexcept
{
    return ResultVec {a.x * b.x, a.y * b.y, a.z * b.z};
}

template <class L, class R, class ResultVec = Vector3D>
constexpr ResultVec Vector3DDivision(const L& a, const R& b) noexcept
{
    return ResultVec {a.x / b.x, a.y / b.y, a.z / b.z};
}

template <class L, class R>
constexpr float Vector3DDot(const L& a, const R& b) noexcept
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <class L, class R, class ResultVec = Vector3D>
constexpr ResultVec Vector3DCross(const L& a, const R& b) noexcept
{
    return ResultVec {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                      a.x * b.y - a.y * b.x};
}

template <class VectorType>
constexpr VectorType Vector3DNegation(const VectorType& v) noexcept
{
    return VectorType {-v.x, -v.y, -v.z};
}

struct Vector3D : Data3D {
    using Data3D::Data3D;

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
        return Vector3DDot(a, b);
    }

    static constexpr Vector3D Cross(const Vector3D& a,
                                    const Vector3D& b) noexcept
    {
        return Vector3DCross(a, b);
    }
};

constexpr Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs) noexcept
{
    return Vector3DAddition(lhs, rhs);
}

constexpr Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs) noexcept
{
    return Vector3DSubtraction(lhs, rhs);
}
constexpr Vector3D operator-(const Vector3D& v) noexcept
{
    return Vector3DNegation(v);
}

constexpr Vector3D operator*(const Vector3D& lhs, const Vector3D& rhs) noexcept
{
    return Vector3DMultiplication(lhs, rhs);
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
    return Vector3DDivision(lhs, rhs);
}
constexpr Vector3D operator/(const Vector3D& lhs, float scalar) noexcept
{
    auto result {lhs};
    return result /= scalar;
}
}