#include "Renderer.h"
#include <sstream>
#include "dxerr.h"
#include "DXExceptionHelper.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

using namespace Hardware::DX;
using namespace Microsoft::WRL;

Renderer::Renderer(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC sd               = {0};
    sd.BufferDesc.Width                   = 0;
    sd.BufferDesc.Height                  = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.RefreshRate.Numerator   = 0;
    sd.BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count            = 1;
    sd.SampleDesc.Quality          = 0;
    sd.BufferUsage                 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount                 = 1;
    sd.OutputWindow                = hwnd;
    sd.Windowed                    = TRUE;
    sd.SwapEffect                  = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags                       = 0;

    UINT swapCreateFlags = 0u;
#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // create device and front/back buffers, and swap chain and rendering
    // context
    ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, swapCreateFlags, nullptr, 0,
        D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice, nullptr,
        &m_pContext));

    // gain access to texture subresource in swap chain (back buffer)
    ComPtr<ID3D11Resource> pResource;
    ThrowIfFailed(
        m_pSwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &pResource));
    ThrowIfFailed(m_pDevice->CreateRenderTargetView(pResource.Get(), nullptr,
                                                    &m_pRenderTargetView));
}
void Renderer::EndFrame() { ThrowIfFailed(m_pSwapChain->Present(1u, 0u)); }
void Renderer::ClearBuffer(float r, float g, float b) noexcept
{
    const float color[] = {r, g, b, 1.0f};
    m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
}
void Hardware::DX::Renderer::DrawTest()
{
    struct Vertex {
        float x;
        float y;
    };

    const Vertex vertices[] = {
        {0.0f,  0.5f },
        {0.5f,  -0.5f},
        {-0.5f, -0.5f}
    };

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

    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    m_pContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);

    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

    // create a vertex shader
    ComPtr<ID3D11VertexShader> pVertexShader;
    ComPtr<ID3DBlob> pBlob;
    ThrowIfFailed(D3DReadFileToBlob(L"Shaders/VertexShader.cso", &pBlob));
    ThrowIfFailed(m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

    // bind a vertex shader
    m_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

    ThrowIfInfoGot(m_pContext->Draw(3u, 0u));
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