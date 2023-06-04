#include "Mouse.h"
#include "Window.h"
#include "HardwareHelper.h"

#define WHEEL_DELTA 120

using namespace Hardware;
using namespace Hardware::Helper;

//-----------------------------------------------------------------------------
// event buffer
//-----------------------------------------------------------------------------
std::optional<Mouse::Event> Mouse::Read() noexcept
{
    if (!IsEmpty()) {
        Mouse::Event e = m_eventBuffer.front();
        m_eventBuffer.pop();
        return e;
    }
    return {};
}

std::optional<std::pair<int, int>> Mouse::ReadDelta() noexcept
{
    if (!m_deltaBuffer.empty()) {
        std::pair<int, int> xy = m_deltaBuffer.front();
        m_deltaBuffer.pop();
        return xy;
    }
    return {};
}

bool Mouse::IsEmpty() const noexcept { return m_eventBuffer.empty(); }

void Mouse::Flush() noexcept { m_eventBuffer = std::queue<Event>(); }

//-----------------------------------------------------------------------------
// windows message handler
//-----------------------------------------------------------------------------

void Mouse::OnLeftButtonDown() noexcept
{
    m_eventBuffer.push(Event(Event::Type::LPress));
    TrimBuffer(m_eventBuffer);
}

void Mouse::OnLeftButtonUp() noexcept
{
    m_eventBuffer.push(Event(Event::Type::LRelease, m_pos));
    TrimBuffer(m_eventBuffer);
}

void Mouse::OnRightButtonDown() noexcept
{
    m_eventBuffer.push(Event(Event::Type::RPress, m_pos));
    TrimBuffer(m_eventBuffer);
}

void Mouse::OnRightButtonUp() noexcept
{
    m_eventBuffer.push(Event(Event::Type::RRelease, m_pos));
    TrimBuffer(m_eventBuffer);
}

void Mouse::OnMouseMove(int xpos, int ypos) noexcept
{
    m_pos = {xpos, ypos};
    m_eventBuffer.push(Event(Event::Type::Move, m_pos));
    TrimBuffer(m_eventBuffer);
}

void Mouse::OnMouseMoveDelta(int dxpos, int dypos) noexcept
{
    m_deltaBuffer.push(std::pair<int, int>(dxpos, dypos));
    TrimBuffer(m_deltaBuffer);
}

void Mouse::OnMouseWheel(int delta) noexcept
{
    m_wheelAmount += delta;
    if (m_wheelAmount >= WHEEL_DELTA) {
        m_wheelAmount -= WHEEL_DELTA;
        OnMouseWheelUp();
    } else if (m_wheelAmount <= -WHEEL_DELTA) {
        m_wheelAmount += WHEEL_DELTA;
        OnMouseWheelDown();
    }
}

void Mouse::OnMouseWheelUp() noexcept
{
    m_eventBuffer.push(Event(Event::Type::WheelUp, m_pos));
    TrimBuffer(m_eventBuffer);
}

void Mouse::OnMouseWheelDown() noexcept
{
    m_eventBuffer.push(Event(Event::Type::WheelDown, m_pos));
    TrimBuffer(m_eventBuffer);
}

void Mouse::OnMouseWheelH(int delta) noexcept
{
    m_wheelAmountH += delta;
    if (m_wheelAmountH >= WHEEL_DELTA) {
        m_wheelAmountH -= WHEEL_DELTA;
        OnMouseWheelRight();
    } else if (m_wheelAmountH <= -WHEEL_DELTA) {
        m_wheelAmountH += WHEEL_DELTA;
        OnMouseWheelLeft();
    }
}

void Mouse::OnMouseWheelLeft() noexcept
{
    m_eventBuffer.push(Event(Event::Type::WheelLeft, m_pos));
    TrimBuffer(m_eventBuffer);
}

void Mouse::OnMouseWheelRight() noexcept
{
    m_eventBuffer.push(Event(Event::Type::WheelRight, m_pos));
    TrimBuffer(m_eventBuffer);
}

void Mouse::OnMouseEnter() noexcept
{
    m_isInWindow = true;
    m_eventBuffer.push(Event(Event::Type::Enter, m_pos));
    TrimBuffer(m_eventBuffer);
}

void Mouse::OnMouseLeave() noexcept
{
    m_isInWindow = false;
    m_eventBuffer.push(Event(Event::Type::Leave, m_pos));
    TrimBuffer(m_eventBuffer);
}
//-----------------------------------------------------------------------------
// Mouse Interface Definitions
//-----------------------------------------------------------------------------
Mouse::Mouse(Window* owner) noexcept
    : m_pOwner(owner)
{ }
float Mouse::GetNormalizedX() const noexcept
{
    float w = static_cast<float>(m_pOwner->GetWidth());
    return 2.0f * GetScreenX() / w - 1.0f;
}
float Mouse::GetNormalizedY() const noexcept
{
    float h = static_cast<float>(m_pOwner->GetHeight());
    return -2.0f * GetScreenY() / h + 1.0f;
}
std::pair<float, float> Mouse::GetNormalizedXY() const noexcept
{
    return std::pair<float, float>(GetNormalizedX(), GetNormalizedY());
}