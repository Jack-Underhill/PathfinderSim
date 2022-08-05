#ifndef _MouseEvent_h_
#define _MouseEvent_h_

#include <sstream>

#include "Event.h"
#include "MouseCodes.h"

namespace PFSim {
    
    class MouseEvent : public Event
    {
    public:
        MouseEvent(const MouseCode MouseCode, int x, int y) : m_MouseCode(MouseCode), m_X(x), m_Y(y) {}

        MouseCode getMouseCode() const { return m_MouseCode; }

        virtual int getCategoryFlags() const override { return EventCategory::EventCategoryMouse; }
        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual std::string toString() const { return getName(); }; // debug tool

        int getX() { return m_X; }
        int getY() { return m_Y; }

    protected:
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

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseEvent: " << m_MouseCode << " = " << getName();
            return ss.str();
        }
    private:

    };

    class MouseButtonReleasedEvent : public MouseEvent
    {
    public:
        MouseButtonReleasedEvent(const MouseCode MouseCode, int x, int y) : MouseEvent(MouseCode, x, y) {}
        
        static EventType getStaticType() { return EventType::MouseButtonReleased; }
        EventType getEventType() const { return getStaticType(); }
        const char* getName() const { return "MouseButtonReleased"; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseEvent: " << m_MouseCode << " = " << getName();
            return ss.str();
        }
    private:

    };

    class MouseMovedEvent : public MouseEvent
    {
    public:
        MouseMovedEvent(const MouseCode MouseCode, int x, int y) : MouseEvent(MouseCode, x, y) {}

        static EventType getStaticType() { return EventType::MouseMoved; }
        EventType getEventType() const { return getStaticType(); }
        const char* getName() const { return "MouseMoved"; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseEvent: " << m_MouseCode << " = " << getName();
            return ss.str();
        }
    private:

    };

} // namespace PFSim


#endif