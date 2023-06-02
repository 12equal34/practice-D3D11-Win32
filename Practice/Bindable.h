#pragma once
#include "DXGettable.h"

namespace Hardware::DX
{
class Bindable : public DXGettable
{
public:
    virtual void Bind(Renderer& renderer) noexcept = 0;
    virtual ~Bindable()                            = default;
};
}