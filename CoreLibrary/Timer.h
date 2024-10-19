#pragma once

#include "Windows.h"

namespace Windows
{
class Timer
{
public:
	Timer();
public:
	void Reset();
	float ElapsedSeconds() const;
	float ElapsedMiliseconds() const;
private:
	LARGE_INTEGER _frequency;
	LARGE_INTEGER _startTimeCount;
};
}
