#pragma once
#include <iostream>

struct A {
    int a = 0;
    int b = 0;

    A(int a, int b)
        : a(a),
          b(b)
    {
        std::cout << "A(int,int) constuctor" << std::endl;
    }
    A(const A& rhs)
        : a(rhs.a),
          b(rhs.b)
    {
        std::cout << "copy constuctor" << std::endl;
    }
    A(A&&) noexcept { std::cout << "move constuctor" << std::endl; }
    ~A() { std::cout << "A distuctor" << std::endl; }
    A& operator=(const A&) noexcept
    {
        std::cout << "copy assignment" << std::endl;
        return *this;
    }
    A& operator=(A&&) noexcept
    {
        std::cout << "move assignment" << std::endl;
        return *this;
    }
};

struct B {

    int a, b;
        operator A() { return {a, b}; }

    B(int a, int b)
        : a(a),
          b(b)
    {
        std::cout << "B(int,int) constuctor" << std::endl;
    }
    ~B() { std::cout << "B distuctor" << std::endl; }


    void Do(const A& rhs) { std::cout << "Do" << std::endl; }
};

void test()
{
    B v {1, 2};
    B w(3, 4);
    A m(3, 4);

    w.Do(v);
    w.Do(m);
}