#ifndef _Application_h_
#define _Application_h_

#include <memory>

#include "gevent.h"

#include "Event.h"
#include "Window.h"
#include "MazeGraph.h"

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
        std::unique_ptr<MazeGraph> m_Graph;

        bool onCheckpointEvent(UpdateCheckpointEvent& e);
        bool onPathfinderEvent(UpdatePathfinderEvent& e);
        bool onGeneratorEvent(UpdateGeneratorEvent& e);

        void runAnimation();        // This is a basic version of run; only applying SimulationDisplay::updateMazeNode().
        // void runResetNodes();    // This is a new name for resetBoard/resetGraph/resetMaze from last version of project.


        int updateMazeLength();
        bool isValidMazeLength(std::string gtf_MazeLength) const;
        bool isAnInteger(std::string str) const;
    };

}

#endif