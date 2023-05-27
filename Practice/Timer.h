#pragma once
#include <chrono>

class Timer
{
public:
    Timer() noexcept;
    double Peek() const noexcept;
    double Mark() noexcept;
    double Time() const noexcept;
    double TimeSinceStart() const noexcept;

    void Stop() noexcept;
    bool IsStop() const noexcept;
    void Resume() noexcept;
    void Restart() noexcept;

private:
    using ClockType = std::chrono::steady_clock;

    ClockType::time_point m_start;
    ClockType::time_point m_last;

    double m_time      = 0.0;
    double m_stopTime  = 0.0;
    bool   m_isStop    = false;
};
