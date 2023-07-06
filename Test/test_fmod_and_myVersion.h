#pragma once
#include <DirectXMath.h>
#include <cmath>
#include <vector>
#include <chrono>
#include <iostream>

namespace which_one_is_more_suitable_at_game_programming
{
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

void test()
{
    // 1. 데이터들이 0과 XM_2PI에 근처에 있을 때,
    // my version : 7[1/10000000]s
    // std::fmodf version : 42[1 / 10000000]s
    std::vector<float> vec {8.14f, -1.0f, 9.0f,
     -5.0f, 12.0f,  14.5f, 13.20f, 23.40f, 17.9f, -2.f, -4.f, -5.f};

    //2. 데이터들이 0과 XM_2PI와 멀리 있을 때,
    // my version : 56[1 / 10000000]s
    // std::fmodf version : 52[1 / 10000000]s
    // std::vector<float> vec {41.4f,  -10.f, 20.f, -50.f,  600.f,  4500.f,
                            // 3200.f, 340.f, 79.f, -200.f, -400.f, -300.f};

    auto vec2 {vec};

    auto clock = std::chrono::system_clock {};

    std::cout << "my version : ";
    auto start = clock.now();
    for (auto v : vec) {
        mod_2PI(v);
    }
    auto duration = clock.now() - start;
    std::cout << duration << std::endl;

    std::cout << "std::fmodf version : ";
    start = clock.now();
    for (auto& v : vec2) {
        v = std::fmodf(v, DirectX::XM_2PI);
    }
    duration = clock.now() - start;
    std::cout << duration << std::endl;
}
}
