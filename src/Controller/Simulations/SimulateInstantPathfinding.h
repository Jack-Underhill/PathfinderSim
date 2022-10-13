#ifndef _SimulateInstantPathfinding_h_
#define _SimulateInstantPathfinding_h_

#include "SimulatePathfinding.h"
#include "MazeGraph.h"
#include "Window.h"
#include "AnimationTimer.h"
#include "AnimationObject.h"
#include "BFS.h"
#include "DFS.h"
#include "PathSolution.h"
#include "ResetNodes.h"
#include "ClearObstacles.h"


namespace PFSim {
    
    class SimulateInstantPathfinding : public SimulatePathfinding
    {
    public:
        SimulateInstantPathfinding(MazeGraph*& graph, Window*& window, AnimationTimer*& timer);
        ~SimulateInstantPathfinding() = default;

    private:
        void runSimulation() override;
        void runPathfinder(int target = -1) override;
        void runGraphReset(bool isClearingObstacles = false, bool isAnimating = true) override;
        void runPath() override;
    };

} // namespace PFSim

#endif