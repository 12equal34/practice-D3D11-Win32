#include "IndexBuffer.h"
#include "DXExceptionMacro.h"

using namespace Microsoft::WRL;
using namespace Hardware::DX;

IndexBuffer::IndexBuffer(Renderer& renderer, UINT numIndex,
                         UINT structureByteStride, const void* indices)
    : m_numIndex(numIndex),
      m_structureByteStride(structureByteStride),
      m_dxgiFormat(DXGI_FORMAT_R16_UINT)
{
    switch (m_structureByteStride) {
    case (sizeof(unsigned short)):
        m_dxgiFormat = DXGI_FORMAT_R16_UINT;
        break;
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
    ThrowIfFailed(GetDevice(renderer)->CreateBuffer(&bd, &sd, &m_pIndexBuffer));
}

void IndexBuffer::Bind(Renderer& renderer) noexcept
{
    // bind index buffer
    GetContext(renderer)->IASetIndexBuffer(m_pIndexBuffer.Get(), m_dxgiFormat,
                                           0u);
}

UINT Hardware::DX::IndexBuffer::GetIndexCount() const noexcept
{
    return m_numIndex;
}
