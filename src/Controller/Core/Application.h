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

        void runPathfindingSimulation(PathfinderType type);
        void runGenerator(GeneratorType type);
        void runPathfinder(PathfinderType type);
        void runGraphReset();
        void runPathSolution();


        int updateMazeLength();
        bool isValidMazeLength(std::string gtf_MazeLength) const;
        bool isAnInteger(std::string str) const;

        void updateCPButtons(bool isIncrementingPositively);

        //Delays the animation accordingly to the time that animationDelay has been set to.
        void runTimer(AnimationType type, int mazeLength) const;
    };

}

#endif