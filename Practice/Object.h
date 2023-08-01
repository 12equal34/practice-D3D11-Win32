#pragma once
#include <memory>
#include <vector>
#include "IBindable.h"
#include "Coordinatable.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace World::Object
{
class Object : public Hardware::DX::IBindable, public Information::Coordinatable
{
public:
    DirectX::XMMATRIX GetRotationMatrix() const noexcept;
    DirectX::XMMATRIX GetTranslationMatrix() const noexcept;
    DirectX::XMMATRIX GetModelMatrix() const noexcept;

    virtual std::unique_ptr<Hardware::DX::VertexBuffer> GetVertexBuffer() = 0;
    virtual std::unique_ptr<Hardware::DX::IndexBuffer>  GetIndexBuffer()  = 0;
    virtual UINT GetIndexCount() const noexcept                           = 0;
    virtual void Bind() noexcept override                                 = 0;

protected:
    std::vector<std::unique_ptr<Hardware::DX::IBindable>> m_bindings;

    UINT m_numIndex = 0u;
};
}
