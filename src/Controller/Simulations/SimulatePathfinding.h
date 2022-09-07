#ifndef _SimulatePathfinding_h_
#define _SimulatePathfinding_h_

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
    
    class SimulatePathfinding
    {
    public:
        SimulatePathfinding(MazeGraph*& graph, Window*& window, AnimationTimer*& timer);
        ~SimulatePathfinding() = default;

        // If isAnimating is false, then the simulation without animating will run.
        // If isAnimating is true, then also give the PathfinderType to animate the simulation.
        void run(bool isAnimating, PathfinderType type);

        // Animate clearing all the pathfinding after-effects to the graph.
        void reset();

        void clearObstacles();

    protected:
        MazeGraph* m_Graph;
        Window* m_Window;
        AnimationTimer* m_AnimationTimer;

        bool m_HasSimulationFailed;

        AnimationObject* m_Animation;
        PathSolution* m_PathSolution;
        PathfinderType m_Pathfinder;

        int m_TargetListSize;
        int* m_TargetList;

        void initPathfinder(bool isAnimating);
        void initPath(bool isAnimating);
        void initReset(bool isClearingObstacles = false);

        void handleAnimationTimer(MazeNode*& node);
        void handlePathfinderFinalizing(Timer& timer);

    private:
        virtual void runSimulation();
        virtual void runPathfinder();
        virtual void runGraphReset(bool isClearingObstacles = false, bool isAnimating = true);
        virtual void runPath();
    };

} // namespace PFSim

#endif