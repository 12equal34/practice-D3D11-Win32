#include "ConstantBuffer.h"
#include <DirectXMath.h>
#include "DXExceptionMacro.h"

using namespace Hardware::DX;
using namespace DirectX;

ConstantBuffer::ConstantBuffer(Renderer& renderer, float x, float z)
{
    // struct ConstantBuffer1 {
    //     XMMATRIX transform;
    // };
    // const ConstantBuffer1 cbuf = {XMMatrixTranspose(
    //     /* clang-format off */
    //     XMMatrixRotationZ(angle) *
    //     XMMatrixRotationX(angle) *
    //     XMMatrixTranslation(x, 0.0f, z + 5.0f) *
    //     XMMatrixPerspectiveLH(1.0f, 0.6f, 0.5f, 10.0f)
    //     /* clang-format on */
    //     )};

    //// create constant buffer 1
    // D3D11_BUFFER_DESC cbd      = {};
    // cbd.Usage                  = D3D11_USAGE_DYNAMIC;
    // cbd.BindFlags              = D3D11_BIND_CONSTANT_BUFFER;
    // cbd.CPUAccessFlags         = D3D11_CPU_ACCESS_WRITE;
    // cbd.MiscFlags              = 0u;
    // cbd.ByteWidth              = sizeof(cbuf);
    // cbd.StructureByteStride    = 0u;
    // D3D11_SUBRESOURCE_DATA csd = {};
    // csd.pSysMem                = &cbuf;
    // ThrowIfFailed(
    //     GetDevice(renderer)->CreateBuffer(&cbd, &csd, &m_pConstantBuffer1));

    struct ConstantBuffer {
        struct {
            float r;
            float g;
            float b;
            float a;
        } faceColor[6];
    };
    const ConstantBuffer cbuf = {
        {
         {1.0f, 0.0f, 1.0f, 1.0f},
         {1.0f, 0.0f, 0.0f, 1.0f},
         }
    };

    // create constant buffer 2
    D3D11_BUFFER_DESC cbd      = {};
    cbd.Usage                  = D3D11_USAGE_DEFAULT;
    cbd.BindFlags              = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags         = 0u;
    cbd.MiscFlags              = 0u;
    cbd.ByteWidth              = sizeof(cbuf);
    cbd.StructureByteStride    = 0u;
    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem                = &cbuf;
    ThrowIfFailed(
        GetDevice(renderer)->CreateBuffer(&cbd, &csd, &m_pConstantBuffer));
}

void ConstantBuffer::Bind(Renderer& renderer) noexcept
{
    // bind constant buffer to vertex shader
    GetContext(renderer)->VSSetConstantBuffers(
        0u, 1u, m_pConstantBuffer.GetAddressOf());
}
