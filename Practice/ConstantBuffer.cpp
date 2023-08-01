#include "ConstantBuffer.h"
#include "DXExceptionMacro.h"

namespace hdx = Hardware::DX;

hdx::ConstantBuffer::ConstantBuffer(UINT byteWidth)
{
    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = 0u;
    cbd.MiscFlags = 0u;
    cbd.ByteWidth = byteWidth;
    cbd.StructureByteStride = 0u;
    ThrowIfFailed(
        DXResource::GetDevice()->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer));
}

hdx::ConstantBuffer::ConstantBuffer(UINT        byteWidth,
                                    const void* pConstantBufferData)
{
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
        DXResource::GetDevice()->CreateBuffer(&cbd, &csd, &m_pConstantBuffer));
}

void Hardware::DX::ConstantBuffer::Update(const void* pSrcData)
{
    DXResource::GetContext()->UpdateSubresource(m_pConstantBuffer.Get(), 0u,
                                                nullptr, pSrcData, 0u, 0u);
}

void hdx::ConstantBuffer::SetToVertexShader(UINT startSlot) noexcept
{
    DXResource::GetContext()->VSSetConstantBuffers(
        startSlot, 1u, m_pConstantBuffer.GetAddressOf());
}

void hdx::ConstantBuffer::SetToPixelShader(UINT startSlot) noexcept
{
    DXResource::GetContext()->PSSetConstantBuffers(
        startSlot, 1u, m_pConstantBuffer.GetAddressOf());
}