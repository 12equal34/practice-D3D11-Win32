#include "Topology.h"
#include "DXExceptionMacro.h"

namespace hdx = Hardware::DX;

hdx::Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY topology)
    : m_topology(topology)
{ }

void hdx::Topology::Bind() noexcept
{
    // set primitive topology
    DXResource::GetContext()->IASetPrimitiveTopology(m_topology);
}
