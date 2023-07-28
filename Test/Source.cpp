// #include "test_add_with_XMVector.h"
// #include "test_fmod_and_myVersion.h"
// #include "test_casting_operator.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct A
{
    float x, y;
};
struct B
{
    float x, y;
};
struct C
{
    float x, y;
};

template <class L, class R, class Result>
Result Add(L&& a, R&& b)
{
    return Result {a.x + b.x, a.y + b.y};
}

int main()
{
    A a(1, 2);
    B b(1, 2);
    C c {Add(a, b)};
    
    cout << c.x << ' ' << c.y;
}