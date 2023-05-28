#include "Renderer.h"
#include <sstream>
#include <DirectXMath.h>
#include "dxerr.h"
#include "DXExceptionHelper.h"
#include "WinExceptionHelper.h"
#include "Window.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

using namespace Hardware;
using namespace Hardware::DX;
using namespace Microsoft::WRL;
using namespace DirectX;

Renderer::Renderer(HWND hwnd)
    : m_hwnd(hwnd)
{
    DXGI_SWAP_CHAIN_DESC scDesc               = {0};
    scDesc.BufferDesc.Width                   = 0;
    scDesc.BufferDesc.Height                  = 0;
    scDesc.BufferDesc.RefreshRate.Denominator = 0;
    scDesc.BufferDesc.RefreshRate.Numerator   = 0;
    scDesc.BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM;
    scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scDesc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;
    scDesc.SampleDesc.Count            = 1;
    scDesc.SampleDesc.Quality          = 0;
    scDesc.BufferUsage                 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.BufferCount                 = 1;
    scDesc.OutputWindow                = m_hwnd;
    scDesc.Windowed                    = TRUE;
    scDesc.SwapEffect                  = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.Flags                       = 0;

    UINT swapCreateFlags = 0u;
#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // create device and front/back buffers, and swap chain and rendering
    // context
    ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, swapCreateFlags, nullptr, 0,
        D3D11_SDK_VERSION, &scDesc, &m_pSwapChain, &m_pDevice, nullptr,
        &m_pContext));

    // gain access to texture subresource in swap chain (back buffer)
    ComPtr<ID3D11Resource> pBackBuffer;
    ThrowIfFailed(
        m_pSwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &pBackBuffer));
    ThrowIfFailed(
        m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pRTV));

    // create depth stencil desc
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable              = TRUE;
    dsDesc.DepthFunc                = D3D11_COMPARISON_LESS;
    dsDesc.DepthWriteMask           = D3D11_DEPTH_WRITE_MASK_ALL;
    ComPtr<ID3D11DepthStencilState> pDSstate;
    ThrowIfFailed(m_pDevice->CreateDepthStencilState(&dsDesc, &pDSstate));

    // bind depth stencil state
    m_pContext->OMSetDepthStencilState(pDSstate.Get(), 1u);

    // create depth stencil texture
    ComPtr<ID3D11Texture2D> pDepthStencil;
    D3D11_TEXTURE2D_DESC    txDesc = {};
    auto [crW, crH]                = GetClientRegionSize();
    txDesc.Width                   = crW;
    txDesc.Height                  = crH;
    txDesc.ArraySize               = 1u;
    txDesc.MipLevels               = 1u;
    txDesc.SampleDesc.Count        = 1u;
    txDesc.SampleDesc.Quality      = 0u;
    txDesc.CPUAccessFlags          = 0u;
    txDesc.Usage                   = D3D11_USAGE_DEFAULT;
    txDesc.Format                  = DXGI_FORMAT_D32_FLOAT;
    txDesc.BindFlags               = D3D11_BIND_DEPTH_STENCIL;
    ThrowIfFailed(m_pDevice->CreateTexture2D(&txDesc, nullptr, &pDepthStencil));

    // create view of depth stencil texture
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0u;
    ThrowIfFailed(m_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvDesc, &m_pDSV));

    // bind depth stencil view to OM
    m_pContext->OMSetRenderTargets(1u, m_pRTV.GetAddressOf(), m_pDSV.Get());
}
void Renderer::EndFrame() { ThrowIfFailed(m_pSwapChain->Present(0u, 0u)); }
void Renderer::ClearBuffer(float r, float g, float b) noexcept
{
    const float color[] = {r, g, b, 1.0f};
    m_pContext->ClearRenderTargetView(m_pRTV.Get(), color);
    m_pContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}
