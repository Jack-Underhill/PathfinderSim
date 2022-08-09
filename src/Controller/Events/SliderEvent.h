#ifndef _SliderEvent_h_
#define _SliderEvent_h_

#include <sstream>

#include "Event.h"
#include "SliderCodes.h"

namespace PFSim {
    
    class SliderEvent : public Event
    {
    public:
        SliderEvent(const SliderCode sliderCode) : m_SliderCode(sliderCode) {}

        SliderCode getSliderCode() const { return m_SliderCode; }

        virtual int getCategoryFlags() const override { return EventCategory::EventCategorySlider; }
        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual std::string toString() const { return getName(); }; // debug tool

    protected:
        SliderCode m_SliderCode;
    };
    
    class SliderMovedEvent : public SliderEvent
    {
    public:
        SliderMovedEvent(const SliderCode sliderCode) : SliderEvent(sliderCode) {}
        
        static EventType getStaticType() { return EventType::SliderMoved; }
        virtual EventType getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "SliderMoved"; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "SliderEvent: "  << m_SliderCode << " = " << getName();
            return ss.str();
        }
    };

} //namespace PFSim

#endif