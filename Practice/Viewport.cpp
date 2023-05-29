#include "Viewport.h"
#include "DXExceptionMacro.h"

using namespace Hardware::DX;
using namespace Microsoft::WRL;

Viewport::Viewport(Renderer& renderer, FLOAT width, FLOAT height)
{
    // configure a viewport
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_viewport.Width    = width;
    m_viewport.Height   = height;
    m_viewport.MinDepth = 0;
    m_viewport.MaxDepth = 1;
}

void Viewport::Bind(Renderer& renderer) noexcept
{
    GetContext(renderer)->RSSetViewports(1u, &m_viewport);
}
