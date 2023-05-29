#include "DXGettable.h"

using namespace Hardware::DX;

ID3D11Device* DXGettable::GetDevice(const Renderer& renderer) noexcept
{
    return renderer.m_dx.Device.Get();
}

ID3D11DeviceContext* DXGettable::GetContext(const Renderer& renderer) noexcept
{
    return renderer.m_dx.Context.Get();
}
