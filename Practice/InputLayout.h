#pragma once
#include "Bindable.h"
#include "VertexShader.h"

namespace Hardware::DX
{
class InputLayout : public Bindable
{
public:
    InputLayout(Renderer& renderer, VertexShader& vertexShader);
    void Bind(Renderer& renderer) noexcept override;
protected:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};
}
