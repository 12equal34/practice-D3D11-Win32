#pragma once
#include <DirectXMath.h>
#include "Coordinate.h"

namespace World::Object
{
class Camera
{
public:
    Camera(float viewAspectRatio = 1.0f);

    Information::Coordinate& GetCoordinate() noexcept;
    const Information::Coordinate& GetCoordinate() const noexcept;

    DirectX::XMMATRIX GetView() const noexcept;
    DirectX::XMMATRIX GetProjection() const noexcept;
private:
    Information::Coordinate m_coordinate;

    // clipping
    float m_viewAspectRatio;
};
}
