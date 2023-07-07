#include "DirectionalLight.h"

namespace WO = World::Object;

WO::DirectionalLight::DirectionalLight() 
    : DirectionalLight({ 1.0f, 1.0f, 1.0f, 1.0f })
{ }

WO::DirectionalLight::DirectionalLight(DirectX::XMFLOAT4 lightColor)
    : m_information {lightColor, {0.0f, 0.0f, 1.0f}},
      m_cbuf(sizeof(Information), &m_information)
{ }

void World::Object::DirectionalLight::Bind() noexcept
{
}

void WO::DirectionalLight::SetLightColor(float r, float g, float b,
                                         float a) noexcept
{
    m_information.light_color.x = r;
    m_information.light_color.y = g;
    m_information.light_color.z = b;
    m_information.light_color.w = a;
}
