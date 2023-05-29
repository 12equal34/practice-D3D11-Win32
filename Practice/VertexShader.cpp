#include "VertexShader.h"
#include "DXExceptionMacro.h"

using namespace Hardware::DX;

VertexShader::VertexShader(Renderer& renderer, std::wstring_view wsv)
{
    // create a vertex shader
    ThrowIfFailed(D3DReadFileToBlob(wsv.data(), &m_pBlob));
    ThrowIfFailed(GetDevice(renderer)->CreateVertexShader(m_pBlob->GetBufferPointer(),
                  m_pBlob->GetBufferSize(),
                  nullptr, &m_pVertexShader));
}

void VertexShader::Bind(Renderer& renderer) noexcept
{
    // bind a vertex shader
    GetContext(renderer)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}

ID3D10Blob* VertexShader::GetBlob() noexcept
{
    return m_pBlob.Get();
}
