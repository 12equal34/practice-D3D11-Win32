#include "PixelShader.h"

#include <d3dcompiler.h>
#include "DXExceptionMacro.h"

namespace HDX = Hardware::DX;

HDX::PixelShader::PixelShader(std::wstring_view wsv)
{
    // create a pixel shader
    ThrowIfFailed(D3DReadFileToBlob(wsv.data(), &m_pBlob));
    ThrowIfFailed(DXResource::GetDevice()->CreatePixelShader(
        m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr,
        &m_pPixelShader));
}

void HDX::PixelShader::Bind() noexcept
{
    // bind a pixel shader
    DXResource::GetContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}

ID3D10Blob* HDX::PixelShader::GetBlob() noexcept { return m_pBlob.Get(); }
