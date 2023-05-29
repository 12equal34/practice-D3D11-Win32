#pragma once
#include "Bindable.h"

namespace Hardware::DX
{
class PixelShader : public Bindable
{
public:
    PixelShader(Renderer& renderer, std::wstring_view wsv);
    void        Bind(Renderer& renderer) noexcept override;
    ID3D10Blob* GetBlob() noexcept;
private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
    Microsoft::WRL::ComPtr<ID3D10Blob>        m_pBlob;
};
}