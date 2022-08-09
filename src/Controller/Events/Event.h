#ifndef _Event_h_
#define _Event_h_

#include <string>
#include <functional>

namespace PFSim {

    enum class EventType
    {
        None = 0,
        MouseButtonPressed, MouseButtonReleased, MouseMoved,
        SliderMoved,
        // WindowResize,
        UpdateGenerator, UpdatePathfinder, UpdateCheckpoint
    };

    enum EventCategory 
    {
        None = 0,
        EventCategoryMouse          = (1 << 0),
        EventCategoryButton         = (1 << 1),
        EventCategorySlider         = (1 << 2)
    };
    
    class Event 
    {
        friend class EventDispatcher;
    public:
        virtual ~Event() = default;

        virtual int getCategoryFlags() const = 0;
        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual std::string toString() const { return getName(); }; // debug tool

        inline bool IsInCategory(EventCategory category) { return getCategoryFlags() & category; }

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
};

#endif