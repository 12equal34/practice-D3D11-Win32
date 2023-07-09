#include "Object.h"

DirectX::XMMATRIX World::Object::Object::GetModelRotation() const noexcept
{
    return m_coordinate.GetModelRotation();
}

DirectX::XMMATRIX World::Object::Object::GetModelTranslation() const noexcept
{
    return m_coordinate.GetModelTranslation();
}

DirectX::XMMATRIX World::Object::Object::GetModelMatrix() const noexcept
{
    return m_coordinate.GetModelMatrix();
}
