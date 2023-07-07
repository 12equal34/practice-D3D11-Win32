#include "Topology.h"
#include "DXExceptionMacro.h"

namespace HDX = Hardware::DX;

HDX::Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY topology)
    : m_topology(topology)
{ }

void HDX::Topology::Bind() noexcept
{
    // set primitive topology
    DXResource::GetContext()->IASetPrimitiveTopology(m_topology);
}
