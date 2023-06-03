#include "Surface.h"
#include "Renderer.h"
#include "BindableHeader.h"

using namespace Hardware::DX;

Surface::Surface(int numZ, int numX)
    : m_nx(numX),
      m_nz(numZ),
      m_H(m_nx, std::vector<float>(m_nz, 0.0f))
{ 
    
}

std::unique_ptr<VertexBuffer>
Surface::GetVertexBuffer(Renderer& renderer, float mx, float mz, float gridSize)
{
    int numVertex = m_nx * m_nz;

    std::vector<Vertex> vertices(numVertex);
    for (int i = 0; i < m_nx; ++i) {
        for (int j = 0; j < m_nz; ++j) {
            float x                = mx + gridSize * i;
            float z                = mz + gridSize * j;
            float y                = m_H[i][j];
            vertices[i * m_nz + j] = {
                {x, y, z}
            };
        }
    }
    return std::make_unique<VertexBuffer>(renderer, (UINT)numVertex, (UINT)sizeof(Vertex),
                                          vertices.data());
}

std::unique_ptr<IndexBuffer> Surface::GetIndexBuffer(Renderer& renderer)
{
    constexpr UINT numIndexOfOneGrid = 6;

    int grid_maxX = m_nx - 1;
    int grid_maxZ = m_nz - 1;
    m_numIndex = static_cast<UINT>(grid_maxX * grid_maxZ) * numIndexOfOneGrid;

    std::vector<IndexType> indices;
    indices.reserve(m_numIndex);

    for (int grid_x = 0; grid_x < grid_maxX; ++grid_x) {
        int offset = m_nz * grid_x;
        for (int grid_z = 0; grid_z < grid_maxZ; ++grid_z) {
            indices.push_back(offset);
            indices.push_back(offset + 1);
            indices.push_back(offset + m_nz);
            indices.push_back(offset + m_nz);
            indices.push_back(offset + 1);
            indices.push_back(offset + m_nz + 1);
            ++offset;
        }
    }
    return std::make_unique<IndexBuffer>(renderer, m_numIndex, (UINT)sizeof(IndexType),
                                         indices.data());
}

UINT Surface::GetIndexCount() const noexcept
{
    return m_numIndex;
}

void Surface::Bind(const DXResources& dxResource, Renderer& renderer)
{
    std::vector<std::unique_ptr<Bindable>> bindings;
    bindings.push_back(std::move(GetVertexBuffer(renderer, -20.f, -20.f, 0.5f)));
    bindings.push_back(std::move(GetIndexBuffer(renderer)));
    bindings.push_back(std::move(std::make_unique<Viewport>(
        renderer, static_cast<FLOAT>(dxResource.ClientRect.right),
        static_cast<FLOAT>(dxResource.ClientRect.bottom))));
    bindings.push_back(std::move(
        std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)));

    auto pVertexShader =
        std::make_unique<VertexShader>(renderer, L"Shaders/VertexShader.cso");


    bindings.push_back(
        std::move(std::make_unique<InputLayout>(renderer, *pVertexShader.get())));
    bindings.push_back(std::move(pVertexShader));
    bindings.push_back(std::move(
        std::make_unique<PixelShader>(renderer, L"Shaders/PixelShader.cso")));

    for (auto& binding : bindings) {
        binding->Bind(renderer);
    }
}
