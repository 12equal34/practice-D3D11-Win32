#include "Mouse.h"
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

bool Mouse::IsEmpty() const noexcept
{
	return m_eventBuffer.empty();
}

void Mouse::Flush() noexcept
{
	m_eventBuffer = std::queue<Event>();
}

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
	m_pos = { xpos, ypos };
	m_eventBuffer.push(Event(Event::Type::Move, m_pos));
	TrimBuffer(m_eventBuffer);
}

void Hardware::Mouse::OnMouseWheel(int delta) noexcept
{
	m_wheelAmount += delta;
	if (m_wheelAmount >= WHEEL_DELTA) {
		m_wheelAmount -= WHEEL_DELTA;
		OnMouseWheelUp();
	}
	else if(m_wheelAmount <= -WHEEL_DELTA)
	{
		m_wheelAmount -= WHEEL_DELTA;
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

void Hardware::Mouse::OnMouseEnter() noexcept
{
	m_isInWindow = true;
	m_eventBuffer.push(Event(Event::Type::Enter, m_pos));
	TrimBuffer(m_eventBuffer);
}

void Hardware::Mouse::OnMouseLeave() noexcept
{
	m_isInWindow = false;
	m_eventBuffer.push(Event(Event::Type::Leave, m_pos));
	TrimBuffer(m_eventBuffer);
}
