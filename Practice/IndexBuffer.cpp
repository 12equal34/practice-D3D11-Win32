#include "IndexBuffer.h"
#include "DXExceptionMacro.h"

namespace hdx = Hardware::DX;

hdx::IndexBuffer::IndexBuffer(UINT numIndex, UINT structureByteStride,
                              const void* indices)
    : m_numIndex(numIndex),
      m_structureByteStride(structureByteStride),
      m_dxgiFormat(DXGI_FORMAT_R16_UINT)
{
    switch (m_structureByteStride) {
    case 2u:
        m_dxgiFormat = DXGI_FORMAT_R16_UINT;
        break;
    case 4u:
        m_dxgiFormat = DXGI_FORMAT_R32_UINT;
        break;
    default:
        throw;
    }

    // create index buffer
    D3D11_BUFFER_DESC bd      = {};
    bd.Usage                  = D3D11_USAGE_DEFAULT;
    bd.BindFlags              = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags         = 0u;
    bd.MiscFlags              = 0u;
    bd.ByteWidth              = m_numIndex * m_structureByteStride;
    bd.StructureByteStride    = m_structureByteStride;
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem                = indices;
    ThrowIfFailed(
        DXResource::GetDevice()->CreateBuffer(&bd, &sd, &m_pIndexBuffer));
}

void hdx::IndexBuffer::Bind()
{
    // bind index buffer
    DXResource::GetContext()->IASetIndexBuffer(m_pIndexBuffer.Get(),
                                               m_dxgiFormat, 0u);
}

UINT hdx::IndexBuffer::GetIndexCount() const noexcept { return m_numIndex; }
