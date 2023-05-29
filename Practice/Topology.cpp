#include "Topology.h"
#include "DXExceptionMacro.h"

using namespace Hardware::DX;

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY topology)
    : m_topology(topology)
{ }

void Topology::Bind(Renderer& renderer) noexcept
{
    // set primitive topology
    GetContext(renderer)->IASetPrimitiveTopology(m_topology);
}
