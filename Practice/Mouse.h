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
            bool IsLRelease() const noexcept
            {
                return m_type == Type::LRelease;
            }
            bool IsRPress() const noexcept { return m_type == Type::RPress; }
            bool IsRRelease() const noexcept
            {
                return m_type == Type::RRelease;
            }
            bool IsWheelUp() const noexcept { return m_type == Type::WheelUp; }
            bool IsWheelDown() const noexcept
            {
                return m_type == Type::WheelDown;
            }
            bool IsMove() const noexcept { return m_type == Type::Move; }
            bool IsEnter() const noexcept { return m_type == Type::Enter; }
            bool IsLeave() const noexcept { return m_type == Type::Leave; }

        private:
            Type                m_type;
            std::pair<int, int> m_pos;
        };

    public:
        Mouse() noexcept                     = default;
        ~Mouse()                             = default;
        Mouse(const Mouse&)                  = delete;
        const Mouse& operator=(const Mouse&) = delete;

        int                 GetX() const noexcept { return m_pos.first; }
        int                 GetY() const noexcept { return m_pos.second; }
        std::pair<int, int> GetXY() const noexcept { return m_pos; }

        bool LeftIsPressed() const noexcept { return LPressed; }
        bool RightIsPressed() const noexcept { return RPressed; }
        bool IsInWindow() const noexcept { return isInWindow; }

        std::optional<Mouse::Event> Read() noexcept;
        bool                        IsEmpty() const noexcept;
        void                        Flush() noexcept;

    private:
        void OnLeftButtonDown() noexcept;
        void OnLeftButtonUp() noexcept;
        void OnRightButtonDown() noexcept;
        void OnRightButtonUp() noexcept;
        void OnMouseMove(int xpos, int ypos) noexcept;
        void OnMouseWheelUp() noexcept;
        void OnMouseWheelDown() noexcept;

    private:
        std::pair<int, int> m_pos;
        std::queue<Event>   m_eventBuffer;

        bool LPressed   = false;
        bool RPressed   = false;
        bool isInWindow = false;
    };
}
