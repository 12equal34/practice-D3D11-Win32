#pragma once
#include "Bindable.h"

namespace Hardware::DX
{
class Topology : public Bindable
{
public:
    Topology(D3D11_PRIMITIVE_TOPOLOGY topology);
    void Bind(Renderer& renderer) noexcept override;
private:
    D3D11_PRIMITIVE_TOPOLOGY m_topology;
};

}