#pragma once
#include <queue>
#include <optional>
#include <map>

namespace Hardware
{
class Mouse
{
    friend class Window;

public:
    class Event
    {
    public:
        enum class Type {
            Invalid,
            LPress,
            LRelease,
            RPress,
            RRelease,
            WheelUp,
            WheelDown,
            WheelLeft,
            WheelRight,
            Move,
            Enter,
            Leave,
        };

    public:
        Event(Type                type = Type::Invalid,
              std::pair<int, int> pos  = {0, 0}) noexcept
            : m_type(type),
              m_pos(pos)
        { }
        int                 GetX() const noexcept { return m_pos.first; }
        int                 GetY() const noexcept { return m_pos.second; }
        std::pair<int, int> GetXY() const noexcept { return m_pos; }
        Type                GetType() const noexcept { return m_type; }

        bool IsInvalid() const noexcept { return m_type == Type::Invalid; }
        bool IsLPress() const noexcept { return m_type == Type::LPress; }
        bool IsLRelease() const noexcept { return m_type == Type::LRelease; }
        bool IsRPress() const noexcept { return m_type == Type::RPress; }
        bool IsRRelease() const noexcept { return m_type == Type::RRelease; }
        bool IsWheelUp() const noexcept { return m_type == Type::WheelUp; }
        bool IsWheelDown() const noexcept { return m_type == Type::WheelDown; }
        bool IsMove() const noexcept { return m_type == Type::Move; }
        bool IsEnter() const noexcept { return m_type == Type::Enter; }
        bool IsLeave() const noexcept { return m_type == Type::Leave; }

    private:
        Type                m_type;
        std::pair<int, int> m_pos;
    };

public:
    Mouse(Window* owner) noexcept;
    ~Mouse()                       = default;
    Mouse(const Mouse&)            = delete;
    Mouse& operator=(const Mouse&) = delete;

    int                     GetScreenX() const noexcept { return m_pos.first; }
    int                     GetScreenY() const noexcept { return m_pos.second; }
    std::pair<int, int>     GetScreenXY() const noexcept { return m_pos; }
    float                   GetNormalizedX() const noexcept;
    float                   GetNormalizedY() const noexcept;
    std::pair<float, float> GetNormalizedXY() const noexcept;

    bool LeftIsPressed() const noexcept { return m_LPressed; }
    bool RightIsPressed() const noexcept { return m_RPressed; }
    bool IsInWindow() const noexcept { return m_isInWindow; }

    std::optional<Mouse::Event>        Read() noexcept;
    std::optional<std::pair<int, int>> ReadDelta() noexcept;

    bool IsEmpty() const noexcept;
    void Flush() noexcept;

private:
    void OnLeftButtonDown() noexcept;
    void OnLeftButtonUp() noexcept;
    void OnRightButtonDown() noexcept;
    void OnRightButtonUp() noexcept;
    void OnMouseMove(int xpos, int ypos) noexcept;
    void OnMouseMoveDelta(int dxpos, int dypos) noexcept;
    void OnMouseWheel(int delta) noexcept;
    void OnMouseWheelUp() noexcept;
    void OnMouseWheelDown() noexcept;
    void OnMouseWheelH(int delta) noexcept;
    void OnMouseWheelLeft() noexcept;
    void OnMouseWheelRight() noexcept;
    void OnMouseEnter() noexcept;
    void OnMouseLeave() noexcept;

private:
    Window*                         m_pOwner;
    std::pair<int, int>             m_pos;
    std::queue<Event>               m_eventBuffer;
    std::queue<std::pair<int, int>> m_deltaBuffer;

    bool m_LPressed   = false;
    bool m_RPressed   = false;
    bool m_isInWindow = false;

    int m_wheelAmount  = 0;
    int m_wheelAmountH = 0;
};
}
