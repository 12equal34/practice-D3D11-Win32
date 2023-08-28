#pragma once
#include "IBindable.h"
#include "DXResource.h"

namespace Hardware::DX
{
class VertexBuffer : public IBindable
{
public:
    VertexBuffer(UINT numVertex, UINT structureByteStride,
                 const void* vertices);

    void Bind() override;
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
    UINT                                 m_structureByteStride;
};
}
