#ifndef _Application_h_
#define _Application_h_

#include "Event.h"
#include "Window.h"

#include <memory>

namespace PFSim {

    class Application 
    {
    public:
        Application();
        ~Application();

        void run();
        void onEvent(Event& e);

    private:
        bool onGeneratorEvent(UpdateGeneratorEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

}

#endif