#ifndef _SliderEvent_h_
#define _SliderEvent_h_

#include <sstream>

#include "Event.h"
#include "SliderCodes.h"

namespace PFSim {
    
    class SliderEvent : public Event
    {
    public:
        SliderCode getSliderCode() const { return m_SliderCode; }

        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        
        std::string toString() const override
        {
            std::stringstream ss;
            ss << getName() << ": " << m_SliderCode;
            return ss.str();
        }

    protected:
        SliderEvent(const SliderCode sliderCode) : m_SliderCode(sliderCode) {}

        SliderCode m_SliderCode;
    };
    
    class SliderMovedEvent : public SliderEvent
    {
    public:
        SliderMovedEvent(const SliderCode sliderCode) : SliderEvent(sliderCode) {}
        
        static EventType getStaticType() { return EventType::SliderMoved; }
        virtual EventType getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "SliderMoved"; }
    };

} //namespace PFSim

#endif