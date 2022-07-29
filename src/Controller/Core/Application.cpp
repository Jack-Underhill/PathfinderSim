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
        while(m_Running)
        {
            // m_Window->OnUpdate();
        }
    }

    void Application::onEvent(Event& e) 
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<UpdateGeneratorEvent>(BIND_EVENT_FN(onGeneratorEvent));

        std::cout << e << std::endl;
    }
    
    bool Application::onGeneratorEvent(UpdateGeneratorEvent& e) 
    {
        // run gen alg

        // std::string s;
        if(e.getButtonCode() == ButtonCode::gen_Open) 
        {
            // s = "OPEN";
        }
        else if(e.getButtonCode() == ButtonCode::gen_DFS) 
        {
            // s = "DFS";
        }

        // std::cout << std::endl << std::endl << "RUNNING GENERATOR ALGORITHM\t\t" << s << std::endl << std::endl;

        return true;
    }

}