#include "InputLayout.h"
#include "DXExceptionMacro.h"

using namespace Hardware::DX;

InputLayout::InputLayout(Renderer& renderer, VertexShader& vertexShader)
{
    // create input layout
    const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
        {"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
         D3D11_INPUT_PER_VERTEX_DATA, 0u}
    };

    auto pVShaderBlob = vertexShader.GetBlob();

    ThrowIfFailed(GetDevice(renderer)->CreateInputLayout(
        inputElementDesc, ARRAYSIZE(inputElementDesc),
        pVShaderBlob->GetBufferPointer(), pVShaderBlob->GetBufferSize(),
        &m_pInputLayout));
}

void InputLayout::Bind(Renderer& renderer) noexcept
{
    GetContext(renderer)->IASetInputLayout(m_pInputLayout.Get());
}
