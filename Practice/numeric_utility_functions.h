#pragma once
#include <DirectXMath.h>

namespace gp
{
namespace math
{
    // returns "x mod (b-a) in [a,b)"
    void map_on_range(float& x, float a, float b) noexcept
    {
        auto distance = b - a;
        while (true) {
            if (x > b) {
                x -= distance;
            } else if (x < a) {
                x += distance;
            } else {
                return;
            }
        }
    }
}
}