void Hardware::DX::Renderer::DrawTest(float angle, float x, float z)
{
    struct Vertex {
        struct {
            float x;
            float y;
            float z;
        } pos;
    };
    const Vertex vertices[] = {
        {-1.0f, -1.0f, -1.0f},
        {1.0f,  -1.0f, -1.0f},
        {-1.0f, 1.0f,  -1.0f},
        {1.0f,  1.0f,  -1.0f},
        {-1.0f, -1.0f, 1.0f },
        {1.0f,  -1.0f, 1.0f },
        {-1.0f, 1.0f,  1.0f },
        {1.0f,  1.0f,  1.0f },
    };

    const unsigned short indices[] = {
        /* clang-format off */
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4,
        /* clang-format on */
    };

    struct ConstantBuffer1 {
        XMMATRIX transform;
    };
    const ConstantBuffer1 cbuf1 = {XMMatrixTranspose(
        /* clang-format off */
        XMMatrixRotationZ(angle) * 
        XMMatrixRotationX(angle) *
        XMMatrixTranslation(x, 0.0f, z + 5.0f) *
        XMMatrixPerspectiveLH(1.0f, 0.6f, 0.5f, 10.0f)
        /* clang-format on */
        )};

    struct ConstantBuffer2 {
        struct {
            float r;
            float g;
            float b;
            float a;
        } faceColor[6];
    };
    const ConstantBuffer2 cbuf2 = {
        {
         {1.0f, 0.0f, 1.0f, 1.0f},
         {1.0f, 0.0f, 0.0f, 1.0f},
         {0.0f, 1.0f, 0.0f, 1.0f},
         {0.0f, 0.0f, 1.0f, 1.0f},
         {1.0f, 1.0f, 0.0f, 1.0f},
         {0.0f, 1.0f, 1.0f, 1.0f},
         }
    };

    // create vertex buffer
    ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC    bd   = {};
    bd.Usage                  = D3D11_USAGE_DEFAULT;
    bd.BindFlags              = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags         = 0u;
    bd.MiscFlags              = 0u;
    bd.ByteWidth              = sizeof(vertices);
    bd.StructureByteStride    = sizeof(Vertex);
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem                = vertices;
    ThrowIfFailed(m_pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));
    // bind vertex buffer
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    m_pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(),
                                   &stride, &offset);

    // create index buffer
    ComPtr<ID3D11Buffer> pIndexBuffer;
    bd                     = {};
    bd.Usage               = D3D11_USAGE_DEFAULT;
    bd.BindFlags           = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags      = 0u;
    bd.MiscFlags           = 0u;
    bd.ByteWidth           = sizeof(indices);
    bd.StructureByteStride = sizeof(unsigned short);
    sd                     = {};
    sd.pSysMem             = indices;
    ThrowIfFailed(m_pDevice->CreateBuffer(&bd, &sd, &pIndexBuffer));
    // bind index buffer
    m_pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

    // set primitive topology
    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // for a vertex/pixel shader
    ComPtr<ID3DBlob> pBlob;

    // create a vertex shader
    ComPtr<ID3D11VertexShader> pVertexShader;
    ThrowIfFailed(D3DReadFileToBlob(L"Shaders/VertexShader.cso", &pBlob));
    ThrowIfFailed(m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(),
                                                pBlob->GetBufferSize(), nullptr,
                                                &pVertexShader));
    // bind a vertex shader
    m_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

    // create constant buffer 1
    ComPtr<ID3D11Buffer> pConstantBuffer1;
    D3D11_BUFFER_DESC    cbd   = {};
    cbd.Usage                  = D3D11_USAGE_DYNAMIC;
    cbd.BindFlags              = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags         = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags              = 0u;
    cbd.ByteWidth              = sizeof(cbuf1);
    cbd.StructureByteStride    = 0u;
    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem                = &cbuf1;
    ThrowIfFailed(m_pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer1));
    // bind constant buffer to vertex shader
    m_pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer1.GetAddressOf());

    // create constant buffer 2
    ComPtr<ID3D11Buffer> pConstantBuffer2;
    cbd                     = {};
    cbd.Usage               = D3D11_USAGE_DEFAULT;
    cbd.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags      = 0u;
    cbd.MiscFlags           = 0u;
    cbd.ByteWidth           = sizeof(cbuf2);
    cbd.StructureByteStride = 0u;
    csd                     = {};
    csd.pSysMem             = &cbuf2;
    ThrowIfFailed(m_pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer2));
    // bind constant buffer to pixel shader
    m_pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

    // create input layout
    ComPtr<ID3D11InputLayout>      pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
        {"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u,
         D3D11_INPUT_PER_VERTEX_DATA, 0u}
    };
    ThrowIfFailed(m_pDevice->CreateInputLayout(
        inputElementDesc, ARRAYSIZE(inputElementDesc),
        pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
    m_pContext->IASetInputLayout(pInputLayout.Get());

    // create a pixel shader
    ComPtr<ID3D11PixelShader> pPixelShader;
    ThrowIfFailed(D3DReadFileToBlob(L"Shaders/PixelShader.cso", &pBlob));
    ThrowIfFailed(m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(),
                                               pBlob->GetBufferSize(), nullptr,
                                               &pPixelShader));
    // bind a pixel shader
    m_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

    // configure a viewport
    auto [crW, crH]   = GetClientRegionSize();
    D3D11_VIEWPORT vp = {0};
    vp.TopLeftX       = 0;
    vp.TopLeftY       = 0;
    vp.Width          = static_cast<FLOAT>(crW);
    vp.Height         = static_cast<FLOAT>(crH);
    vp.MinDepth       = 0;
    vp.MaxDepth       = 1;
    m_pContext->RSSetViewports(1u, &vp);

    ThrowIfInfoGot(m_pContext->DrawIndexed(ARRAYSIZE(indices), 0u, 0u));
}
std::pair<LONG, LONG> Hardware::DX::Renderer::GetClientRegionSize() const
{
    RECT cr = {};
    Win::ThrowIfNull(GetClientRect(m_hwnd, &cr));
    return std::pair<LONG, LONG>(cr.right, cr.bottom);
}
//-----------------------------------------------------------------------------
// Exceptions
//-----------------------------------------------------------------------------
Renderer::InfoException::InfoException(
    int line, const char* file,
    const std::vector<std::string>& infoMsgs) noexcept
    : Exception(line, file)
{
    for (const auto& msg : infoMsgs) {
        m_info += msg;
        m_info.push_back('\n');
    }
    if (!infoMsgs.empty()) {
        m_info.pop_back();
    }
}
const char* Renderer::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << "\n[ErrorInfo] " << GetErrorInfo() << "\n\n";
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}
const char* Renderer::InfoException::GetType() const noexcept
{
    return "Renderer::InfoException";
}
std::string Renderer::InfoException::GetErrorInfo() const noexcept
{
    return m_info;
}
//-----------------------------------------------------------------------------
Renderer::HrException::HrException(int line, const char* file, HRESULT hr,
                                   std::vector<std::string> infoMsgs) noexcept
    : Exception(line, file),
      m_hr(hr)
{
    for (const auto& msg : infoMsgs) {
        m_info += msg;
        m_info.push_back('\n');
    }
    if (!infoMsgs.empty()) {
        m_info.pop_back();
    }
}
const char* Renderer::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << "\n[ErrorCode] 0x" << std::hex << std::uppercase
        << GetErrorCode() << std::dec << " (" << (unsigned long)GetErrorCode()
        << ")\n"
        << "[ErrorString] " << GetErrorString() << '\n'
        << "[Description] " << GetErrorDescription() << '\n';
    if (!m_info.empty()) {
        oss << "[ErrorInfo] " << GetErrorInfo() << "\n\n";
    }
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}
const char* Renderer::HrException::GetType() const noexcept
{
    return "Renderer::HrException";
}
HRESULT Renderer::HrException::GetErrorCode() const noexcept { return m_hr; }
std::string Renderer::HrException::GetErrorString() const noexcept
{
    return DXGetErrorStringA(m_hr);
}
std::string Renderer::HrException::GetErrorDescription() const noexcept
{
    char buf[512];
    DXGetErrorDescriptionA(m_hr, buf, sizeof(buf));
    return buf;
}
std::string Renderer::HrException::GetErrorInfo() const noexcept
{
    return m_info;
}
//-----------------------------------------------------------------------------
const char* Renderer::DeviceRemovedException::GetType() const noexcept
{
    return "Renderer::DeviceRemovedException";
}
//-----------------------------------------------------------------------------