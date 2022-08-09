#include "application.h"

namespace PFSim {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() 
    {
        m_Window = std::unique_ptr<Window>(new Window());
        m_Window->setEventCallback(BIND_EVENT_FN(onEvent));
        
        m_Graph = std::unique_ptr<MazeGraph>(new MazeGraph());

        m_AnimationTimer = AnimationTimer();

        runGenerator(Open);
    }

    Application::~Application() 
    {
    }

    void Application::run() 
    {
        //inifinitely keeping the project up
        while(m_Window->isRunning()) {}
    }

    void Application::onEvent(Event& e) 
    {
        // std::cout << "Event Fired: " << e << std::endl;

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(onMousePressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(onMouseReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(onMouseMovedEvent));

        dispatcher.Dispatch<UpdateCheckpointEvent>(BIND_EVENT_FN(onCheckpointEvent));
        dispatcher.Dispatch<UpdatePathfinderEvent>(BIND_EVENT_FN(onPathfinderEvent));
        dispatcher.Dispatch<UpdateGeneratorEvent>(BIND_EVENT_FN(onGeneratorEvent));
        
        dispatcher.Dispatch<SliderMovedEvent>(BIND_EVENT_FN(onSliderEvent));
    }

    bool Application::onCheckpointEvent(UpdateCheckpointEvent& e)
    {
        if(!m_Graph->isReadyForSimulation())
        {
            runGraphReset();
        }


        if(e.getButtonCode() == ButtonCode::cp_Add) 
        {
            updateCPButtons(true);
            
            m_Window->getSimulationDisplay()->updateMazeNode( m_Graph->addCheckpoint(), m_Graph->getCellSize(), false );
        }
        else if(e.getButtonCode() == ButtonCode::cp_Subtract) 
        {
            updateCPButtons(false);

            m_Window->getSimulationDisplay()->updateMazeNode( m_Graph->removeTopCheckpoint(), m_Graph->getCellSize(), false );
        }

        return true;
    }

    bool Application::onPathfinderEvent(UpdatePathfinderEvent& e)
    {
        PathfinderType type;

        if(e.getButtonCode() == ButtonCode::pf_BFS) 
        {
            type = BFS;
        }
        else if(e.getButtonCode() == ButtonCode::pf_DFS) 
        {
            type = DFS;
        }

        runPathfindingSimulation(type);

        return true;
    }
            
    bool Application::onGeneratorEvent(UpdateGeneratorEvent& e) 
    {
        GeneratorType type;

        if(e.getButtonCode() == ButtonCode::gen_Open) 
        {
            type = Open;
        }
        else if(e.getButtonCode() == ButtonCode::gen_DFS) 
        {
            type = DFSMaze;
        }

        runGenerator(type);

        return true;
    }
    
    bool Application::onMousePressedEvent(MouseButtonPressedEvent& e)
    {
        if(!m_Graph->isReadyForSimulation())
        {
            runGraphReset();
            m_Graph->setIsReadyForSimulation(true);
        }

        if(isMouseInsideSimBounds( e.getX(), e.getY() ))
        {
            runMousePressed( e.getX(), e.getY() );
        }

        return true;
    }

    bool Application::onMouseReleasedEvent(MouseButtonReleasedEvent& e)
    {
        if(m_Graph->isMouseInteractive())
        {
            runMouseReleased();
        }

        return true;
    }

    bool Application::onMouseMovedEvent(MouseMovedEvent& e)
    {
        if(m_Graph->isMouseInteractive() && isMouseInsideSimBounds( e.getX(), e.getY() ))
        {   
            runMouseMoved( e.getX(), e.getY() );

            std::stack<MazeNode*> swapCells = m_Graph->getNodesToSwap(); // change to array

            if(swapCells.size() > 0)
            {
                MazeNode* node = swapCells.top();
                swapCells.pop();

                m_Window->getSimulationDisplay()->updateMazeNode( node, m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
                m_Window->getSimulationDisplay()->updateMazeNode( swapCells.top(), m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
            }       
        }

        return true;
    }
    
    bool Application::onSliderEvent(SliderMovedEvent& e)
    {
        int speedPercent = m_Window->getAnimationSpeedValue();
        m_Window->setSpeedText(speedPercent);

        //update animation timer
        m_AnimationTimer.updatePercentage(speedPercent);

        return true;
    }

    void Application::runGenerator(GeneratorType type)
    {
        updateMazeLength();             // the updateMazeLength() full logic might need to be moved into graph
        m_Graph->setGenerator(type);
        updateCPButtons(false);
        
        m_Window->getStatisticsDisplay()->updateTitle( Generate, m_Graph->getAnimationTitle() );
        m_Window->getSimulationDisplay()->clearDisplay();

        m_AnimationTimer.updateAnimation(AnimationType::Generate, m_Graph->getMazeLength(), type);
        Timer timer;

        while(!m_Graph->isAnimationComplete()) 
        {
            MazeNode*& node = m_Graph->updateAnimation();
            handleAnimationTimer(node);
            
            m_Window->getSimulationDisplay()->updateMazeNode( node, m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
        }

        //set EndNode
        m_AnimationTimer.run();
        m_Window->getSimulationDisplay()->updateMazeNode( m_Graph->setEndNode(), m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
        
        //update Stats
        m_Window->getStatisticsDisplay()->updateStepCount( Generate, m_Graph->getStepCount() );
        m_Window->getStatisticsDisplay()->updateTimeRan( Generate, timer.getElapsedTime() );

        // std::cout << "Generation Finished\tTime Ran (" << timer.getElapsedTime() << "ms)\t Stepped (" << m_Graph->getStepCount() << ")\t\n";
        // std::cout << "\n";
    }

    void Application::runPathfindingSimulation(PathfinderType type)
    {
        m_Window->getStatisticsDisplay()->resetPathingStats();

        m_Graph->updateSimulationSetup();
        // Timer timer;

        if(!m_Graph->isReadyForSimulation())
        {
            runGraphReset();
        }

        while(m_Graph->getTargetFoundType() != EndCell) 
        {
            runPathfinder(type);
            runGraphReset();
        }
        runPathSolution();
        
        // std::cout << "Simulation Finished\tTime Ran (" << timer.getElapsedTime() << "ms)\n";
        // std::cout << "\n";
    }
    
    void Application::runPathfinder(PathfinderType type)
    {
        m_Graph->setPathfinder(type);
        m_AnimationTimer.updateAnimation(AnimationType::Pathfind, m_Graph->getMazeLength(), m_Graph->getGeneratorType());
        Timer timer;

        m_Window->getStatisticsDisplay()->updateTitle( Pathfind, m_Graph->getAnimationTitle() );

        while(!m_Graph->isAnimationComplete()) 
        {
            MazeNode*& node = m_Graph->updateAnimation();
            handleAnimationTimer(node);
            
            m_Window->getSimulationDisplay()->updateMazeNode( node, m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
        }

        m_Graph->updateTargetFound();
        
        //update Stats
        m_Window->getStatisticsDisplay()->updateStepCount( Pathfind, m_Graph->getStepCount() );
        m_Window->getStatisticsDisplay()->updateTimeRan( Pathfind, timer.getElapsedTime() );
        
        // std::cout << "Pathfinding Finished\tTime Ran (" << timer.getElapsedTime() << "ms)\t Stepped (" << m_Graph->getStepCount() << ")\n";
    }

    void Application::runGraphReset()
    {
        m_Graph->setGraphReset();
        m_AnimationTimer.updateAnimation(AnimationType::Reset, m_Graph->getMazeLength());
        Timer timer;

        int lastMarkerPosition = 0;
        while(!m_Graph->isAnimationComplete()) 
        {
            MazeNode*& node = m_Graph->updateAnimation();
            handleAnimationTimer(node);
            
            m_Window->getSimulationDisplay()->updateMazeNode( node, m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
            
            // update markers only when needed
            if(lastMarkerPosition < node->getPosition().x)  
            {
                m_Window->getSimulationDisplay()->updateResetMarkers( node->getPosition().x, m_Graph->getCellSize(), m_Graph->getMazeLength() );
                lastMarkerPosition = node->getPosition().x;
            }
        }
        
        //clear leftover markers
        m_Window->getSimulationDisplay()->updateResetMarkers( m_Graph->getMazeLength() + 1, m_Graph->getCellSize(), m_Graph->getMazeLength() );
        
        // std::cout << "Resetting Finished\tTime Ran (" << timer.getElapsedTime() << "ms)\t Stepped (" << m_Graph->getStepCount() << ")\n";
    }

    void Application::runPathSolution()
    {
        m_Graph->setPathSolution();
        m_AnimationTimer.updateAnimation(AnimationType::DrawPath, m_Graph->getMazeLength());
        Timer timer;

        m_Window->getStatisticsDisplay()->updateTitle( DrawPath, m_Graph->getAnimationTitle() );

        while(!m_Graph->isAnimationComplete()) 
        {
            MazeNode*& node = m_Graph->updateAnimation();
            handleAnimationTimer(node);

            // path solution update
            m_Window->getSimulationDisplay()->updatePathNode( node, m_Graph->getCellSize() );
        }
        
        //update Stats
        m_Window->getStatisticsDisplay()->updateStepCount( DrawPath, m_Graph->getStepCount() );
        m_Window->getStatisticsDisplay()->updateTimeRan( DrawPath, timer.getElapsedTime() );

        // std::cout << "Path Solution Finished\tTime Ran (" << timer.getElapsedTime() << "ms)\t Stepped (" << m_Graph->getStepCount() << ")\n";
    }
    
    void Application::runMousePressed(int x, int y)
    {
        m_Graph->setMousePressed(x, y);
    }

    void Application::runMouseReleased()
    {
        m_Graph->setMouseReleased();
    }

    void Application::runMouseMoved(int x, int y)
    {
        m_Graph->setMouseMoved(x, y);
    }
    
    void Application::handleAnimationTimer(MazeNode*& node)
    {
        if(!node->isNext() && m_Window->getAnimationSpeedValue() != 100)
        {
            m_AnimationTimer.run();
        }
    }








    /***********************************************************************************************************/
    
    bool Application::isMouseInsideSimBounds(int x, int y)
    {
        // actual displayed size 
        int displayLengthUsed = (WALL_WIDTH + m_Graph->getCellSize()) * m_Graph->getMazeLength();

        bool isInsideXBounds = (DISPLAY_LEFT_BUFFER + WALL_WIDTH < x && x < DISPLAY_LEFT_BUFFER + displayLengthUsed);
        bool isInsideYBounds = (DISPLAY_TOP_BUFFER + WALL_WIDTH < y && y < DISPLAY_TOP_BUFFER + displayLengthUsed);

        return (isInsideXBounds && isInsideYBounds);
    }
    
    void Application::updateMazeLength() 
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

    bool Application::isValidMazeLength(std::string gtf_MazeLength) const
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
    
    bool Application::isAnInteger(std::string str) const
    {
        for(char c : str) 
        {
            if(!isdigit(c))
                return false;
        }

        return true;
    }

    
    void Application::updateCPButtons(bool isIncrementingPositively)
    {        
        // enable if currently disabled
        if(!isIncrementingPositively && !m_Window->isAddCPEnabled())
        {
            m_Window->setAddCPEnabled(true);
        }
        else if(isIncrementingPositively && !m_Window->isRemoveCPEnabled())
        {
            m_Window->setRemoveCPEnabled(true);
        }

        // check to disable addCP
        if(m_Graph->getCheckpointCount() == CHECKPOINT_LIMIT - 1 && isIncrementingPositively)
        {
            m_Window->setAddCPEnabled(false);
        }
        // check to disable removeCP
        else if(m_Graph->getCheckpointCount() <= 1 && !isIncrementingPositively)
        {
            m_Window->setRemoveCPEnabled(false);
        }
    }

}