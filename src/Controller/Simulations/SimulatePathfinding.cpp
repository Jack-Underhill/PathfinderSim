#include "SimulatePathfinding.h"

namespace PFSim
{

    SimulatePathfinding::SimulatePathfinding(MazeGraph*& graph, Window*& window, AnimationTimer*& timer)
    {
        m_Graph = graph;
        m_Window = window;
        m_AnimationTimer = timer;
        m_HasSimulationFailed = false;
    }

    void SimulatePathfinding::run(bool isAnimating, PathfinderType type)
    {
        if(isAnimating)
        {
            runPathfindingSimulation(type);
        }
        else
        {
            runNonAnimationSimulation();
        }
    }

    void SimulatePathfinding::reset()
    {
        runGraphReset(false);
    }
    
    void SimulatePathfinding::clearObstacles()
    {
        runGraphReset(true);
    }

    void SimulatePathfinding::runPathfindingSimulation(PathfinderType type)
    {
        //simulation setup
        m_Window->getStatisticsDisplay()->resetPathingStats();
        m_Graph->updateSimulationSetup();
        if(!m_Graph->isReadyForSimulation())
        {
            runGraphReset(false);
        }

        //execute simulation
        while(m_Graph->getTargetFoundType() != EndCell && !m_HasSimulationFailed) 
        {
            runPathfinder(type);

            if(!m_HasSimulationFailed)
            {
                runGraphReset(false);
            }
        }

        // simulation after-effects
        if(m_HasSimulationFailed)
        {
            m_Graph->setIsReadyForSimulation(false);
        }
        else
        {
            runPathSolution();
        }
    }

