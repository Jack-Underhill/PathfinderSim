#ifndef _Application_h_
#define _Application_h_

#include <chrono>
#include <thread>

#include "Event.h"
#include "AnimationTimer.h"
#include "Window.h"
#include "MazeGraph.h"
#include "SimulateGeneration.h"
#include "SimulatePathfinding.h"
#include "SimulateInstantPathfinding.h"
#include "InteractiveCells.h"

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
        bool m_IsInstantRepathingEnabled; //True if the user enabled instant repathing (from cell dragging).
        bool m_IsInstantRepathingValid;   //True if the graph conditions are valid for instant repathing.
        
        InteractiveCells* m_MouseInteraction;

        PathfinderType m_PathfinderType;

        void onEvent(Event& e);

        bool onButtonEvent(ButtonEvent& e);
        bool onCheckboxClickedEvent(CheckboxClickedEvent& e);
        bool onSliderMovedEvent(SliderMovedEvent& e);
        bool onMousePressedEvent(MouseButtonPressedEvent& e);
        bool onMouseReleasedEvent(MouseButtonReleasedEvent& e);
        bool onMouseMovedEvent(MouseMovedEvent& e);
        
        bool handleCheckpoint(ButtonCode& code);
        bool handlePathfinder(ButtonCode& code);
        bool handleGenerator(ButtonCode& code);
        bool handleObstacles(/*ButtonCode& code*/);
        void handleMouseDrawingEvent();
        void handleMouseDraggingEvent();

        void updateCPButtons(bool isIncrementingPositively);
        bool isMouseInsideSimBounds(int x, int y);
    };

}

#endif