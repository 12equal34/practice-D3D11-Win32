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

double Windows::Timer::ElapsedSeconds() const
{
	LARGE_INTEGER timeCount;
	QueryPerformanceCounter(&timeCount);
	LONGLONG deltaTimeCount = timeCount.QuadPart - _startTimeCount.QuadPart;
	double elapsedSeconds = deltaTimeCount / static_cast<double>(_frequency.QuadPart);
	return elapsedSeconds;
}

double Windows::Timer::ElapsedMiliseconds() const
{
	return ElapsedSeconds() / 1000.0;
}