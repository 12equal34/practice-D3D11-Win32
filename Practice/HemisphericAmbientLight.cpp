#include "HemisphericAmbientLight.h"
#include "ConstantBuffers.h"

World::Object::HemisphericAmbientLight::HemisphericAmbientLight(
    DirectX::XMFLOAT3 lightBaseColor, DirectX::XMFLOAT3 lightColorRange)
    : m_ambientLightBaseColor(lightBaseColor),
      m_ambientLightColorRange(lightColorRange),
      m_ambientLightCB(sizeof(ConstantBufferHemisphericAmbientLight))
{
    m_ambientLightCB.SetToPixelShader(4u);
}

void World::Object::HemisphericAmbientLight::Bind()
{
    auto newData = ConstantBufferHemisphericAmbientLight(
        m_ambientLightBaseColor, m_ambientLightColorRange);
    m_ambientLightCB.Update(&newData);
}

void World::Object::HemisphericAmbientLight::SetLightBaseColor(float r, float g,
                                                               float b,
                                                               float a) noexcept
{
    m_ambientLightBaseColor.x = r * a;
    m_ambientLightBaseColor.y = g * a;
    m_ambientLightBaseColor.z = b * a;
}

void World::Object::HemisphericAmbientLight::SetLightColorRange(
    float r, float g, float b, float a) noexcept
{
    m_ambientLightColorRange.x = r * a;
    m_ambientLightColorRange.y = g * a;
    m_ambientLightColorRange.z = b * a;
}
