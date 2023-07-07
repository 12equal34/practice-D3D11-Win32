#pragma once
#include "DXResource.h"

namespace Hardware::DX
{
class ConstantBuffer
{
public:
    ConstantBuffer(UINT byteWidth, const void* pConstantBufferData);

    void Update(const void* pSrcData);
    void SetToVertexShader(UINT startSlot) noexcept;
    void SetToPixelShader(UINT startSlot) noexcept;
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};
}