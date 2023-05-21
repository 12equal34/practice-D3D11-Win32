#include "Timer.h"

using namespace std::chrono;

Timer::Timer() noexcept
    : m_last(steady_clock::now())
{ }

double Timer::Peek() const noexcept
{
    duration<double> frameTime = steady_clock::now() - m_last;
    return frameTime.count();
}
double Timer::Mark() noexcept
{
    double frameTime = Peek();
    m_timeAmount += frameTime;
    if (IsStop()) m_stopTimeAmount += frameTime;
    m_last = steady_clock::now();
    return frameTime;
}
double Timer::Amount() const noexcept
{
    return m_timeAmount - m_stopTimeAmount;
}

void Timer::Stop() noexcept { m_isStop = true; }
bool Timer::IsStop() const noexcept { return m_isStop; }
void Timer::Resume() noexcept { m_isStop = false; }
void Timer::Restart() noexcept
{
    m_isStop         = false;
    m_timeAmount     = 0.0;
    m_stopTimeAmount = 0.0;
    m_last           = steady_clock::now();
}
