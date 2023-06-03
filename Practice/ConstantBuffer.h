#pragma once
#include "DXGettable.h"

namespace Hardware::DX
{
class ConstantBuffer : public DXGettable
{
public:
    ConstantBuffer(Renderer& renderer, UINT byteWidth,
                   const void* pConstantBufferData);

    void SetToVertexShader(Renderer& renderer, UINT startSlot) noexcept;
    void SetToPixelShader(Renderer& renderer, UINT startSlot) noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};
}