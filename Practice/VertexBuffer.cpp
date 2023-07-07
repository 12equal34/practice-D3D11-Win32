#include "VertexBuffer.h"
#include "DXExceptionMacro.h"

namespace HDX = Hardware::DX;

HDX::VertexBuffer::VertexBuffer(UINT numVertex,
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
        DXResource::GetDevice()->CreateBuffer(&bd, &sd, &m_pVertexBuffer));
}

void HDX::VertexBuffer::Bind() noexcept
{
    // bind vertex buffer
    const UINT offset = 0u;
    DXResource::GetContext()->IASetVertexBuffers(0u, 1u,
                                             m_pVertexBuffer.GetAddressOf(),
                                             &m_structureByteStride, &offset);
}
