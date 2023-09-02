#pragma once
#include "Coordinatable.h"
#include "IBindable.h"
#include "ConstantBuffer.h"

namespace World::Object
{
class HemisphericAmbientLight : public Information::Coordinatable,
                                public Hardware::DX::IBindable
{
public:
    HemisphericAmbientLight(
        DirectX::XMFLOAT3 lightBaseColor  = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
        DirectX::XMFLOAT3 lightColorRange = DirectX::XMFLOAT3(0.0f, 0.0f,
                                                              0.0f));

    void Bind() override;
    void SetLightBaseColor(float r, float g, float b, float a) noexcept;
    void SetLightColorRange(float r, float g, float b, float a) noexcept;
private:
    DirectX::XMFLOAT3            m_ambientLightBaseColor;
    DirectX::XMFLOAT3            m_ambientLightColorRange;
    Hardware::DX::ConstantBuffer m_ambientLightCB;
};
}