    void SimulatePathfinding::runPathfinder(PathfinderType type)
    {
        //algorithm setup
        m_Graph->setPathfinder(type);
        m_Window->getStatisticsDisplay()->updateTitle( Pathfind, m_Graph->getAnimationTitle() );
        m_AnimationTimer->updateAnimation(AnimationType::Pathfind, m_Graph->getMazeLength(), m_Graph->getGeneratorType());
        Timer timer;

        //execute algorithm
        while(!m_Graph->isAnimationComplete() && m_Graph->isPathfinderStillSearching()) 
        {
            MazeNode*& node = m_Graph->updateAnimation();
            handleAnimationTimer(node);
            
            m_Window->getSimulationDisplay()->updateMazeNode( node, m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
        }

        // alogorithm after-effects
        if(!m_Graph->isAnimationComplete())
        {
            m_HasSimulationFailed = true;
        }
        else
        {
            //tell the graph this algorithm ended
            m_Graph->updateTargetFound();
            
            //update Stats
            m_Window->getStatisticsDisplay()->updateStepCount( Pathfind, m_Graph->getStepCount() );
            m_Window->getStatisticsDisplay()->updateTimeRan( Pathfind, timer.getElapsedTime() );
        }
    }

    void SimulatePathfinding::runGraphReset(bool isClearingObstacles)
    {
        //algorithm setup
        if(isClearingObstacles)
        {
            m_Graph->setObstacleClear();
        }
        else
        {
            m_Graph->setGraphReset();
        }
        m_AnimationTimer->updateAnimation(AnimationType::Reset, m_Graph->getMazeLength());
        int lastMarkerPosition = 0;

        //execute algorithm
        while(!m_Graph->isAnimationComplete()) 
        {
            MazeNode*& node = m_Graph->updateAnimation();
            handleAnimationTimer(node);
            
            m_Window->getSimulationDisplay()->updateMazeNode( node, m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
            
            // update markers when moved
            if(lastMarkerPosition < node->getPosition().x)  
            {
                m_Window->getSimulationDisplay()->updateResetMarkers( node->getPosition().x, m_Graph->getCellSize(), m_Graph->getMazeLength() );
                lastMarkerPosition = node->getPosition().x;
            }
        }
        
        //clear leftover markers
        m_Window->getSimulationDisplay()->updateResetMarkers( m_Graph->getMazeLength() + 1, m_Graph->getCellSize(), m_Graph->getMazeLength() );
    }

    void SimulatePathfinding::runPathSolution()
    {
        //algorithm setup
        m_Graph->setPathSolution();
        m_Window->getStatisticsDisplay()->updateTitle( DrawPath, m_Graph->getAnimationTitle() );
        m_AnimationTimer->updateAnimation(AnimationType::DrawPath, m_Graph->getMazeLength());
        Timer timer;

        //execute algorithm
        while(!m_Graph->isAnimationComplete()) 
        {
            MazeNode*& node = m_Graph->updateAnimation();
            handleAnimationTimer(node);

            m_Window->getSimulationDisplay()->updatePathNode( node, m_Graph->getCellSize() );
        }
        
        //update Stats
        m_Window->getStatisticsDisplay()->updateStepCount( DrawPath, m_Graph->getStepCount() );
        m_Window->getStatisticsDisplay()->updateTimeRan( DrawPath, timer.getElapsedTime() );
    }
        
    void SimulatePathfinding::runNonAnimationSimulation()
    {
        //simulation setup
        m_Window->getStatisticsDisplay()->resetPathingStats();
        m_Graph->updateSimulationSetup();
        if(!m_Graph->isReadyForSimulation())
        {
            runNonAnimationReset(true);
        }

        //execute simulation
        while(m_Graph->getTargetFoundType() != EndCell && !m_HasSimulationFailed) 
        {
            runNonAnimationPathfinder(m_Graph->getPathfinderType());

            if(!m_HasSimulationFailed)
            {
                runNonAnimationReset(false);
            }
        }

        // simulation after-effects
        if(m_HasSimulationFailed)
        {
            m_Graph->setIsReadyForSimulation(false);
        }
        else
        {
            runNonAnimationPath();
        }
    }

    void SimulatePathfinding::runNonAnimationPathfinder(PathfinderType type)
    {
        //algorithm setup
        m_Graph->setPathfinder(type);
        m_Graph->setIsAnimating(false);
        m_Window->getStatisticsDisplay()->updateTitle( Pathfind, m_Graph->getAnimationTitle() );
        Timer timer;
        
        //execute algorithm
        while(!m_Graph->isAnimationComplete() && m_Graph->isPathfinderStillSearching()) 
        {
            m_Graph->updateAnimation();
        }

        // alogorithm after-effects
        if(!m_Graph->isAnimationComplete())
        {
            m_HasSimulationFailed = true;
        }
        else
        {
            //tell the graph this algorithm ended
            m_Graph->updateTargetFound();
            
            //update Stats
            m_Window->getStatisticsDisplay()->updateStepCount( Pathfind, m_Graph->getStepCount() );
            m_Window->getStatisticsDisplay()->updateTimeRan( Pathfind, timer.getElapsedTime() );
        }
    }

    void SimulatePathfinding::runNonAnimationReset(bool doesUpdateScreen)
    {
        //algorithm setup
        m_Graph->setGraphReset();

        //execute algorithm
        while(!m_Graph->isAnimationComplete()) 
        {
            MazeNode*& node = m_Graph->updateAnimation();

            if(doesUpdateScreen)
            {
                m_Window->getSimulationDisplay()->updateMazeNode( node, m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
            }
        }
    }

    void SimulatePathfinding::runNonAnimationPath()
    {
        //algorithm setup
        m_Graph->setPathSolution();
        m_Graph->setIsAnimating(false);
        m_Window->getStatisticsDisplay()->updateTitle( DrawPath, m_Graph->getAnimationTitle() );
        Timer timer;

        //execute algorithm
        while(!m_Graph->isAnimationComplete()) 
        {
            m_Window->getSimulationDisplay()->updatePathNode( m_Graph->updateAnimation(), m_Graph->getCellSize() );
        }
        
        //update Stats
        m_Window->getStatisticsDisplay()->updateStepCount( DrawPath, m_Graph->getStepCount() );
        m_Window->getStatisticsDisplay()->updateTimeRan( DrawPath, timer.getElapsedTime() );
    }

    void SimulatePathfinding::handleAnimationTimer(MazeNode*& node)
    {
        if(!node->isNext() && m_Window->getAnimationSpeedValue() != 100)
        {
            m_AnimationTimer->run();
        }
    }

} // namespace PFSim