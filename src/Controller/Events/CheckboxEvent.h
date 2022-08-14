#ifndef _CheckboxEvent_h_
#define _CheckboxEvent_h_

#include <sstream>

#include "Event.h"
#include "CheckboxCodes.h"

namespace PFSim {
    
    class CheckboxEvent : public Event
    {
    public:
        CheckboxEvent(const CheckboxCode checkboxCode, bool isChecked) : m_CheckboxCode(checkboxCode), m_IsChecked(isChecked) {}

        CheckboxCode getCheckboxCode() const { return m_CheckboxCode; }

        virtual int getCategoryFlags() const override { return EventCategory::EventCategoryCheckbox; }
        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual std::string toString() const { return getName(); }; // debug tool

        bool isChecked() const { return m_IsChecked; }

    protected:
        CheckboxCode m_CheckboxCode;

    private:
        bool m_IsChecked;
    };
    
    class InstantPathUpdatingEvent : public CheckboxEvent
    {
    public:
        InstantPathUpdatingEvent(const CheckboxCode checkboxCode, bool isChecked) : CheckboxEvent(checkboxCode, isChecked) {}
        
        static EventType getStaticType() { return EventType::InstantPathUpdating; }
        virtual EventType getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "InstantPathUpdating"; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "CheckboxEvent: "  << m_CheckboxCode << " = " << getName();
            return ss.str();
        }
    };

} //namespace PFSim

#endif