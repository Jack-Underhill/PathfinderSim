#ifndef _ButtonEvent_h_
#define _ButtonEvent_h_

#include <sstream>

#include "Event.h"
#include "ButtonCodes.h"

namespace PFSim {
    
    class ButtonEvent : public Event
    {
    public:
        ButtonEvent(const ButtonCode buttonCode) : m_ButtonCode(buttonCode) {}

        ButtonCode getButtonCode() const { return m_ButtonCode; }
        
        static EventType getStaticType() { return EventType::ButtonPressed; }
        virtual EventType getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "ButtonPressed"; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << getName() << ": " << m_ButtonCode;
            return ss.str();
        }

    protected:
        ButtonCode m_ButtonCode;
    };

}

#endif