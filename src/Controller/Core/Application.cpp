#include "application.h"

namespace PFSim {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() 
    {
        m_Window = std::unique_ptr<Window>(new Window());
        m_Window->setEventCallback(BIND_EVENT_FN(onEvent));
        
        m_Graph = std::unique_ptr<MazeGraph>(new MazeGraph());

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
        if(m_Graph->isMouseInteractive())
        {   
            if(isMouseInsideSimBounds( e.getX(), e.getY() ))
            {
                runMouseMoved( e.getX(), e.getY() );

                std::stack<MazeNode*> swapCells = m_Graph->getNodesToSwap();

                if(swapCells.size() > 0)
                {
                    MazeNode* node = swapCells.top();
                    swapCells.pop();

                    m_Window->getSimulationDisplay()->updateMazeNode( node, m_Graph->getCellSize(), false );
                    m_Window->getSimulationDisplay()->updateMazeNode( swapCells.top(), m_Graph->getCellSize(), false );
                }
            }
            else
            {
                runMouseReleased();
            }
            
        }

        return true;
    }

    void Application::runGenerator(GeneratorType type)
    {
        updateMazeLength();             // the updateMazeLength() full logic might need to be moved into graph
        m_Graph->setGenerator(type);
        updateCPButtons(false);
        
        m_Window->getSimulationDisplay()->clearDisplay();

        while(!m_Graph->isAnimationComplete()) 
        {
            runTimer( m_Graph->getAnimationType(), m_Graph->getMazeLength() );  // Commenting this out removes the Window Not Responding Freezing Error
            
            m_Window->getSimulationDisplay()->updateMazeNode( m_Graph->updateAnimation(), m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
        }
        
        std::cout << "Generation Finished" << std::endl;
    }

    void Application::runPathfindingSimulation(PathfinderType type)
    {
        m_Graph->updateSimulationSetup();

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
        
        std::cout << "Simulation Finished" << std::endl;
    }
    
    void Application::runPathfinder(PathfinderType type)
    {
        m_Graph->setPathfinder(type);

        while(!m_Graph->isAnimationComplete()) 
        {
            runTimer( m_Graph->getAnimationType(), m_Graph->getMazeLength() );  // Commenting this out removes the Window Not Responding Freezing Error
            
            m_Window->getSimulationDisplay()->updateMazeNode( m_Graph->updateAnimation(), m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
        }

        m_Graph->updatePathfinderStart();
        
        std::cout << "Pathfinding Finished" << std::endl;
    }

    void Application::runGraphReset()
    {
        m_Graph->setGraphReset();

        int lastMarkerPosition = 0;
        while(!m_Graph->isAnimationComplete()) 
        {
            runTimer( m_Graph->getAnimationType(), m_Graph->getMazeLength() );  // Commenting this out removes the Window Not Responding Freezing Error

            MazeNode*& node = m_Graph->updateAnimation();
            
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
        
        std::cout << "Reseting Finished" << std::endl;
    }

    void Application::runPathSolution()
    {
        // set path solution animation
        m_Graph->setPathSolution();

        while(!m_Graph->isAnimationComplete()) 
        {
            runTimer( m_Graph->getAnimationType(), m_Graph->getMazeLength() );  // Commenting this out removes the Window Not Responding Freezing Error

            // path solution update
            m_Window->getSimulationDisplay()->updatePathNode( m_Graph->updateAnimation(), m_Graph->getCellSize() );
        }
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








    /***********************************************************************************************************/
    
    bool Application::isMouseInsideSimBounds(int x, int y)
    {
        bool isInsideXBounds = (DISPLAY_LEFT_BUFFER < x && x < DISPLAY_RIGHT_BUFFER);
        bool isInsideYBounds = (DISPLAY_TOP_BUFFER < y && y < DISPLAY_BOTTOM_BUFFER);

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



    void Application::runTimer(AnimationType type, int mazeLength) const {
        double lengthMultiplier;
        double updateTime;

        if(type == DrawPath) {
            updateTime = DEFAULT_ANIMATION_SPEED * 2;
            lengthMultiplier = 10 / pow((mazeLength), 1.05);
        }
        else if(type == Reset) {
            updateTime = DEFAULT_ANIMATION_SPEED / 15;
            lengthMultiplier = 10 / pow((mazeLength), 1.25);
        }
        else {//Generators and Pathfinders
            updateTime = DEFAULT_ANIMATION_SPEED;
            lengthMultiplier = 10 / pow((mazeLength), 1.25);
        }
        
        // speed mulptiplier to account for the length of the maze.
        if(lengthMultiplier != 0) {
            updateTime *= lengthMultiplier;
        }

        updateTime *= CLOCKS_PER_SEC;
        clock_t nowTime = clock();
        while(clock() - nowTime < updateTime);
    }

}