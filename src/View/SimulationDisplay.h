#ifndef _SimulationDisplay_h_
#define _SimulationDisplay_h_

#include "gwindow.h"

#include "Constants.h"

#include "MazeNode.h"

namespace PFSim {

    class SimulationDisplay
    {
    public: 
        SimulationDisplay(sgl::GWindow*& window) : m_Window(window) {}
        ~SimulationDisplay() = default;

        void clearDisplay();
        void updateResetMarkers(int x);
        void updateMazeNode(MazeNode* node);
        void updatePathNode(MazeNode* node);
        void updateMazeNodeFiller(MazeNode* node);
        void updatePathNodeFiller(MazeNode* node);
        
    private: 
        sgl::GWindow* m_Window;
    };

}

#endif