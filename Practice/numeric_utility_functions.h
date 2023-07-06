#pragma once
#include <DirectXMath.h>

namespace gp
{
namespace math
{
    // returns "x mod 2pi"
    void mod_2PI(float& x) noexcept
    {
        using namespace DirectX;
        while (true) {
            if (x > XM_2PI) {
                x -= XM_2PI;
            } else if (x < 0) {
                x += XM_2PI;
            } else {
                return;
            }
        }
    }

    // returns "just x or x + dx in [xmin,xmax]"
    void add_or_leave(float& x, float dx, float xmin, float xmax)
    {
        x += dx;
        if (x < xmin || xmax < x) {
            x -= dx;
        }
    }
}
}
