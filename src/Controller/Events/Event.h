#ifndef _Event_h_
#define _Event_h_

#include <string>
#include <functional>

namespace PFSim {

    enum class EventType
    {
        None = 0,
        // WindowResize,
        MouseButtonPressed, MouseButtonReleased, MouseMoved,
        ButtonPressed, //(GUI Button)
        SliderMoved,
        CheckboxClicked
    };
    
    class Event 
    {
        friend class EventDispatcher;
    public:
        virtual ~Event() = default;

        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual std::string toString() const { return getName(); }; // debug tool

    protected:
        bool m_IsHandled = false;
    };

    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
    public:
        EventDispatcher(Event& event) : m_Event(event) {}

        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            if(m_Event.getEventType() == T::getStaticType()) {
                m_Event.m_IsHandled = func(*(T*)&m_Event);
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };

    inline std::ostream& operator<< (std::ostream& os, const Event& e) { // debug tool
        return os << e.toString();
    }

} //namespace PFSim

#endif