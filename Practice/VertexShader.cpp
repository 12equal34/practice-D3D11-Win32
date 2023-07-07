#include "VertexShader.h"
#include "DXExceptionMacro.h"

namespace HDX = Hardware::DX;

HDX::VertexShader::VertexShader(std::wstring_view wsv)
{
    // create a vertex shader
    ThrowIfFailed(D3DReadFileToBlob(wsv.data(), &m_pBlob));
    ThrowIfFailed(DXResource::GetDevice()->CreateVertexShader(m_pBlob->GetBufferPointer(),
                  m_pBlob->GetBufferSize(),
                  nullptr, &m_pVertexShader));
}

void HDX::VertexShader::Bind() noexcept
{
    // bind a vertex shader
    DXResource::GetContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}

ID3D10Blob* HDX::VertexShader::GetBlob() noexcept
{
    return m_pBlob.Get();
}
