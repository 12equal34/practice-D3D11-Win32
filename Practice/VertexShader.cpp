#include "VertexShader.h"

#include <d3dcompiler.h>
#include "DXExceptionMacro.h"

namespace hdx = Hardware::DX;

hdx::VertexShader::VertexShader(std::wstring_view wsv)
{
    // create a vertex shader
    ThrowIfFailed(D3DReadFileToBlob(wsv.data(), &m_pBlob));
    ThrowIfFailed(DXResource::GetDevice()->CreateVertexShader(m_pBlob->GetBufferPointer(),
                  m_pBlob->GetBufferSize(),
                  nullptr, &m_pVertexShader));
}

void hdx::VertexShader::Bind()
{
    // bind a vertex shader
    DXResource::GetContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}

ID3D10Blob* hdx::VertexShader::GetBlob() noexcept
{
    return m_pBlob.Get();
}
