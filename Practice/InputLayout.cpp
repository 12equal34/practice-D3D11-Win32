#include "InputLayout.h"
#include "DXExceptionMacro.h"
#include "ConstantBuffers.h"

namespace hdx = Hardware::DX;

hdx::InputLayout::InputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDesc,
                              UINT numElements, VertexShader& vertexShader)
{
    auto pVSShaderBlob = vertexShader.GetBlob();

    ThrowIfFailed(DXResource::GetDevice()->CreateInputLayout(
        pInputElementDesc, numElements, pVSShaderBlob->GetBufferPointer(),
        pVSShaderBlob->GetBufferSize(), &m_pInputLayout));
}

void hdx::InputLayout::Bind()
{
    DXResource::GetContext()->IASetInputLayout(m_pInputLayout.Get());
}
