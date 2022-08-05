#ifndef _ButtonEvent_h_
#define _ButtonEvent_h_

#include <sstream>

#include "Event.h"
#include "ButtonCodes.h"

namespace PFSim {
    
    class ButtonEvent : public Event
    {
    public:
        ButtonEvent(const ButtonCode ButtonCode) : m_ButtonCode(ButtonCode) {}

        ButtonCode getButtonCode() const { return m_ButtonCode; }

        virtual int getCategoryFlags() const override { return EventCategory::EventCategoryButton; }
        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual std::string toString() const { return getName(); }; // debug tool

    protected:
        ButtonCode m_ButtonCode;
    };

    class UpdateGeneratorEvent : public ButtonEvent
    {
    public:
        UpdateGeneratorEvent(const ButtonCode ButtonCode) : ButtonEvent(ButtonCode) 
        {
        }
        
        static EventType getStaticType() { return EventType::UpdateGenerator; }
        virtual EventType getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "UpdateGenerator"; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "ButtonEvent: " << m_ButtonCode << " = " << getName();
            return ss.str();
        }

    private:
    
    };

    class UpdatePathfinderEvent : public ButtonEvent
    {
    public:
        UpdatePathfinderEvent(const ButtonCode ButtonCode) : ButtonEvent(ButtonCode) 
        {
        }
        
        static EventType getStaticType() { return EventType::UpdatePathfinder; }
        virtual EventType getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "UpdatePathfinder"; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "ButtonEvent: " << m_ButtonCode << " = " << getName();
            return ss.str();
        }

    private:
    
    };

    class UpdateCheckpointEvent : public ButtonEvent
    {
    public:
        UpdateCheckpointEvent(const ButtonCode ButtonCode) : ButtonEvent(ButtonCode) 
        {
        }
        
        static EventType getStaticType() { return EventType::UpdateCheckpoint; }
        virtual EventType getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "UpdateCheckpoint"; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "ButtonEvent: " << m_ButtonCode << " = " << getName();
            return ss.str();
        }

    private:
    
    };

}

#endif