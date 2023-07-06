#include <iostream>
#include <chrono>
#include <DirectXMath.h>
#include <vector>

namespace which_one_is_more_efficient
{
using namespace DirectX;

struct Coord3D {
    XMFLOAT3 pos_;

    void add(const XMFLOAT3& rhs) noexcept
    {
        pos_.x += rhs.x;
        pos_.y += rhs.y;
        pos_.z += rhs.z;
    }

    void add_with_XMVector(const XMFLOAT3& rhs) noexcept
    {
        auto a      = XMLoadFloat3(&pos_);
        auto b      = XMLoadFloat3(&rhs);
        auto result = XMVectorAdd(a, b);
        XMStoreFloat3(&pos_, result);
    }
};
struct Coord4D
{
    XMFLOAT4 pos_;

    void add(const XMFLOAT4& rhs) noexcept
    {
        pos_.x += rhs.x;
        pos_.y += rhs.y;
        pos_.z += rhs.z;
        pos_.w += rhs.w;
    }

    void add_with_XMVector(const XMFLOAT4& rhs) noexcept
    {
        auto a = XMLoadFloat4(&pos_);
        auto b = XMLoadFloat4(&rhs);
        auto result = XMVectorAdd(a, b);
        XMStoreFloat4(&pos_, result);
    }
};

void test3D()
{
    std::vector<XMFLOAT3> coords;
    coords.reserve(20);
    for (int i = 0; i < coords.capacity(); ++i) {
        coords.emplace_back((float)i, (float)i, (float)i);
    }

    Coord3D a;
    auto  clock = std::chrono::system_clock {};

    // 1. add
    std::cout << "add ���� : ";
    auto start = clock.now();
    for (auto& pos : coords) {
        a.add(pos);
    }
    auto duration = clock.now() - start;
    std::cout << duration << std::endl;

    // 2. add with XMVector
    std::cout << "add_XMVector���� : ";
    start = clock.now();
    for (auto& pos : coords) {
        a.add_with_XMVector(pos);
    }
    duration = clock.now() - start;
    std::cout << duration << std::endl;
}

void test4D()
{
    std::vector<XMFLOAT4> coords;
    coords.reserve(20);
    for (int i = 0; i < coords.capacity(); ++i) {
        coords.emplace_back((float) i, (float) i, (float) i, (float) i);
    }

    Coord4D a;
    auto  clock = std::chrono::system_clock {};

    // 1. add
    std::cout << "add ���� : ";
    auto start = clock.now();
    for (auto& pos : coords) {
        a.add(pos);
    }
    auto duration = clock.now() - start;
    std::cout << duration << std::endl;

    // 2. add with XMVector
    std::cout << "add_XMVector���� : ";
    start = clock.now();
    for (auto& pos : coords) {
        a.add_with_XMVector(pos);
    }
    duration = clock.now() - start;
    std::cout << duration << std::endl;
}

// ��� : �ܼ��� 4���� ������ �ϱ� ���ؼ� XMVector�� ����ϴ� ����
// ������尡 �� ũ�Ƿ�, ���� ������ ��������.
}

//int main() 
//{ 
//    which_one_is_more_efficient::test3D(); 
//    which_one_is_more_efficient::test4D(); 
//}