#include "SimulatePathfinding.h"

namespace PFSim {

    SimulatePathfinding::SimulatePathfinding(MazeGraph*& graph, Window*& window, AnimationTimer*& timer)
    {
        m_Graph = graph;
        m_Window = window;
        m_AnimationTimer = timer;

        m_HasSimulationFailed = false;
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

    void SimulatePathfinding::initPathfinder(bool isAnimating, int target) 
    {
        switch(m_Pathfinder)
        {
        case(BFS):
            m_Animation = new Pathfinder::BFS(m_Graph);
            break;
        case(DFS):
            m_Animation = new Pathfinder::DFS(m_Graph);
            break;
        case(AStar):
            m_Animation = new Pathfinder::AStar(m_Graph);
            break;
        case(SHP_BFS):
            m_Animation = new Pathfinder::SHP_BFS(m_Graph, target);
            break;
        case(SHP_AStar):
            m_Animation = new Pathfinder::SHP_AStar(m_Graph, target);
            break;
        }
        ((PathfinderTemplate*)m_Animation)->setTargetList(m_TargetListSize, m_TargetList);
        
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
    
    void SimulatePathfinding::initTargets()
    {
        m_TargetListSize = m_Graph->getTargetCount();
        int* list = m_Graph->getTargets();

        //Copy array.
        //(So changing data in the SimulatePathfinding::m_TargetList wont affect the data in 
        //MazeGraph::m_Graph)
        for(int i = 0; i < m_TargetListSize; i++)
        {
            m_TargetList[i] = list[i];
        }
    }

    void SimulatePathfinding::runSimulation()
    {
        //simulation setup
        initPath(true);
        initTargets();
        m_Window->getStatisticsDisplay()->resetPathingStats();
        m_Graph->clearTargetFound();
        if(!m_Graph->isReadyForSimulation())
        {
            runGraphReset();
        }


        //execute simulation
        if(m_Pathfinder == PathfinderType::SHP_BFS || m_Pathfinder == PathfinderType::SHP_AStar)
        {
            if(m_Graph->getTargetCount() > 0)
            {
                runSHPSimulation();
            }
            else
            {
                m_TargetListSize = 0;
                int target = m_Graph->getEndNode()->getPosition().positionKey;
                runPathfinder(target);
                runGraphReset();
            }
        }
        else
        {
            while((m_Graph->getLastFoundTarget() == nullptr || m_Graph->getLastFoundTarget()->getType() != EndCell) && !m_HasSimulationFailed) 
            {
                runPathfinder();
                runGraphReset();
            }
        }


        // simulation after-effects
        if(!m_HasSimulationFailed)
        {
            runPath();
        }
        
        delete m_PathSolution;
    }

    //runSHPExecution() ?
    void SimulatePathfinding::runSHPSimulation()
    {
        Pathfinder::SHPMinHeap minHeap;
        Pathfinder::Permutations permutations(m_Graph->getTargets(), m_Graph->getTargetCount());

        while(!permutations.empty())
        {
            Pathfinder::Sequence* sequence = permutations.top();
            permutations.pop();

            //execute simulation
            while((m_Graph->getLastFoundTarget() == nullptr || m_Graph->getLastFoundTarget()->getType() != EndCell) && !m_HasSimulationFailed) 
            {
                int target;
                if(!sequence->empty())
                {
                    target = sequence->top();
                    sequence->pop();
                    m_TargetListSize = 1;
                }
                else 
                {
                    target = m_Graph->getEndNode()->getPosition().positionKey;
                    m_TargetListSize = 0;
                }
                m_TargetList[0] = target; 
                
                runPathfinder(target);
                runGraphReset();
            }

            int pathSolutionSteps = m_PathSolution->getStepCount();
            minHeap.push( new Pathfinder::SHPHeapProp(m_PathSolution, pathSolutionSteps) );

            //reset
            initPath(true);
            m_Window->getStatisticsDisplay()->resetPathSolution();
            m_Graph->clearTargetFound();
            delete sequence;
        }
        
        m_PathSolution = ((Pathfinder::SHPHeapProp*)minHeap.top())->path;
    }

    void SimulatePathfinding::runPathfinder(int target)
    {
        //algorithm setup
        initPathfinder(true, target);
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

        //update Targets
        m_TargetListSize = ((PathfinderTemplate*)m_Animation)->getTargetListSize();
        int* list = ((PathfinderTemplate*)m_Animation)->getTargetList();
        for(int i = 0; i < m_TargetListSize; i++)
        {
            m_TargetList[i] = list[i];
        }
    }

} // namespace PFSim