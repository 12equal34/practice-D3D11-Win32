#pragma once
#include "IBindable.h"
#include "DXResource.h"

namespace Hardware::DX
{
class Topology : public IBindable
{
public:
    Topology(D3D11_PRIMITIVE_TOPOLOGY topology);
    void Bind() noexcept override;
private:
    D3D11_PRIMITIVE_TOPOLOGY m_topology;
};

}