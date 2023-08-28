#pragma once

namespace gp::math
{
class NumericUtilityFunc
{
public:
    // returns "x mod (b-a) in [a,b)"
    static void map_on_range(float& x, float a, float b) noexcept
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
};
}
