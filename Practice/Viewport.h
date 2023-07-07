#pragma once
#include "IBindable.h"
#include "DXResource.h"

namespace Hardware::DX
{
class Viewport : public IBindable
{
public:
	Viewport(FLOAT width, FLOAT height);
	void Bind() noexcept override;
private:
	D3D11_VIEWPORT m_viewport;
};
}

