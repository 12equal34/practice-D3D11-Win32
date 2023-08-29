#pragma once
#include <vector>
#include <memory>
#include "Object.h"
#include "InputLayout.h"

namespace World::Object
{
class Surface : public Object
{
public:
    using IndexType  = UINT;
    using VertexType = SimpleVertex;

public:
    Surface(size_t numXGrid, size_t numZGrid, float gridSize = 1.0f);

    std::unique_ptr<Hardware::DX::VertexBuffer> GetVertexBuffer() override;
    std::unique_ptr<Hardware::DX::IndexBuffer>  GetIndexBuffer() override;
    UINT GetIndexCount() const noexcept override;
    void Bind() override;
protected:
    size_t                          m_numXGrid;
    size_t                          m_numZGrid;
    float                           m_gridSize;
    std::vector<std::vector<float>> m_H;
};
}