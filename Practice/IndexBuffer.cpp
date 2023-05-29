#include "IndexBuffer.h"
#include "DXExceptionMacro.h"

using namespace Microsoft::WRL;
using namespace Hardware::DX;

IndexBuffer::IndexBuffer(Renderer& renderer)
{
    const unsigned short indices[] = {
        /* clang-format off */
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4,
        /* clang-format on */
    };

    m_numIndices = ARRAYSIZE(indices);

    // create index buffer
    D3D11_BUFFER_DESC bd      = {};
    bd.Usage                  = D3D11_USAGE_DEFAULT;
    bd.BindFlags              = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags         = 0u;
    bd.MiscFlags              = 0u;
    bd.ByteWidth              = sizeof(indices);
    bd.StructureByteStride    = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem                = indices;
    ThrowIfFailed(GetDevice(renderer)->CreateBuffer(&bd, &sd, &m_pIndexBuffer));
}

void IndexBuffer::Bind(Renderer& renderer) noexcept
{
    // bind index buffer
    GetContext(renderer)->IASetIndexBuffer(m_pIndexBuffer.Get(),
                                           DXGI_FORMAT_R16_UINT, 0u);
}
