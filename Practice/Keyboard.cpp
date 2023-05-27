#include "Keyboard.h"
#include "Window.h"
#include "HardwareHelper.h"

using namespace Hardware;
using namespace Hardware::Helper;

//-----------------------------------------------------------------------------
// key event
//-----------------------------------------------------------------------------

bool Keyboard::KeyIsPressed(unsigned char code) const noexcept
{
    return m_keystates[code];
}

bool Keyboard::KeyIsEmpty() const noexcept { return m_keybuffer.empty(); }

std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept
{
    if (m_keybuffer.size() > 0) {
        Keyboard::Event event = m_keybuffer.front();
        m_keybuffer.pop();
        return event;
    }
    return {};
}

void Keyboard::FlushKey() noexcept
{
    m_keybuffer = std::queue<Keyboard::Event>();
}

//-----------------------------------------------------------------------------
// char event
//-----------------------------------------------------------------------------

bool Keyboard::CharIsEmpty() const noexcept { return m_charbuffer.empty(); }

std::optional<char> Keyboard::ReadChar() noexcept
{
    if (m_charbuffer.size() > 0) {
        char c = m_charbuffer.front();
        m_charbuffer.pop();
        return c;
    }
    return {};
}

void Keyboard::FlushChar() noexcept { m_charbuffer = std::queue<char>(); }

//-----------------------------------------------------------------------------
// autorepeat event
//-----------------------------------------------------------------------------
void Keyboard::EnableAutorepeat() noexcept { m_autorepeatedEnabled = true; }
void Keyboard::DisableAutorepeat() noexcept { m_autorepeatedEnabled = false; }
bool Keyboard::AutorepeatIsEnabled() const noexcept
{
    return m_autorepeatedEnabled;
}

//-----------------------------------------------------------------------------
// windows message handler
//-----------------------------------------------------------------------------
void Keyboard::OnKeyPressed(unsigned char code) noexcept
{
    m_keystates[code] = true;
    m_keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, code));
    TrimBuffer(m_keybuffer);
}

void Keyboard::OnKeyReleased(unsigned char code) noexcept {
    m_keystates[code] = false;
    m_keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, code));
    TrimBuffer(m_keybuffer);
}

void Keyboard::OnChar(char c) noexcept { 
    m_charbuffer.push(c);
    TrimBuffer(m_charbuffer);
}
//-----------------------------------------------------------------------------
void Keyboard::Flush() noexcept
{
    FlushKey();
    FlushChar();
}

void Keyboard::ClearState() noexcept { 
    m_keystates.reset();
}
//-----------------------------------------------------------------------------
// Keyboard Interface Definitions
//-----------------------------------------------------------------------------
Hardware::Keyboard::Keyboard(Window* owner) noexcept
    : m_pOwner(owner)
{
}
