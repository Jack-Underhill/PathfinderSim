#include "application.h"

namespace PFSim {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() 
    {
        m_Window = std::unique_ptr<Window>(new Window());
        m_Window->setEventCallback(BIND_EVENT_FN(onEvent));
        
        m_Graph = std::unique_ptr<MazeGraph>(new MazeGraph());

        m_Graph->setGeneratorOpen(DEFAULT_MAZE_LENGTH);
        runAnimation();
    }

    Application::~Application() 
    {
    }

    void Application::run() 
    {
        while(m_Window->isRunning())
        {
            // m_Window->OnUpdate();
        }
    }

    void Application::onEvent(Event& e) 
    {
        std::cout << "Event Fired: " << e << std::endl;

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<UpdateCheckpointEvent>(BIND_EVENT_FN(onCheckpointEvent));
        dispatcher.Dispatch<UpdatePathfinderEvent>(BIND_EVENT_FN(onPathfinderEvent));
        dispatcher.Dispatch<UpdateGeneratorEvent>(BIND_EVENT_FN(onGeneratorEvent));
    }

    bool Application::onCheckpointEvent(UpdateCheckpointEvent& e)
    {
        if(e.getButtonCode() == ButtonCode::cp_Add) 
        {
        }
        else if(e.getButtonCode() == ButtonCode::cp_Subtract) 
        {
        }

        return true;
    }

    bool Application::onPathfinderEvent(UpdatePathfinderEvent& e)
    {
        if(e.getButtonCode() == ButtonCode::pf_BFS) 
        {
            m_Graph->setPathfinderBFS();
        }
        else if(e.getButtonCode() == ButtonCode::pf_DFS) 
        {
            // m_Graph->setPathfinderDFS();
        }

        runAnimation();

        return true;
    }
            
    bool Application::onGeneratorEvent(UpdateGeneratorEvent& e) 
    {
        int mazeLength = updateMazeLength();

        m_Window->getSimulationDisplay()->clearDisplay();

        if(e.getButtonCode() == ButtonCode::gen_Open) 
        {
            m_Graph->setGeneratorOpen(mazeLength);
        }
        else if(e.getButtonCode() == ButtonCode::gen_DFS) 
        {
            // m_Graph->setGeneratorDFS(mazeLength);
        }

        runAnimation();

        return true;
    }
    
    void Application::runAnimation()
    {
        while(!m_Graph->isAnimationComplete()) {
            m_Window->getSimulationDisplay()->updateMazeNode( m_Graph->updateAnimation(), m_Graph->getCellSize() );
        }

        std::cout << "Animation Finished" << std::endl;
    }






    /***********************************************************************************************************/
    
    int Application::updateMazeLength() 
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

        return mazeLength;
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
            {
                return false;
            }
        }

        return true;
    }

}