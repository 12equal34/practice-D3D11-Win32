#include "ConstantBuffer.h"
#include <DirectXMath.h>
#include "DXExceptionMacro.h"

using namespace Hardware::DX;
using namespace DirectX;

ConstantBuffer::ConstantBuffer(Renderer& renderer, UINT byteWidth,
                               const void* pConstantBufferData)
{
    // create constant buffer 2
    D3D11_BUFFER_DESC cbd      = {};
    cbd.Usage                  = D3D11_USAGE_DEFAULT;
    cbd.BindFlags              = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags         = 0u;
    cbd.MiscFlags              = 0u;
    cbd.ByteWidth              = byteWidth;
    cbd.StructureByteStride    = 0u;
    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem                = pConstantBufferData;
    ThrowIfFailed(
        GetDevice(renderer)->CreateBuffer(&cbd, &csd, &m_pConstantBuffer));
}

void ConstantBuffer::SetToVertexShader(Renderer& renderer, UINT startSlot) noexcept
{
    GetContext(renderer)->VSSetConstantBuffers(startSlot, 1u, m_pConstantBuffer.GetAddressOf());
}

void ConstantBuffer::SetToPixelShader(Renderer& renderer, UINT startSlot) noexcept
{
    GetContext(renderer)->PSSetConstantBuffers(startSlot, 1u, m_pConstantBuffer.GetAddressOf());
}