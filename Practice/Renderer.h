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
        float dt, const World::Object::Camera& camera,
        const std::vector<std::unique_ptr<World::Object::WaterSurface>>&
            objects);
};
}
