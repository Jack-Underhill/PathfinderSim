#ifndef _SimulatePathfinding_h_
#define _SimulatePathfinding_h_

#include "MazeGraph.h"
#include "Window.h"
#include "AnimationTimer.h"

namespace PFSim
{
    
    class SimulatePathfinding
    {
    public:
        SimulatePathfinding(MazeGraph*& graph, Window*& window, AnimationTimer*& timer);
        ~SimulatePathfinding() = default;

        // If isAnimating is false, then the simulation without animating will run.
        // If isAnimating is true, then also give the PathfinderType to animate the simulation.
        void run(bool isAnimating, PathfinderType type = PathfinderType::BFS);

        // Animate clearing all the pathfinding after-effects to the graph.
        void reset();

        void clearObstacles();

    private:
        MazeGraph* m_Graph;
        Window* m_Window;
        AnimationTimer* m_AnimationTimer;
        bool m_HasSimulationFailed;

        void runPathfindingSimulation(PathfinderType type);
        void runPathfinder(PathfinderType type);
        void runGraphReset(bool isClearingObstacles);
        void runPathSolution();
        
        void runNonAnimationSimulation();
        void runNonAnimationPathfinder(PathfinderType type);
        void runNonAnimationReset(bool doesUpdateScreen);
        void runNonAnimationPath();

        void handleAnimationTimer(MazeNode*& node);
    };

} // namespace PFSim


#endif