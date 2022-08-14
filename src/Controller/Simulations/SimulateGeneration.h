#ifndef _SimulateGeneration_h_
#define _SimulateGeneration_h_

#include "MazeGraph.h"
#include "Window.h"
#include "AnimationTimer.h"

namespace PFSim
{
    
    class SimulateGeneration
    {
    public:
        SimulateGeneration(MazeGraph*& graph, Window*& window, AnimationTimer*& timer);
        ~SimulateGeneration() = default;

        void run(GeneratorType type);

    private:
        MazeGraph* m_Graph;
        Window* m_Window;
        AnimationTimer* m_AnimationTimer;
        
        void runGenerator(GeneratorType type);

        void updateMazeLength();
        bool isValidMazeLength(std::string gtf_MazeLength) const;
        bool isAnInteger(std::string str) const;

        void handleAnimationTimer(MazeNode*& node);////////////////////////////////////////////////////
    };

} // namespace PFSim


#endif