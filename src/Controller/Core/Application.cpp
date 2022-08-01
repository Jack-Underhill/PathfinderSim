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
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<UpdateCheckpointEvent>(BIND_EVENT_FN(onCheckpointEvent));
        dispatcher.Dispatch<UpdatePathfinderEvent>(BIND_EVENT_FN(onPathfinderEvent));
        dispatcher.Dispatch<UpdateGeneratorEvent>(BIND_EVENT_FN(onGeneratorEvent));
        // dispatcher.Dispatch<UpdateMazeLengthEvent>(BIND_EVENT_FN(onMazeLengthEvent));

        std::cout << e << std::endl;
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
        }
        else if(e.getButtonCode() == ButtonCode::pf_DFS) 
        {
        }

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

    // bool Application::onMazeLengthEvent(UpdateMazeLengthEvent& e) 
    // {
    //     // set new mazelength
    //     // m_Window->getSimulationDisplay()->clearDisplay();

    //     return true;
    // }
    
    void Application::runAnimation()
    {
        while(!m_Graph->isAnimationComplete()) {
            m_Window->getSimulationDisplay()->updateMazeNode( m_Graph->updateAnimation(), m_Graph->getCellSize() );
        }
        std::cout << "Animation Finished" << std::endl;
    }
    
    int Application::updateMazeLength() 
    {
        int mazeLength;
        sgl::GTextField* gtf_MazeLength = m_Window->getGTFMazeLength();

        if(isValidMazeLength(gtf_MazeLength))
        {
            mazeLength = std::stoi(gtf_MazeLength->getText());
        }
        else
        {
            mazeLength = m_Graph->getMazeLength();
            gtf_MazeLength->setText(std::to_string(mazeLength));
        }

        return mazeLength;
    }

    bool Application::isValidMazeLength(sgl::GTextField*& gtf_MazeLength) const
    {
        if(gtf_MazeLength->getText().size() == 0) 
        {
            return false;
        }
        else 
        {
            bool isInteger = gtf_MazeLength->valueIsInteger();
            bool isInsideLowerBound = MINIMUM_MAZE_LENGTH <= std::stoi(gtf_MazeLength->getText());
            bool isInsideUpperBound = std::stoi(gtf_MazeLength->getText()) <= MAXIMUM_MAZE_LENGTH;

            return (isInteger && isInsideLowerBound && isInsideUpperBound);
        }
    }

}