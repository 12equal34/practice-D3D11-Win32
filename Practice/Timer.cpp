#include "Timer.h"

using namespace std::chrono;

Timer::Timer() noexcept
    : m_start(ClockType::now()),
      m_last(m_start)
{ }

double Timer::Peek() const noexcept
{
    duration<double> frameTime = ClockType::now() - m_last;
    return frameTime.count();
}
double Timer::Mark() noexcept
{
    double frameTime = Peek();
    m_time += frameTime;
    if (IsStop()) m_stopTime += frameTime;
    m_last = ClockType::now();
    return frameTime;
}
double Timer::Time() const noexcept { return m_time - m_stopTime; }

double Timer::TimeSinceStart() const noexcept
{
    duration<double> total = ClockType::now() - m_start;
    return total.count();
}

void Timer::Stop() noexcept { m_isStop = true; }
bool Timer::IsStop() const noexcept { return m_isStop; }
void Timer::Resume() noexcept { m_isStop = false; }
void Timer::Restart() noexcept
{
    m_isStop   = false;
    m_time     = 0.0;
    m_stopTime = 0.0;
    m_last     = ClockType::now();
}
