#pragma once
#include <DirectXMath.h>
#include "Coordinatable.h"
#include "IBindable.h"
#include "DXResource.h"
#include "ConstantBuffer.h"

namespace World::Object
{
class Camera : public Hardware::DX::IBindable, public Information::Coordinatable
{
public:
    Camera(float viewAspectRatio = 1.0f) noexcept;
    DirectX::XMMATRIX GetView() const noexcept;
    DirectX::XMMATRIX GetProjection() const noexcept;

    void SetViewport(FLOAT width, FLOAT height) noexcept;

    void Bind() noexcept override;
private:
    D3D11_VIEWPORT               m_viewport;
    float                        m_viewAspectRatio;
};
}
