#ifndef _CheckboxEvent_h_
#define _CheckboxEvent_h_

#include <sstream>

#include "Event.h"
#include "CheckboxCodes.h"

namespace PFSim {
    
    class CheckboxEvent : public Event
    {
    public:
        CheckboxCode getCheckboxCode() const { return m_CheckboxCode; }

        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        
        std::string toString() const override
        {
            std::stringstream ss;
            ss << getName() << ": " << m_CheckboxCode;
            return ss.str();
        }

        bool isChecked() const { return m_IsChecked; }

    protected:
        CheckboxEvent(const CheckboxCode checkboxCode, bool isChecked) : m_CheckboxCode(checkboxCode), m_IsChecked(isChecked) {}
        
        CheckboxCode m_CheckboxCode;

    private:
        bool m_IsChecked;
    };
    
    class CheckboxClickedEvent : public CheckboxEvent
    {
    public:
        CheckboxClickedEvent(const CheckboxCode checkboxCode, bool isChecked) : CheckboxEvent(checkboxCode, isChecked) {}
        
        static EventType getStaticType() { return EventType::CheckboxClicked; }
        virtual EventType getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "CheckboxClicked"; }
    };

} //namespace PFSim

#endif