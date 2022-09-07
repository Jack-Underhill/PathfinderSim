#include "SimulatePathfinding.h"

namespace PFSim
{

    SimulatePathfinding::SimulatePathfinding(MazeGraph*& graph, Window*& window, AnimationTimer*& timer)
    {
        m_Graph = graph;
        m_Window = window;
        m_AnimationTimer = timer;

        m_HasSimulationFailed = false;

        m_TargetList = nullptr;
    }

    void SimulatePathfinding::run(bool isAnimating, PathfinderType type)
    {
        m_Pathfinder = type;
        runSimulation();
    }

    void SimulatePathfinding::reset()
    {
        runGraphReset();
    }
    
    void SimulatePathfinding::clearObstacles()
    {
        runGraphReset(true);
    }

    void SimulatePathfinding::initPathfinder(bool isAnimating)
    {
        switch(m_Pathfinder)
        {
        case(BFS):
            m_Animation = new Pathfinder::BFS(m_Graph);
            break;
        case(DFS):
            m_Animation = new Pathfinder::DFS(m_Graph);
            break;
        }

        if(m_TargetList != nullptr)
        {
            ((PathfinderTemplate*)m_Animation)->setTargetList(m_TargetListSize, m_TargetList);
        }
        m_Window->getStatisticsDisplay()->updateTitle( AnimationType::Pathfind, m_Animation->getTitle() );

        if(isAnimating)
        {
            m_AnimationTimer->updateAnimation( AnimationType::Pathfind, m_Graph->getMazeLength(), m_Graph->getGeneratorType() );
        }
        m_Animation->setIsAnimating(isAnimating);
    }
    
    void SimulatePathfinding::initPath(bool isAnimating)
    {
        m_PathSolution = new PathSolution();

        m_PathSolution->setIsAnimating(isAnimating);
    }

    void SimulatePathfinding::initReset(bool isClearingObstacles)
    {
        if(isClearingObstacles)
        {
            m_Animation = new ClearObstacles(m_Graph);
        }
        else
        {
            m_Animation = new ResetNodes(m_Graph);
        }

        m_Graph->setIsReadyForSimulation(true);
    }

    void SimulatePathfinding::runSimulation()
    {
        //simulation setup
        initPath(true);
        m_Window->getStatisticsDisplay()->resetPathingStats();
        m_Graph->clearTargetFound();
        if(!m_Graph->isReadyForSimulation())
        {
            runGraphReset();
        }

        //execute simulation
        while((m_Graph->getLastFoundTarget() == nullptr || m_Graph->getLastFoundTarget()->getType() != EndCell) && !m_HasSimulationFailed) 
        {
            runPathfinder();
            runGraphReset();
        }

        // simulation after-effects
        if(!m_HasSimulationFailed)
        {
            runPath();
        }
        
        delete m_PathSolution;
    }

    void SimulatePathfinding::runPathfinder()
    {
        //algorithm setup
        initPathfinder(true);
        Timer timer;

        //execute algorithm
        while(!m_Animation->isComplete() && ((PathfinderTemplate*)m_Animation)->isStillSearching()) 
        {
            MazeNode*& node = m_Graph->getNodeMap()->at(m_Animation->step());
            handleAnimationTimer(node);
            
            m_Window->getSimulationDisplay()->updateMazeNode( node, m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
        }

        handlePathfinderFinalizing(timer);
        delete m_Animation;
    }

    void SimulatePathfinding::runGraphReset(bool isClearingObstacles, bool isAnimating)
    {
        //algorithm setup
        initReset(isClearingObstacles);
        m_AnimationTimer->updateAnimation(AnimationType::Reset, m_Graph->getMazeLength());
        int lastMarkerPosition = 0;

        //execute algorithm
        while(!m_Animation->isComplete()) 
        {
            MazeNode*& node = m_Graph->getNodeMap()->at( m_Animation->step() );
            handleAnimationTimer(node);
            
            m_Window->getSimulationDisplay()->updateNodeReset( node, m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
            ((ResetNodes*)m_Animation)->resetDirection();
            
            // update markers when moved
            if(lastMarkerPosition < node->getPosition().x)  
            {
                m_Window->getSimulationDisplay()->updateResetMarkers( node->getPosition().x, m_Graph->getCellSize(), m_Graph->getMazeLength() );
                lastMarkerPosition = node->getPosition().x;
            }
        }
        
        //clear leftover markers
        m_Window->getSimulationDisplay()->updateResetMarkers( m_Graph->getMazeLength() + 1, m_Graph->getCellSize(), m_Graph->getMazeLength() );
        
        delete m_Animation;
    }

    void SimulatePathfinding::runPath()
    {
        //algorithm setup
        m_PathSolution->reversePath();
        m_Graph->setIsReadyForSimulation(false);
        m_Window->getStatisticsDisplay()->updateTitle( DrawPath, m_PathSolution->getTitle() );
        m_AnimationTimer->updateAnimation(AnimationType::DrawPath, m_Graph->getMazeLength());
        Timer timer;

        //execute algorithm
        while(!m_PathSolution->isComplete()) 
        {
            MazeNode*& node = m_Graph->getNodeMap()->at( m_PathSolution->step() );
            handleAnimationTimer(node);

            m_Window->getSimulationDisplay()->updatePathNode( node, m_Graph->getCellSize() );
        }
        
        //update Stats
        m_Window->getStatisticsDisplay()->updateStepCount( DrawPath, m_PathSolution->getStepCount() );
        m_Window->getStatisticsDisplay()->updateTimeRan( DrawPath, timer.getElapsedTime() );
    }

    void SimulatePathfinding::handleAnimationTimer(MazeNode*& node)
    {
        if(!node->isNext() && m_Window->getAnimationSpeedValue() != 100)
        {
            m_AnimationTimer->run();
        }
    }
    
    void SimulatePathfinding::handlePathfinderFinalizing(Timer& timer)
    {
        if(m_Animation->isComplete() && !((PathfinderTemplate*)m_Animation)->isStillSearching())
        {
            //tell the graph this algorithm ended
            m_Graph->setTargetFound( ((PathfinderTemplate*)m_Animation)->getTargetNodeFound() );
            m_PathSolution->addCurrentSolution( m_Graph->getLastFoundTarget() );
            
            //update Stats
            m_Window->getStatisticsDisplay()->updateStepCount( Pathfind, m_Animation->getStepCount() );
            m_Window->getStatisticsDisplay()->updateTimeRan( Pathfind, timer.getElapsedTime() );
        }
        else
        {
            m_HasSimulationFailed = true;
            m_Graph->setIsReadyForSimulation(false);
        }

        m_TargetListSize = ((PathfinderTemplate*)m_Animation)->getTargetListSize();
        m_TargetList = ((PathfinderTemplate*)m_Animation)->getTargetList();
    }

} // namespace PFSim