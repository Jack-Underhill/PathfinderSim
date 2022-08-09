#ifndef _Application_h_
#define _Application_h_

#include <memory>
#include <chrono>
#include <thread>

#include "gevent.h"

#include "Event.h"
// #include "SliderEvent.h"
#include "AnimationTimer.h"
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
        AnimationTimer m_AnimationTimer;
        
        bool onCheckpointEvent(UpdateCheckpointEvent& e);
        bool onPathfinderEvent(UpdatePathfinderEvent& e);
        bool onGeneratorEvent(UpdateGeneratorEvent& e);

        bool onMousePressedEvent(MouseButtonPressedEvent& e);
        bool onMouseReleasedEvent(MouseButtonReleasedEvent& e);
        bool onMouseMovedEvent(MouseMovedEvent& e);

        bool onSliderEvent(SliderMovedEvent& e);

        void runPathfindingSimulation(PathfinderType type);
        void runGenerator(GeneratorType type);
        void runPathfinder(PathfinderType type);
        void runGraphReset();
        void runPathSolution();

        void runMousePressed(int x, int y);
        void runMouseReleased();
        void runMouseMoved(int x, int y);

        void handleAnimationTimer(MazeNode*& node);


        bool isMouseInsideSimBounds(int x, int y);

        void updateMazeLength();
        bool isValidMazeLength(std::string gtf_MazeLength) const;
        bool isAnInteger(std::string str) const;

        void updateCPButtons(bool isIncrementingPositively);
    };

}

#endif