#pragma once
#include "Bindable.h"

namespace Hardware::DX
{
class IndexBuffer : public Bindable
{
public:
    IndexBuffer(Renderer& renderer);

    void Bind(Renderer& renderer) noexcept override;
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
public:
    UINT                                 m_numIndices;
};
}
