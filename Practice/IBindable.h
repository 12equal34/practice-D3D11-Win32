#pragma once
#include "Renderer.h"

namespace Hardware::DX
{
class IBindable
{
public:
    virtual void Bind() noexcept = 0;
    virtual ~IBindable()         = default;
};
}