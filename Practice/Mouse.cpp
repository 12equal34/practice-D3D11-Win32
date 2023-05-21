#include "Mouse.h"
#include "HardwareHelper.h"

using namespace Hardware;
using namespace Hardware::Helper;

//-----------------------------------------------------------------------------
// windows message handler
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
