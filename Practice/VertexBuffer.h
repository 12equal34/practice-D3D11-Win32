#pragma once
#include "Bindable.h"

namespace Hardware::DX
{
class VertexBuffer : public Bindable
{
public:
    VertexBuffer(Renderer& renderer);

    void Bind(Renderer& renderer) noexcept override;
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
    UINT m_stride;
};
}
