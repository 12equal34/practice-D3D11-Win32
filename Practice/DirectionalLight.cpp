#include "DirectionalLight.h"
#include "ConstantBuffers.h"

namespace WO  = World::Object;
namespace hdx = Hardware::DX;

WO::DirectionalLight::DirectionalLight(DirectX::XMFLOAT4 lightColor)
    : m_directionalLightColor(lightColor),
      m_directionalLightCB(sizeof(ConstantBufferNeverChanges))
{
    m_directionalLightCB.SetToPixelShader(0u);
}

void World::Object::DirectionalLight::Bind()
{
    auto newData = ConstantBufferNeverChanges(m_coord.DirBackward(),
                                              m_directionalLightColor);
    m_directionalLightCB.Update(&newData);
}

void WO::DirectionalLight::SetLightColor(float r, float g, float b,
                                         float a) noexcept
{
    m_directionalLightColor.x = r;
    m_directionalLightColor.y = g;
    m_directionalLightColor.z = b;
    m_directionalLightColor.w = a;
}
