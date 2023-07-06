#pragma once
#include <DirectXMath.h>
#include "Coordinatable.h"

namespace World::Object
{
class Camera : public Coordinatable
{
public:
    Camera(float viewAspectRatio = 1.0f);
    DirectX::XMMATRIX GetView() const noexcept;
    DirectX::XMMATRIX GetProjection() const noexcept;
private:
    // clipping
    float m_viewAspectRatio;
};
}
