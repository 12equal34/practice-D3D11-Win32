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
        DirectX::XMFLOAT4 lightBaseColor  = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f,
                                                              0.0f),
        DirectX::XMFLOAT4 lightColorRange = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f,
                                                              0.0f));

    void Bind() override;
    void SetLightBaseColor(float r, float g, float b, float a) noexcept;
    void SetLightColorRange(float r, float g, float b, float a) noexcept;
private:
    DirectX::XMFLOAT4            m_ambientLightBaseColor;
    DirectX::XMFLOAT4            m_ambientLightColorRange;
    Hardware::DX::ConstantBuffer m_ambientLightCB;
};
}
