#pragma once
#include "DXResource.h"

namespace Hardware::DX
{
class ConstantBuffer
{
public:
    ConstantBuffer(UINT byteWidth, const void* pInitialData = nullptr);

    void Update(const void* pNewSrcData);
    void SetToVertexShader(UINT startSlot);
    void SetToPixelShader(UINT startSlot);
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};
}