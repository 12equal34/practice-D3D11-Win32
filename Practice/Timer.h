#pragma once
#include <chrono>

class Timer
{
public:
    Timer() noexcept;
    float  Peek() const noexcept;
    void   Mark() noexcept;
    double Amount() const noexcept;

    void Stop() noexcept;
    bool IsStop() const noexcept;
    void Resume() noexcept;
    void Restart() noexcept;

private:
    std::chrono::time_point<std::chrono::steady_clock> m_last;

    double m_timeAmount     = 0.0;
    double m_stopTimeAmount = 0.0;
    bool   m_isStop         = false;
};
