#pragma once

namespace Hardware::DX
{
class IBindable
{
public:
    virtual void Bind() = 0;
    virtual ~IBindable()         = default;
};
}