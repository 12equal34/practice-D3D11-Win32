#pragma once
#include "WindowsHeader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <vector>
#include <string>
#include "BaseException.h"
#include "DXResources.h"

namespace Hardware::DX
{
class Renderer
{
    friend class DXGettable;
public:
    Renderer(HWND hwnd);
    ~Renderer()                          = default;
    Renderer(const Renderer&)            = delete;
    Renderer& operator=(const Renderer&) = delete;

    void EndFrame();
    void ClearBuffer(float r, float g, float b) noexcept;
    // void DrawTest(float dt, float x, float y);
    void DrawTestSurface(float x, float z, float angle);

private:
    DXResources m_dx;
};
}
