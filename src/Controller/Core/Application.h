#ifndef _Application_h_
#define _Application_h_

#include <memory>

#include "gevent.h"

#include "Event.h"
#include "Window.h"

namespace PFSim {

    class Application 
    {
    public:
        Application();
        ~Application();

        void run();
        void onEvent(Event& e);

    private:
        std::unique_ptr<Window> m_Window;

        bool onCheckpointEvent(UpdateCheckpointEvent& e);
        bool onPathfinderEvent(UpdatePathfinderEvent& e);
        bool onGeneratorEvent(UpdateGeneratorEvent& e);
        bool onMazeLengthEvent(UpdateMazeLengthEvent& e);
    };

}

#endif