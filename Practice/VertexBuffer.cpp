#include "VertexBuffer.h"
#include "DXExceptionMacro.h"

using namespace Hardware::DX;

VertexBuffer::VertexBuffer(Renderer& renderer, UINT numVertex,
                           UINT structureByteStride, const void* vertices)
    : m_structureByteStride(structureByteStride)
{
    // create vertex buffer
    D3D11_BUFFER_DESC bd      = {};
    bd.Usage                  = D3D11_USAGE_DEFAULT;
    bd.BindFlags              = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags         = 0u;
    bd.MiscFlags              = 0u;
    bd.ByteWidth              = numVertex * structureByteStride;
    bd.StructureByteStride    = structureByteStride;
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem                = vertices;
    ThrowIfFailed(
        GetDevice(renderer)->CreateBuffer(&bd, &sd, &m_pVertexBuffer));
}

void VertexBuffer::Bind(Renderer& renderer) noexcept
{
    // bind vertex buffer
    const UINT offset = 0u;
    GetContext(renderer)->IASetVertexBuffers(0u, 1u,
                                             m_pVertexBuffer.GetAddressOf(),
                                             &m_structureByteStride, &offset);
}
