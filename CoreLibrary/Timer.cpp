#include "pch.h"
#include "Timer.h"

Windows::Timer::Timer()
{
	QueryPerformanceFrequency(&_frequency);
	Reset();
}

void Windows::Timer::Reset()
{
	QueryPerformanceCounter(&_startTimeCount);
}

float Windows::Timer::ElapsedSeconds() const
{
	LARGE_INTEGER timeCount;
	QueryPerformanceCounter(&timeCount);
	LONGLONG deltaTimeCount = timeCount.QuadPart - _startTimeCount.QuadPart;
	float elapsedSeconds = deltaTimeCount / static_cast<float>(_frequency.QuadPart);
	return elapsedSeconds;
}

float Windows::Timer::ElapsedMiliseconds() const
{
	return ElapsedSeconds() / 1000.f;
}