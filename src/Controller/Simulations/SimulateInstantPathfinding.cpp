#include "SimulateInstantPathfinding.h"

namespace PFSim
{

    SimulateInstantPathfinding::SimulateInstantPathfinding(MazeGraph*& graph, Window*& window, AnimationTimer*& timer) : 
                                                           SimulatePathfinding(graph, window, timer)
    {
    }
        
    void SimulateInstantPathfinding::runSimulation()
    {
        //simulation setup
        initPath(false);
        initTargets();
        m_Window->getStatisticsDisplay()->resetPathingStats();
        m_Graph->clearTargetFound();
        if(!m_Graph->isReadyForSimulation())
        {
            runGraphReset(false, true);
        }


        //execute simulation
        if(m_Pathfinder == PathfinderType::SHP_BFS || m_Pathfinder == PathfinderType::SHP_AStar)//why does this not have the same conditions as SimulatePathfinding?
        {
            runSHPSimulation();
        }
        else
        {
            while((m_Graph->getLastFoundTarget() == nullptr || m_Graph->getLastFoundTarget()->getType() != EndCell) && !m_HasSimulationFailed) 
            {
                runPathfinder();
                runGraphReset(false, false);
            }
        }
        

        // simulation after-effects
        if(!m_HasSimulationFailed)
        {
            runPath();
        }
        
        delete m_PathSolution;
    }

    void SimulateInstantPathfinding::runPathfinder(int target)
    {
        //algorithm setup
        initPathfinder(false, target);
        Timer timer;
        
        //execute algorithm
        while(!m_Animation->isComplete() && ((PathfinderTemplate*)m_Animation)->isStillSearching()) 
        {
            m_Animation->step();
        }

        handlePathfinderFinalizing(timer);
        delete m_Animation;
    }

    void SimulateInstantPathfinding::runGraphReset(bool isClearingObstacles, bool isAnimating)
    {
        //algorithm setup
        initReset();

        //execute algorithm
        while(!m_Animation->isComplete()) 
        {
            MazeNode*& node = m_Graph->getNodeMap()->at( m_Animation->step() );

            if(isAnimating)
            {
                m_Window->getSimulationDisplay()->updateNodeReset( node, m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
            }
            ((ResetNodes*)m_Animation)->resetDirection();
        }
        
        delete m_Animation;
    }

    void SimulateInstantPathfinding::runPath()
    {
        //algorithm setup
        m_PathSolution->reversePath();
        m_Graph->setIsReadyForSimulation(false);
        m_Window->getStatisticsDisplay()->updateTitle( DrawPath, m_PathSolution->getTitle() );
        Timer timer;

        //execute algorithm
        while(!m_PathSolution->isComplete()) 
        {
            MazeNode*& node = m_Graph->getNodeMap()->at( m_PathSolution->step() );

            m_Window->getSimulationDisplay()->updatePathNode( node, m_Graph->getCellSize() );
        }
        
        //update Stats
        m_Window->getStatisticsDisplay()->updateStepCount( DrawPath, m_PathSolution->getStepCount() );
        m_Window->getStatisticsDisplay()->updateTimeRan( DrawPath, timer.getElapsedTime() );
    }

} // namespace PFSim