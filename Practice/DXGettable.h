#pragma once
#include "Renderer.h"

namespace Hardware::DX
{
class DXGettable
{
public:
    static ID3D11Device*        GetDevice(const Renderer& renderer) noexcept;
    static ID3D11DeviceContext* GetContext(const Renderer& renderer) noexcept;
};
}