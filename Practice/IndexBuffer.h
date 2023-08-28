#pragma once
#include "IBindable.h"
#include "DXResource.h"

namespace Hardware::DX
{
class IndexBuffer : public IBindable
{
public:
    IndexBuffer(UINT numIndex, UINT structureByteStride,
                const void* indices);

    void Bind() override;
    UINT GetIndexCount() const noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
public:
    DXGI_FORMAT m_dxgiFormat;
    UINT        m_numIndex;
    UINT        m_structureByteStride;
};
}
