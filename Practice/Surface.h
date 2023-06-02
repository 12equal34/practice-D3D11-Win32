#pragma once
#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Surface
{
public:
    using IndexType = unsigned short;

    struct Vertex {
        struct {
            float x;
            float y;
            float z;
        } pos;
    };

public:
    Surface(int numX, int numZ);
    std::unique_ptr<Hardware::DX::VertexBuffer>
    GetVertexBuffer(Hardware::DX::Renderer& renderer, float mx, float mz,
                    float gridSize);
    std::unique_ptr<Hardware::DX::IndexBuffer>
         GetIndexBuffer(Hardware::DX::Renderer& renderer);
    UINT GetIndexCount() const noexcept;
    void Bind(const Hardware::DX::DXResources& dxResource, Hardware::DX::Renderer& renderer);
protected:
    UINT                            m_numIndex = 0u;
    int                             m_nx;
    int                             m_nz;
    std::vector<std::vector<float>> m_H;
};