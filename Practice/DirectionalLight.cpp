#include "DirectionalLight.h"

namespace WO  = World::Object;
namespace hdx = Hardware::DX;

WO::DirectionalLight::DirectionalLight() noexcept
    : DirectionalLight({1.0f, 1.0f, 1.0f, 1.0f})
{ }

WO::DirectionalLight::DirectionalLight(DirectX::XMFLOAT4 lightColor) noexcept
    : m_information {lightColor, {0.0f, 0.0f, 1.0f}},
    m_infoCbuf(sizeof(Information), &m_information)
{
    m_infoCbuf.SetToPixelShader(0u);
}

void World::Object::DirectionalLight::Bind() noexcept
{
    m_information.light_direction = m_coord.DirBackward();
    m_infoCbuf.Update(&m_information);
}

void WO::DirectionalLight::SetLightColor(float r, float g, float b,
                                         float a) noexcept
{
    m_information.light_color.x = r;
    m_information.light_color.y = g;
    m_information.light_color.z = b;
    m_information.light_color.w = a;
}
