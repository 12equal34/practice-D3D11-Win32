#pragma once
#include "Vector3D.h"

namespace gp::math
{
struct Direction3D;

struct Point3D : Data3D {
    using Data3D::Data3D;

    constexpr Point3D& operator+=(const Direction3D& rhs) noexcept
    {
        reinterpret_cast<Vector3D&>(*this) += rhs;
        return *this;
    }

    constexpr Point3D& operator-=(const Direction3D& rhs) noexcept
    {
        reinterpret_cast<Vector3D&>(*this) -= rhs;
        return *this;
    }

    constexpr Point3D& operator*=(float scalar) noexcept
    {
        reinterpret_cast<Vector3D&>(*this) *= scalar;
        return *this;
    }

    constexpr Point3D& operator/=(float scalar) noexcept
    {
        reinterpret_cast<Vector3D&>(*this) /= scalar;
        return *this;
    }
};

struct Direction3D : Data3D {
    using Data3D::Data3D;

    constexpr Direction3D& operator+=(const Direction3D& rhs) noexcept
    {
        reinterpret_cast<Vector3D&>(*this) += rhs;
        return *this;
    }

    constexpr Direction3D& operator-=(const Direction3D& rhs) noexcept
    {
        reinterpret_cast<Vector3D&>(*this) -= rhs;
        return *this;
    }

    constexpr Direction3D& operator-=(const Point3D& rhs) noexcept
    {
        reinterpret_cast<Vector3D&>(*this) -= rhs;
        return *this;
    }

    constexpr Direction3D& operator*=(float scalar) noexcept
    {
        reinterpret_cast<Vector3D&>(*this) *= scalar;
        return *this;
    }

    constexpr Direction3D& operator/=(float scalar) noexcept
    {
        reinterpret_cast<Vector3D&>(*this) /= scalar;
        return *this;
    }

    constexpr Direction3D operator-() noexcept
    {
        return Vector3DNegation(*this);
    }

    constexpr float Dot(const Direction3D& rhs) const noexcept
    {
        return Direction3D::Dot(*this, rhs);
    }

    constexpr Direction3D Cross(const Direction3D& rhs) const noexcept
    {
        return Direction3D::Cross(*this, rhs);
    }

    static constexpr float Dot(const Direction3D& a,
                               const Direction3D& b) noexcept
    {
        return Vector3DDot(a, b);
    }

    static constexpr Direction3D Cross(const Direction3D& a,
                                       const Direction3D& b) noexcept
    {
        return Vector3DCross<Direction3D, Direction3D, Direction3D>(a, b);
    }
};

constexpr Direction3D operator-(const Point3D& to, const Point3D& from) noexcept
{
    return Vector3DSubtraction<Point3D, Point3D, Direction3D>(to, from);
}

constexpr Direction3D operator-(const Direction3D& a,
                                const Direction3D& b) noexcept
{
    return Vector3DSubtraction<Direction3D, Direction3D, Direction3D>(a, b);
}

constexpr Direction3D operator+(const Direction3D& a,
                                const Direction3D& b) noexcept
{
    return Vector3DAddition<Direction3D, Direction3D, Direction3D>(a, b);
}

constexpr Point3D operator-(const Point3D& p, const Direction3D& dp) noexcept
{
    return Vector3DSubtraction<Point3D, Direction3D, Point3D>(p, dp);
}

constexpr Point3D operator+(const Point3D& p, const Direction3D& dp) noexcept
{
    return Vector3DAddition<Point3D, Direction3D, Point3D>(p, dp);
}

constexpr Point3D operator+(const Direction3D& dp, const Point3D& p) noexcept
{
    return Vector3DAddition<Direction3D, Point3D, Point3D>(dp, p);
}
}