#pragma once
#include "Coordinatable.h"
#include "IBindable.h"
#include "ConstantBuffer.h"

namespace World::Object
{
class DirectionalLight : public Information::Coordinatable,
                         public Hardware::DX::IBindable
{
public:
    DirectionalLight(DirectX::XMFLOAT4 lightColor =
                         DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

    void Bind() override;
    void SetLightColor(float r, float g, float b, float a) noexcept;
private:
    DirectX::XMFLOAT4            m_directionalLightColor;
    Hardware::DX::ConstantBuffer m_directionalLightCB;
};
}