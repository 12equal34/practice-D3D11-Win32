#include "Surface.h"

#include "Renderer.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace Hardware::DX;
namespace WO = World::Object;

WO::Surface::Surface(size_t numXGrid, size_t numZGrid, float gridSize)
    : m_numXGrid(numXGrid),
      m_numZGrid(numZGrid),
      m_gridSize(gridSize),
      m_H(m_numXGrid + 1, std::vector<float>(m_numZGrid + 1, 0.0f))
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
    size_t numXVertex = m_numXGrid + 1;
    size_t numZVertex = m_numZGrid + 1;
    size_t numVertex  = static_cast<size_t>(numXVertex * numZVertex);

    std::vector<VertexType> vertices;
    vertices.reserve(numVertex);
    for (size_t i = 0; i < numXVertex; ++i) {
        for (size_t j = 0; j < numZVertex; ++j) {
            DirectX::XMFLOAT3 position {
                m_coord.GetPositionX() + m_gridSize * i, m_H[i][j],
                m_coord.GetPositionZ() + m_gridSize * j};

            vertices.emplace_back(position);
        }
    }
    return std::make_unique<VertexBuffer>(
        (UINT)numVertex, (UINT)sizeof(VertexType), vertices.data());
}

std::unique_ptr<IndexBuffer> WO::Surface::GetIndexBuffer()
{
    constexpr size_t numIndexOfOneGrid = 6;

    m_numIndex = static_cast<UINT>(m_numXGrid * m_numZGrid * numIndexOfOneGrid);

    std::vector<IndexType> indices;
    indices.reserve(m_numIndex);

    for (size_t xGrid = 0; xGrid < m_numXGrid; ++xGrid) {
        size_t numZVertex = m_numZGrid + 1;
        size_t offset     = numZVertex * xGrid;
        for (size_t zGrid = 0; zGrid < m_numZGrid; ++zGrid) {
            // a lower triangle
            // 1 (offset+1)  * (offset+1+numZVertex)
            // 0 (offset)    2 (offset+numZVertex)
            indices.push_back(static_cast<IndexType>(offset));
            indices.push_back(static_cast<IndexType>(offset + 1));
            indices.push_back(static_cast<IndexType>(offset + numZVertex));

            // a upper triangle
            // 1 (offset+1)  2 (offset+1+numZVertex)
            // * (offset)    0 (offset+numZVertex)
            indices.push_back(static_cast<IndexType>(offset + numZVertex));
            indices.push_back(static_cast<IndexType>(offset + 1));
            indices.push_back(static_cast<IndexType>(offset + 1 + numZVertex));

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

WO::Information::Coordinate::Point3D
WO::Surface::GetCenterPosition() const noexcept
{
    float centerX = GetLengthX() / 2.0f;
    float centerY = m_coord.GetPositionY();
    float centerZ = GetLengthZ() / 2.0f;

    return Information::Coordinate::Point3D {centerX, centerY, centerZ};
}

float WO::Surface::GetLengthX() const noexcept
{
    return m_numXGrid * m_gridSize;
}

float WO::Surface::GetLengthZ() const noexcept
{
    return m_numZGrid * m_gridSize;
}

void WO::Surface::SetCenterPosition(
    const Information::Coordinate::Point3D& newCenterPos) noexcept
{
    float posX = newCenterPos.x - GetLengthX() / 2.0f;
    float posY = newCenterPos.y;
    float posZ = newCenterPos.z - GetLengthZ() / 2.0f;

    m_coord.SetPosition(posX, posY, posZ);
}
