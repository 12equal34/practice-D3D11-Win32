#pragma once
#include "IBindable.h"
#include "DXResource.h"
#include "VertexShader.h"

namespace Hardware::DX
{
class InputLayout : public IBindable
{
public:
    InputLayout(VertexShader& vertexShader);
    void Bind() noexcept override;
protected:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};
}
