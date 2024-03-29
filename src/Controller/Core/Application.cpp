#include "application.h"
// #include <chrono>

namespace PFSim {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() 
    {
        m_IsInstantRepathingEnabled = false;
        m_IsInstantRepathingValid = false;

        m_Window = new Window();
        m_Window->setEventCallback(BIND_EVENT_FN(onEvent));
        
        m_Graph = nullptr;
        m_MouseInteraction = nullptr;

        m_AnimationTimer = new AnimationTimer();

        SimulateGeneration gen(m_Graph, m_Window, m_AnimationTimer);
        gen.run(Open);
    }

    Application::~Application() 
    {
        delete m_Graph;
        delete m_Window;
        delete m_AnimationTimer;
    }

    void Application::run() 
    {
        //inifinitely keeping the project up
        while(m_Window->isRunning()) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void Application::onEvent(Event& e) 
    {
        // std::cout << "Event Fired: " << e << std::endl;

        // if(/*not currently computing an event then continue to trigger new event*/)
        // {
            EventDispatcher dispatcher(e);
            dispatcher.Dispatch<ButtonEvent>(BIND_EVENT_FN(onButtonEvent));
            dispatcher.Dispatch<CheckboxClickedEvent>(BIND_EVENT_FN(onCheckboxClickedEvent));
            dispatcher.Dispatch<SliderMovedEvent>(BIND_EVENT_FN(onSliderMovedEvent));

            dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(onMousePressedEvent));
            dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(onMouseReleasedEvent));
            dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(onMouseMovedEvent));
        // }
    }

    bool Application::onButtonEvent(ButtonEvent& e)
    {
        ButtonCode code = e.getButtonCode();
        
        if(code == ButtonCode::cp_Add || code == ButtonCode::cp_Subtract)
        {
            handleCheckpoint(code);
        }
        else if(code == ButtonCode::pf_BFS || code == ButtonCode::pf_DFS || code == ButtonCode::pf_AStar || code == ButtonCode::pf_SHP_BFS || code == ButtonCode::pf_SHP_AStar)
        {
            handlePathfinder(code);
        }
        else if(code == ButtonCode::gen_Open || code == ButtonCode::gen_DFS || code == ButtonCode::gen_Prims)
        {
            handleGenerator(code);
        }
        else if(code == ButtonCode::clear_Obstacles)
        {
            handleObstacles(/*code*/);
        }

        return true;
    }
    
    bool Application::onCheckboxClickedEvent(CheckboxClickedEvent& e)
    {
        m_IsInstantRepathingEnabled = e.isChecked();

        return true;
    }
    
    bool Application::onSliderMovedEvent(SliderMovedEvent& e)
    {
        int speedPercent = m_Window->getAnimationSpeedValue();
        m_Window->setSpeedText(speedPercent);

        //update animation timer
        m_AnimationTimer->updatePercentage(speedPercent);

        return true;
    }
    
    bool Application::onMousePressedEvent(MouseButtonPressedEvent& e)
    {
        if(isMouseInsideSimBounds( e.getX(), e.getY() ))
        {
            m_MouseInteraction = new InteractiveCells(e.getX(), e.getY(), m_Graph);

            CellType type = m_Graph->getNodeMap()->at( m_MouseInteraction->getMousePositionKey() )->getType();
            if(type == BlankCell || type == WallCell) 
            {
                handleMouseDrawingEvent();
            }
            else
            {
                handleMouseDraggingEvent();
            }
        }

        return true;
    }

    bool Application::onMouseReleasedEvent(MouseButtonReleasedEvent& e)
    {
        if(m_MouseInteraction != nullptr)
        {
            // update pointers if cells were moved
            if(m_MouseInteraction->isStartNodeMoved())
            {
                m_Graph->setStartNode( m_MouseInteraction->getNewStartNode() );
            }
            if(m_MouseInteraction->isEndNodeMoved())
            {
                m_Graph->setEndNode( m_MouseInteraction->getNewEndNode() );
            }

            delete m_MouseInteraction; 
            m_MouseInteraction = nullptr; 
        }

        return true;
    }

    bool Application::onMouseMovedEvent(MouseMovedEvent& e)
    {
        if(m_MouseInteraction != nullptr && isMouseInsideSimBounds( e.getX(), e.getY() ) && m_MouseInteraction->isNewPosition( e.getX(), e.getY() ))
        {
            if(m_MouseInteraction->setMouseMoved( e.getX(), e.getY() )) // continue if set was a success
            {
                std::stack<MazeNode*> updatedNodes = m_MouseInteraction->getMouseUpdatedNodes(); // change to array
                while(updatedNodes.size() > 0)
                {
                    m_Window->getSimulationDisplay()->updateMazeNode( updatedNodes.top(), m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
                    updatedNodes.pop();
                }

                if(m_IsInstantRepathingEnabled && m_IsInstantRepathingValid) 
                {
                    SimulateInstantPathfinding pf(m_Graph, m_Window, m_AnimationTimer);
                    pf.run(false, m_PathfinderType);
                }
            }
        }

        return true;
    }

    bool Application::handleCheckpoint(ButtonCode& code)
    {
        if(!m_Graph->isReadyForSimulation())
        {
            SimulatePathfinding pf(m_Graph, m_Window, m_AnimationTimer);
            pf.reset();
        }

        if(code == ButtonCode::cp_Add) 
        {
            updateCPButtons(true);
            
            m_Window->getSimulationDisplay()->updateMazeNode( m_Graph->updateCheckpoint(true), m_Graph->getCellSize(), false );
        }
        else if(code == ButtonCode::cp_Subtract) 
        {
            updateCPButtons(false);

            m_Window->getSimulationDisplay()->updateMazeNode( m_Graph->updateCheckpoint(false), m_Graph->getCellSize(), false );
        }

        return true;
    }

    bool Application::handlePathfinder(ButtonCode& code)
    {
        if(code == ButtonCode::pf_BFS) 
        {
            m_PathfinderType = BFS;
        }
        else if(code == ButtonCode::pf_DFS) 
        {
            m_PathfinderType = DFS;
        }
        else if(code == ButtonCode::pf_AStar) 
        {
            m_PathfinderType = AStar;
        }
        else if(code == ButtonCode::pf_SHP_BFS) 
        {
            m_PathfinderType = SHP_BFS;
        }
        else if(code == ButtonCode::pf_SHP_AStar) 
        {
            m_PathfinderType = SHP_AStar;
        }

        SimulatePathfinding pf(m_Graph, m_Window, m_AnimationTimer);
        pf.run(true, m_PathfinderType);

        return true;
    }

    bool Application::handleGenerator(ButtonCode& code) 
    {
        GeneratorType type;

        if(code == ButtonCode::gen_Open) 
        {
            type = Open;
        }
        else if(code == ButtonCode::gen_DFS) 
        {
            type = DFSMaze;
        }
        else if(code == ButtonCode::gen_Prims)
        {
            type = Prims;
        }

        SimulateGeneration gen(m_Graph, m_Window, m_AnimationTimer);
        gen.run(type);
        updateCPButtons(false);

        return true;
    }
    
    bool Application::handleObstacles(/*ButtonCode& code*/)
    {
        SimulatePathfinding pf(m_Graph, m_Window, m_AnimationTimer);
        pf.clearObstacles();

        return true;
    }

    void Application::handleMouseDrawingEvent()
    {
        if(!m_Graph->isMazeGenerated())
        {
            // setup
            m_IsInstantRepathingValid = false;
            if(!m_Graph->isReadyForSimulation())
            {
                SimulatePathfinding pf(m_Graph, m_Window, m_AnimationTimer);
                pf.reset();
            }

            // execute drawEvent
            CellType type = m_Graph->getNodeMap()->at( m_MouseInteraction->getMousePositionKey() )->getType();
            if(type == BlankCell) 
            {
                m_MouseInteraction->setDrawWallsMode(true);
            }
            else if(type == WallCell) 
            {
                m_MouseInteraction->setEraseWallsMode(true);
            }
            m_Window->getSimulationDisplay()->updateMazeNode( m_MouseInteraction->getMouseUpdatedNodes().top(), m_Graph->getCellSize(), m_Graph->isMazeGenerated() );
        }
        else
        {
            delete m_MouseInteraction; 
            m_MouseInteraction = nullptr; 
        }
    }

    void Application::handleMouseDraggingEvent()
    {
        if(m_IsInstantRepathingEnabled)
        {
            m_IsInstantRepathingValid = !m_Graph->isReadyForSimulation();
        }
        else if(!m_Graph->isReadyForSimulation())
        {
            SimulatePathfinding pf(m_Graph, m_Window, m_AnimationTimer);
            pf.reset();
        }
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
        if(m_Graph->getTargetCount() == CHECKPOINT_LIMIT - 1 && isIncrementingPositively)
        {
            m_Window->setAddCPEnabled(false);
        }
        // check to disable removeCP
        else if(m_Graph->getTargetCount() <= 1 && !isIncrementingPositively)
        {
            m_Window->setRemoveCPEnabled(false);
        }
    }
    
    bool Application::isMouseInsideSimBounds(int x, int y)
    {
        // actual displayed size 
        int displayLengthUsed = (WALL_WIDTH + m_Graph->getCellSize()) * m_Graph->getMazeLength();

        bool isInsideXBounds = (DISPLAY_LEFT_BUFFER + WALL_WIDTH < x && x < DISPLAY_LEFT_BUFFER + displayLengthUsed);
        bool isInsideYBounds = (DISPLAY_TOP_BUFFER + WALL_WIDTH < y && y < DISPLAY_TOP_BUFFER + displayLengthUsed);

        return (isInsideXBounds && isInsideYBounds);
    }

}