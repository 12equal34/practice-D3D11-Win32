#pragma once
#include "Coordinatable.h"
#include "IBindable.h"
#include "ConstantBuffer.h"

namespace World::Object
{
class DirectionalLight : public Coordinatable, public Hardware::DX::IBindable
{
public:
    struct Information {
        DirectX::XMFLOAT4 light_color;
        alignas(16) DirectX::XMFLOAT3 light_direction;
    };

    DirectionalLight() noexcept;
    DirectionalLight(DirectX::XMFLOAT4 lightColor) noexcept;

    void Bind() noexcept override;
    void SetLightColor(float r, float g, float b, float a) noexcept;
private:
    Information                  m_information;
    Hardware::DX::ConstantBuffer m_cbuf;
};
}