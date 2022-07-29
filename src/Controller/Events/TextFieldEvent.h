#ifndef _TextFieldEvent_h_
#define _TextFieldEvent_h_ 

#include <sstream>

#include "Event.h"
#include "TextFieldCodes.h"

namespace PFSim {

    class TextFieldEvent : public Event
    {
    public:
        TextFieldCode getTextFieldCode() const { return m_TextFieldCode; }

        virtual int getCategoryFlags() const override { return EventCategory::EventCategoryTextField; }
        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual std::string toString() const { return getName(); }; // debug tool

    protected:
        TextFieldEvent(const TextFieldCode TextFieldCode) 
                        : m_TextFieldCode(TextFieldCode) {}

        TextFieldCode m_TextFieldCode;
    };

    class UpdateMazeLengthEvent : public TextFieldEvent
    {
    public:
        UpdateMazeLengthEvent(const TextFieldCode TextFieldCode) 
                            : TextFieldEvent(TextFieldCode) 
        {
        }
        
        static EventType getStaticType() { return EventType::UpdateMazeLength; }
        virtual EventType getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "UpdateMazeLength"; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "TextFieldEvent: " << m_TextFieldCode << " = " << getName();
            return ss.str();
        }

    private:
    
    };

}

#endif