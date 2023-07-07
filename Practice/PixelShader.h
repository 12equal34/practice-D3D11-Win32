#pragma once
#include <string>
#include "IBindable.h"
#include "DXResource.h"

namespace Hardware::DX
{
class PixelShader : public IBindable
{
public:
    PixelShader(std::wstring_view wsv);
    void        Bind() noexcept override;
    ID3D10Blob* GetBlob() noexcept;
private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
    Microsoft::WRL::ComPtr<ID3D10Blob>        m_pBlob;
};
}