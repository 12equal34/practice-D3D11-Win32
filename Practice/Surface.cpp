#include "Surface.h"

#include "Renderer.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace Hardware::DX;
namespace WO = World::Object;

WO::Surface::Surface(int numZ, int numX, float gridSize)
    : m_nx(numX),
      m_nz(numZ),
      m_gridSize(gridSize),
      m_H(m_nx, std::vector<float>(m_nz, 0.0f))
{
    m_bindings.reserve(6);

    // vertex/index buffer
    m_bindings.push_back(std::move(GetVertexBuffer()));
    m_bindings.push_back(std::move(GetIndexBuffer()));

    // vertex shader & input layout
    auto pVertexShader =
        std::make_unique<VertexShader>(L"Shaders/GerstnerWaveVS.cso");

    auto& vertexLayout = SimpleVertexLayout;

    m_bindings.push_back(std::move(std::make_unique<InputLayout>(
        vertexLayout, (UINT)std::size(vertexLayout), *pVertexShader.get())));
    m_bindings.push_back(std::move(pVertexShader));

    // pixel shader
    m_bindings.push_back(
        std::move(std::make_unique<PixelShader>(L"Shaders/PixelShader.cso")));
}

std::unique_ptr<VertexBuffer> WO::Surface::GetVertexBuffer()
{
    size_t numVertex = static_cast<size_t>(m_nx * m_nz);

    std::vector<VertexType> vertices;
    vertices.reserve(numVertex);
    for (size_t i = 0; i < m_nx; ++i) {
        for (size_t j = 0; j < m_nz; ++j) {

            DirectX::XMFLOAT3 position;
            position.x = m_coord.GetPositionX() + m_gridSize * i;
            position.z = m_coord.GetPositionZ() + m_gridSize * j;
            position.y = m_H[i][j];

            VertexType vertex {position};

            vertices.push_back(std::move(vertex));
        }
    }
    return std::make_unique<VertexBuffer>(
        (UINT)numVertex, (UINT)sizeof(VertexType), vertices.data());
}

std::unique_ptr<IndexBuffer> WO::Surface::GetIndexBuffer()
{
    constexpr UINT numIndexOfOneGrid = 6;

    const int grid_maxX = m_nx - 1;
    const int grid_maxZ = m_nz - 1;
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
    return std::make_unique<IndexBuffer>(m_numIndex, (UINT)sizeof(IndexType),
                                         indices.data());
}

UINT WO::Surface::GetIndexCount() const noexcept { return m_numIndex; }

void WO::Surface::Bind()
{
    for (auto& binding : m_bindings) {
        binding->Bind();
    }
}
