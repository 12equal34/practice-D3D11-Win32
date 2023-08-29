#pragma once
#include "WindowsHeader.h"
#include "Camera.h"
#include "Object.h"
#include "WaterSurface.h"

namespace Hardware::DX
{
class Renderer
{
public:
    Renderer(HWND hwnd);
    ~Renderer()                          = default;
    Renderer(const Renderer&)            = delete;
    Renderer& operator=(const Renderer&) = delete;

    void EndFrame();
    void ClearBuffer(float r, float g, float b) noexcept;
    void DrawObjects(
        const World::Object::Camera&                               camera,
        const std::vector<std::unique_ptr<World::Object::Object>>& objects);
    void DrawDynamicObjects(
        const World::Object::Camera& camera,
        const std::vector<std::unique_ptr<World::Object::WaterSurface>>&
            objects);
    void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);

    D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const noexcept;

private:
    D3D11_PRIMITIVE_TOPOLOGY m_topology;
};
}
