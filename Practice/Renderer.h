#pragma once
#include "WindowsHeader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <vector>
#include <string>
#include "BaseException.h"
#include "DXResource.h"
#include "Camera.h"

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
    void DrawTestSurface(const World::Object::Camera& camera, float x, float z,
                         float time);
};
}
