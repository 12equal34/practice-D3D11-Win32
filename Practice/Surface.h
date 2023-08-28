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
    using IndexType  = unsigned short;
    using VertexType = SimpleVertex;

public:
    Surface(int numX, int numZ, float gridSize = 1.0f);

    std::unique_ptr<Hardware::DX::VertexBuffer> GetVertexBuffer() override;
    std::unique_ptr<Hardware::DX::IndexBuffer>  GetIndexBuffer() override;
    UINT GetIndexCount() const noexcept override;
    void Bind() override;
protected:
    int                             m_nx;
    int                             m_nz;
    float                           m_gridSize;
    std::vector<std::vector<float>> m_H;
};
}