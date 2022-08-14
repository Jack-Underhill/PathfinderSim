#include "SimulateGeneration.h"

namespace PFSim
{

    SimulateGeneration::SimulateGeneration(MazeGraph*& graph, Window*& window, AnimationTimer*& timer)
    {
        m_Graph = graph;
        m_Window = window;
        m_AnimationTimer = timer;
    }

    void SimulateGeneration::run(GeneratorType type)
    {
        runGenerator(type);
    }
    
    void SimulateGeneration::runGenerator(GeneratorType type)
    {
        //algorithm setup
        updateMazeLength();
        m_Graph->setGenerator(type);
        m_Window->getStatisticsDisplay()->updateTitle( Generate, m_Graph->getAnimationTitle() );
        m_Window->getSimulationDisplay()->clearDisplay();
        m_AnimationTimer->updateAnimation(AnimationType::Generate, m_Graph->getMazeLength(), type);
        Timer timer;

        //execute algorithm
        while(!m_Graph->isAnimationComplete()) 
        {
            MazeNode*& node = m_Graph->updateAnimation();
            handleAnimationTimer(node);
            
            m_Window->getSimulationDisplay()->updateMazeNode( node, m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
        }

        //set the graph's end point
        MazeNode*& node = m_Graph->setEndNode();
        handleAnimationTimer(node);
        m_Window->getSimulationDisplay()->updateMazeNode( node, m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
        
        //update Stats
        m_Window->getStatisticsDisplay()->updateStepCount( Generate, m_Graph->getStepCount() );
        m_Window->getStatisticsDisplay()->updateTimeRan( Generate, timer.getElapsedTime() );
    }

    void SimulateGeneration::updateMazeLength()
    {
        int mazeLength;
        std::string gtf_MazeLength = m_Window->getInputMazeLength();

        if(isValidMazeLength(gtf_MazeLength))
        {
            mazeLength = std::stoi(gtf_MazeLength);
        }
        else
        {
            mazeLength = m_Graph->getMazeLength();
            m_Window->setInputMazeLength(mazeLength);
        }

        m_Graph->setMazeLength(mazeLength);
    }

    bool SimulateGeneration::isValidMazeLength(std::string gtf_MazeLength) const
    {
        if(gtf_MazeLength.length() == 0) 
        {
            return false;
        }
        else 
        {
            bool isInteger = isAnInteger(gtf_MazeLength);
            bool isInsideLowerBound;
            bool isInsideUpperBound;

            if(isInteger) 
            {
                isInsideLowerBound = MINIMUM_MAZE_LENGTH <= std::stoi(gtf_MazeLength);
                isInsideUpperBound = std::stoi(gtf_MazeLength) <= MAXIMUM_MAZE_LENGTH;
            }

            return (isInteger && isInsideLowerBound && isInsideUpperBound);
        }
    }

    bool SimulateGeneration::isAnInteger(std::string str) const
    {
        for(char c : str) 
        {
            if(!isdigit(c))
                return false;
        }

        return true;
    }

    void SimulateGeneration::handleAnimationTimer(MazeNode*& node)
    {
        if(!node->isNext() && m_Window->getAnimationSpeedValue() != 100)
        {
            m_AnimationTimer->run();
        }
    }

} // namespace PFSim