#pragma once
#include <DirectXMath.h>
#include "IBindable.h"
#include "DXResource.h"
#include "VertexShader.h"

struct PNTVertex {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 textureUV;
};

static const D3D11_INPUT_ELEMENT_DESC PNTVertexLayout[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
     D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
     D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24,
     D3D11_INPUT_PER_VERTEX_DATA, 0},
};

struct SimpleVertex {
    DirectX::XMFLOAT3 position;
};

static const D3D11_INPUT_ELEMENT_DESC SimpleVertexLayout[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
     D3D11_INPUT_PER_VERTEX_DATA, 0},
};

namespace Hardware::DX
{
class InputLayout : public IBindable
{
public:
    InputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDesc,
                UINT numElements, VertexShader& vertexShader);
    void Bind() override;
protected:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
};
}
