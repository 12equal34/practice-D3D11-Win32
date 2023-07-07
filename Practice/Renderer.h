#pragma once
#include "WindowsHeader.h"

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
    void DrawTestSurface();
};
}
