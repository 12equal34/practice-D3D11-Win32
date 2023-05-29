#include "PixelShader.h"
#include "DXExceptionMacro.h"

using namespace Hardware::DX;

PixelShader::PixelShader(Renderer& renderer, std::wstring_view wsv)
{
    // create a pixel shader
    ThrowIfFailed(D3DReadFileToBlob(wsv.data(), &m_pBlob));
    ThrowIfFailed(GetDevice(renderer)->CreatePixelShader(m_pBlob->GetBufferPointer(),
                  m_pBlob->GetBufferSize(),
                  nullptr, &m_pPixelShader));
}

void PixelShader::Bind(Renderer& renderer) noexcept
{
    // bind a pixel shader
    GetContext(renderer)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}

ID3D10Blob* PixelShader::GetBlob() noexcept
{
    return m_pBlob.Get();
}
