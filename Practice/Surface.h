#pragma once
#include <vector>
#include "Bindable.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Coordinatable.h"

namespace World::Object
{
class Surface : public Hardware::DX::Bindable, public Coordinatable
{
public:
    using IndexType = unsigned short;

    struct VertexType {
        float x, y, z;
    };

public:
    Surface(Hardware::DX::Renderer& renderer, int numX, int numZ);
    std::unique_ptr<Hardware::DX::VertexBuffer>
    GetVertexBuffer(Hardware::DX::Renderer& renderer, float gridSize);
    std::unique_ptr<Hardware::DX::IndexBuffer>
         GetIndexBuffer(Hardware::DX::Renderer& renderer);
    UINT GetIndexCount() const noexcept;
    void Bind(Hardware::DX::Renderer& renderer) noexcept;
protected:
    std::vector<std::unique_ptr<Hardware::DX::Bindable>> m_bindings;

    UINT                            m_numIndex;
    int                             m_nx;
    int                             m_nz;
    std::vector<std::vector<float>> m_H;
};
}