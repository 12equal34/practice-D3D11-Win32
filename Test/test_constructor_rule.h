#pragma once
#include <iostream>

struct A
{
	A() noexcept
	{
		std::cout << "default constuctor" << std::endl;
	}
	A(const A&) noexcept
	{
		std::cout << "copy constuctor" << std::endl;
	}
	A(A&&) noexcept
	{
		std::cout << "move constuctor" << std::endl;
	}
	~A()
	{
		std::cout << "distuctor" << std::endl;
	}
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

	A Do() noexcept
	{
		auto result = *this;
		result.OtherDo();
		return result;
	}

	void OtherDo() noexcept
	{
	}
};