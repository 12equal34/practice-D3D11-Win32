#pragma once
#include "Bindable.h"

namespace Hardware::DX
{
class Viewport : public Bindable
{
public:
	Viewport(Renderer& renderer, FLOAT width, FLOAT height);
	void Bind(Renderer& renderer) noexcept override;
private:
	D3D11_VIEWPORT m_viewport;
};
}

