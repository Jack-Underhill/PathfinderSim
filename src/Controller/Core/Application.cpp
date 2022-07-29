#include "application.h"

namespace PFSim {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() 
    {
        m_Window = std::unique_ptr<Window>(new Window());
        m_Window->setEventCallback(BIND_EVENT_FN(onEvent));
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
        dispatcher.Dispatch<UpdateMazeLengthEvent>(BIND_EVENT_FN(onMazeLengthEvent));

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
        // run gen alg

        // std::string s;
        if(e.getButtonCode() == ButtonCode::gen_Open) 
        {
            // m_Window->getSimulationDisplay()->paintDisplay("blue");
        }
        else if(e.getButtonCode() == ButtonCode::gen_DFS) 
        {
            // m_Window->getSimulationDisplay()->paintDisplay("red");
        }

        // std::cout << std::endl << std::endl << "RUNNING GENERATOR ALGORITHM\t\t" << s << std::endl << std::endl;

        return true;
    }

    bool Application::onMazeLengthEvent(UpdateMazeLengthEvent& e) 
    {
        // set new mazelength
        // m_Window->getSimulationDisplay()->clearDisplay();

        return true;
    }

}