#pragma once
#include "Vector3D.h"

namespace gp::math
{
struct Point3D;
struct Direction3D;
struct Orientation3D;

struct Point3D : Data3D {
    constexpr Point3D() noexcept = default;
    constexpr Point3D(float x, float y, float z) noexcept
        : Data3D(x, y, z)
    { }

    constexpr Point3D(const Point3D&) noexcept            = default;
    constexpr Point3D& operator=(const Point3D&) noexcept = default;

    constexpr Point3D(Point3D&&) noexcept            = default;
    constexpr Point3D& operator=(Point3D&&) noexcept = default;

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
    constexpr Direction3D() noexcept = default;
    constexpr Direction3D(float x, float y, float z) noexcept
        : Data3D(x, y, z)
    { }

    constexpr Direction3D(const Direction3D&) noexcept            = default;
    constexpr Direction3D& operator=(const Direction3D&) noexcept = default;

    constexpr Direction3D(Direction3D&&) noexcept            = default;
    constexpr Direction3D& operator=(Direction3D&&) noexcept = default;

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

    constexpr Direction3D operator-() const noexcept
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

struct Orientation3D : Data3D {
    constexpr Orientation3D() noexcept = default;
    constexpr Orientation3D(float pitch, float yaw, float roll) noexcept
        : Data3D(pitch, yaw, roll)
    { }

    constexpr Orientation3D(const Orientation3D&) noexcept            = default;
    constexpr Orientation3D& operator=(const Orientation3D&) noexcept = default;

    constexpr Orientation3D(Orientation3D&&) noexcept            = default;
    constexpr Orientation3D& operator=(Orientation3D&&) noexcept = default;

    constexpr void Set(float pitch, float yaw, float roll) noexcept
    {
        x = pitch;
        y = yaw;
        z = roll;
    }
    constexpr void SetPitch(float pitch) noexcept { x = pitch; }
    constexpr void SetYaw(float yaw) noexcept { y = yaw; }
    constexpr void SetRoll(float roll) noexcept { z = roll; }

    constexpr float GetPitch() const noexcept { return x; }
    constexpr float GetYaw() const noexcept { return y; }
    constexpr float GetRoll() const noexcept { return z; }

    Direction3D GetAxisX() const noexcept
    {
        using DirectX::XMScalarSinCos;
        DirectX::XMScalarSin(1.f);
        float cosX, sinX;
        float cosY, sinY;
        float cosZ, sinZ;
        XMScalarSinCos(&cosX, &sinX, x);
        XMScalarSinCos(&cosY, &sinY, x);
        XMScalarSinCos(&cosZ, &sinZ, x);

        float sinXsinZ = sinX * sinZ;

        return Direction3D {cosY * cosZ + sinXsinZ * sinY, cosX * sinZ,
                            cosY * sinXsinZ - cosZ * sinY};
    }
    Direction3D GetAxisY() const noexcept
    {
        using DirectX::XMScalarSinCos;

        float cosX, sinX;
        float cosY, sinY;
        float cosZ, sinZ;
        XMScalarSinCos(&cosX, &sinX, x);
        XMScalarSinCos(&cosY, &sinY, x);
        XMScalarSinCos(&cosZ, &sinZ, x);

        float cosZsinX = cosZ * sinX;

        return Direction3D {cosZsinX * sinY - cosY * sinZ, cosX * cosZ,
                            sinY * sinZ + cosY * cosZsinX};
    }
    Direction3D GetAxisZ() const noexcept
    {
        using DirectX::XMScalarSinCos;

        float cosX, sinX;
        float cosY, sinY;
        XMScalarSinCos(&cosX, &sinX, x);
        XMScalarSinCos(&cosY, &sinY, x);

        return Direction3D {cosX * sinY, -sinX, cosX * cosY};
    }

    DirectX::XMMATRIX GetMatrixRotationRollPitchYaw() const noexcept
    {
        return DirectX::XMMatrixRotationRollPitchYaw(x, y, z);
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

constexpr Point3D operator*(const Point3D& p, float scalar) noexcept
{
    return Point3D {p.x * scalar, p.y * scalar, p.z * scalar};
}

constexpr Point3D operator*(float scalar, const Point3D& p) noexcept
{
    return p * scalar;
}

constexpr Direction3D operator*(const Direction3D& dir, float scalar) noexcept
{
    return Direction3D {dir.x * scalar, dir.y * scalar, dir.z * scalar};
}

constexpr Direction3D operator*(float scalar, const Direction3D& dir) noexcept
{
    return dir * scalar;
}
}