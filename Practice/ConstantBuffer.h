#pragma once
#include "Bindable.h"

namespace Hardware::DX
{
class ConstantBuffer : public Bindable
{
public:
    ConstantBuffer(Renderer& renderer, float x, float z);
    void Bind(Renderer& renderer) noexcept override;
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};

}