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
        void updateResetMarkers(int x, int cellSize, int mazeLength);
        void updateMazeNode(MazeNode*& node, int cellSize, bool isMazeGenerated);
        void updatePathNode(MazeNode*& node, int cellSize);
        void updateNodeReset(MazeNode*& node, int cellSize, bool isMazeGenerated);

    private: 
        sgl::GWindow* m_Window;
        
        void updateMazeNodeFiller(MazeNode*& node, int cellSize, bool isSpecialNode = false);
        void updatePathNodeFiller(MazeNode*& node, int cellSize);
        void updateNodeResetFillers(MazeNode*& node, int cellSize);

        void specialCase(MazeNode*& node, bool isSpecialNode);

        void fillMazeNode(MazeNode*& node, int cellSize);
    };

} // namespace PFSim

#endif