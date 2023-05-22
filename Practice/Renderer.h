#pragma once
#include <d3d11.h>
#include <wrl.h>

namespace Hardware
{
class Renderer
{
public:
    Renderer(HWND hwnd);
    ~Renderer()                          = default;
    Renderer(const Renderer&)            = delete;
    Renderer& operator=(const Renderer&) = delete;

    void EndFrame() noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D11Device>        m_pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain>      m_pSwapChain;
};
}
