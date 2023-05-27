#pragma once
#include <queue>
#include <bitset>
#include <optional>

namespace Hardware
{
class Keyboard
{
    friend class Window;

public:
    class Event
    {
    public:
        enum class Type { Invalid, Press, Release };

    public:
        Event() noexcept
            : m_type(Type::Invalid),
              m_code(0u)
        { }
        Event(Type type, unsigned char code) noexcept
            : m_type(type),
              m_code(code)
        { }
        unsigned char GetCode() const noexcept { return m_code; }
        bool IsInvalid() const noexcept { return m_type == Type::Invalid; }
        bool IsPress() const noexcept { return m_type == Type::Press; }
        bool IsRelease() const noexcept { return m_type == Type::Release; }

    private:
        Type          m_type;
        unsigned char m_code;
    };

public:
    Keyboard(Window* owner) noexcept;
    ~Keyboard()                          = default;
    Keyboard(const Keyboard&)            = delete;
    Keyboard& operator=(const Keyboard&) = delete;

    void Flush() noexcept;

    // key event
    bool                 KeyIsPressed(unsigned char code) const noexcept;
    bool                 KeyIsEmpty() const noexcept;
    std::optional<Event> ReadKey() noexcept;
    void                 FlushKey() noexcept;

    // char event
    bool                CharIsEmpty() const noexcept;
    std::optional<char> ReadChar() noexcept;
    void                FlushChar() noexcept;

    // autorepeat event
    void EnableAutorepeat() noexcept;
    void DisableAutorepeat() noexcept;
    bool AutorepeatIsEnabled() const noexcept;

private:
    // windows message handler
    void OnKeyPressed(unsigned char code) noexcept;
    void OnKeyReleased(unsigned char code) noexcept;
    void OnChar(char c) noexcept;

    void ClearState() noexcept;

private:
    static constexpr unsigned int sKeySize = 256u;

    Window* m_pOwner;
    bool                  m_autorepeatedEnabled = false;
    std::bitset<sKeySize> m_keystates;
    std::queue<Event>     m_keybuffer;
    std::queue<char>      m_charbuffer;
};
}
