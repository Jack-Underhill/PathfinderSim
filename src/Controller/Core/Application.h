#ifndef _Application_h_
#define _Application_h_

#include <memory>
#include <chrono>
#include <thread>

#include "gevent.h"

#include "Event.h"
#include "AnimationTimer.h"
#include "Window.h"
#include "MazeGraph.h"
#include "SimulateGeneration.h"
#include "SimulatePathfinding.h"

namespace PFSim {

    class Application 
    {
    public:
        Application();
        ~Application();

        void run();

    private:
        Window* m_Window;
        MazeGraph* m_Graph;
        AnimationTimer* m_AnimationTimer;
        bool m_IsValidToInstantPathUpdate;  // need better bool names
        bool m_IsInstantPathUpdatingEnabled;

        void onEvent(Event& e);
        
        bool onCheckpointEvent(UpdateCheckpointEvent& e);
        bool onPathfinderEvent(UpdatePathfinderEvent& e);
        bool onGeneratorEvent(UpdateGeneratorEvent& e);
        bool onMousePressedEvent(MouseButtonPressedEvent& e);
        bool onMouseReleasedEvent(MouseButtonReleasedEvent& e);
        bool onMouseMovedEvent(MouseMovedEvent& e);
        bool onSliderEvent(SliderMovedEvent& e);
        bool onInstantPathUpdatingEvent(InstantPathUpdatingEvent& e);
        bool onWallCellClearEvent(WallCellClearEvent& e);

        bool isMouseInsideSimBounds(int x, int y);
        void updateCPButtons(bool isIncrementingPositively);
        
        void handleMouseDrawingEvent();
        void handleMouseDraggingEvent();
    };

}

#endif