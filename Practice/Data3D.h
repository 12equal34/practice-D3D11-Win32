#pragma once
#include <DirectXMath.h>

#pragma warning(disable : 26495)

namespace gp::math
{
struct Data3D {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float data[3] = {0.f, 0.f, 0.f};
    };

    constexpr Data3D() noexcept = default;
    constexpr Data3D(float _x, float _y, float _z) noexcept
        : x(_x),
          y(_y),
          z(_z)
    { }

    constexpr Data3D(const Data3D&) noexcept            = default;
    constexpr Data3D& operator=(const Data3D&) noexcept = default;

    constexpr Data3D(Data3D&&) noexcept            = default;
    constexpr Data3D& operator=(Data3D&&) noexcept = default;

    // for SIMD
    Data3D(DirectX::FXMVECTOR v) noexcept
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
    DirectX::XMVECTOR Load() const noexcept
    {
        return DirectX::XMLoadFloat3(
            reinterpret_cast<const DirectX::XMFLOAT3*>(this));
    }
};
}