#include "Viewport.h"
#include "DXExceptionMacro.h"

namespace HDX = Hardware::DX;

HDX::Viewport::Viewport(FLOAT width, FLOAT height)
{
    // configure a viewport
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_viewport.Width    = width;
    m_viewport.Height   = height;
    m_viewport.MinDepth = 0;
    m_viewport.MaxDepth = 1;
}

void HDX::Viewport::Bind() noexcept
{
    DXResource::GetContext()->RSSetViewports(1u, &m_viewport);
}
