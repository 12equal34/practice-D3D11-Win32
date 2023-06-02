#pragma once
#include "Bindable.h"

namespace Hardware::DX
{
class IndexBuffer : public Bindable
{
public:
    IndexBuffer(Renderer& renderer, UINT numIndex, UINT structureByteStride,
                const void* indices);

    void Bind(Renderer& renderer) noexcept override;
    UINT GetIndexCount() const noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
public:
    DXGI_FORMAT m_dxgiFormat;
    UINT        m_numIndex;
    UINT        m_structureByteStride;
};
}
