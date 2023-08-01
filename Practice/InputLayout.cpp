#include "InputLayout.h"
#include "DXExceptionMacro.h"

namespace hdx = Hardware::DX;

hdx::InputLayout::InputLayout(VertexShader& vertexShader)
{
    // create input layout
    const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
        {"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
         D3D11_INPUT_PER_VERTEX_DATA, 0u}
    };

    auto pVShaderBlob = vertexShader.GetBlob();

    ThrowIfFailed(DXResource::GetDevice()->CreateInputLayout(
        inputElementDesc, ARRAYSIZE(inputElementDesc),
        pVShaderBlob->GetBufferPointer(), pVShaderBlob->GetBufferSize(),
        &m_pInputLayout));
}

void hdx::InputLayout::Bind() noexcept
{
    DXResource::GetContext()->IASetInputLayout(m_pInputLayout.Get());
}
