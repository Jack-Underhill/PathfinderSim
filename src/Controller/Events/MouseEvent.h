#ifndef _MouseEvent_h_
#define _MouseEvent_h_

#include <sstream>

#include "Event.h"
#include "MouseCodes.h"

namespace PFSim {
    
    class MouseEvent : public Event
    {
    public:
        MouseCode getMouseCode() const { return m_MouseCode; }

        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        
        std::string toString() const override
        {
            std::stringstream ss;
            ss << getName() << ": " << m_MouseCode;
            return ss.str();
        }

        int getX() { return m_X; }
        int getY() { return m_Y; }

    protected:
        MouseEvent(const MouseCode MouseCode, int x, int y) : m_MouseCode(MouseCode), m_X(x), m_Y(y) {}
    
        MouseCode m_MouseCode;
        
    private:
        int m_X;
        int m_Y;

    };

    class MouseButtonPressedEvent : public MouseEvent
    {
    public:
        MouseButtonPressedEvent(const MouseCode MouseCode, int x, int y) : MouseEvent(MouseCode, x, y) {}
        
        static EventType getStaticType() { return EventType::MouseButtonPressed; }
        EventType getEventType() const { return getStaticType(); }
        const char* getName() const { return "MouseButtonPressed"; }
    };

    class MouseButtonReleasedEvent : public MouseEvent
    {
    public:
        MouseButtonReleasedEvent(const MouseCode MouseCode, int x, int y) : MouseEvent(MouseCode, x, y) {}
        
        static EventType getStaticType() { return EventType::MouseButtonReleased; }
        EventType getEventType() const { return getStaticType(); }
        const char* getName() const { return "MouseButtonReleased"; }
    };

    class MouseMovedEvent : public MouseEvent
    {
    public:
        MouseMovedEvent(const MouseCode MouseCode, int x, int y) : MouseEvent(MouseCode, x, y) {}

        static EventType getStaticType() { return EventType::MouseMoved; }
        EventType getEventType() const { return getStaticType(); }
        const char* getName() const { return "MouseMoved"; }
    };

} // namespace PFSim

#endif