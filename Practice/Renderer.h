#pragma once
#include "WindowsHeader.h"
#include "Camera.h"
#include "Object.h"

namespace Hardware::DX
{
class Renderer
{
public:
    struct Transform {
        DirectX::XMMATRIX model;
        DirectX::XMMATRIX modelView;
        DirectX::XMMATRIX modelViewProj;
        DirectX::XMMATRIX modelRotation;
    };

    Renderer(HWND hwnd);
    ~Renderer()                          = default;
    Renderer(const Renderer&)            = delete;
    Renderer& operator=(const Renderer&) = delete;

    void EndFrame();
    void ClearBuffer(float r, float g, float b) noexcept;
    void DrawObjects(
        const World::Object::Camera&                               camera,
        const std::vector<std::unique_ptr<World::Object::Object>>& objects);
};
}
