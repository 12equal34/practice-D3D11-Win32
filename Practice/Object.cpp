#include "Object.h"

DirectX::XMMATRIX World::Object::Object::GetRotationMatrix() const noexcept
{
    return m_coord.GetRotationMatrix();
}

DirectX::XMMATRIX World::Object::Object::GetTranslationMatrix() const noexcept
{
    return m_coord.GetTranslationMatrix();
}

DirectX::XMMATRIX World::Object::Object::GetModelMatrix() const noexcept
{
    return m_coord.GetModelMatrix();
}
