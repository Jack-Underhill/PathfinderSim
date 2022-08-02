#include "Window.h"

namespace PFSim {

    Window::Window(const WindowProps& props) 
    { 
        Init(props); 
        m_SimDisplay = new SimulationDisplay(m_Window);
    }

    Window::~Window()
    {
        delete m_SimDisplay;

        while(!m_Interactors.empty()) 
        {
            delete m_Interactors.top();
            m_Interactors.pop();
        }

        delete m_Window;
    }

    void Window::Init(const WindowProps& props) 
    {
        m_Data.title = props.Title;
        m_Data.width = props.Width;
        m_Data.height = props.Height;
        
        m_Window = new sgl::GWindow(m_Data.width, m_Data.height);
        m_Window->setWindowTitle(m_Data.title);

        m_Window->setResizable(false);
        m_Window->setAutoRepaint(true);
        m_Window->toFront();
        m_Window->setBackground(BACKGROUND_WINDOW_COLOR);

        // Set callbacks
        loadPanelCheckpoint();
        loadPanelPathfinder();
        loadPanelGenerator();
    }

    void Window::loadPanelCheckpoint() 
    {
        Panel p_Checkpoint("Specify Checkpoints");
        addSpacer(p_Checkpoint);


        btn_AddCP = new sgl::GButton("Add Checkpoint");
        btn_AddCP->setActionListener([this] 
        {
            WindowData& data = this->m_Data;

            UpdateCheckpointEvent event(ButtonCode::cp_Add);
            data.EventCallback(event);
        });
        p_Checkpoint.addInteractor(btn_AddCP);
        m_Interactors.push(btn_AddCP);
        

        btn_RemoveCP = new sgl::GButton("Remove Checkpoint");
        btn_RemoveCP->setActionListener([this] 
        {
            WindowData& data = this->m_Data; 

            UpdateCheckpointEvent event(ButtonCode::cp_Subtract);
            data.EventCallback(event);
        });
        btn_RemoveCP->setEnabled(false);
        p_Checkpoint.addInteractor(btn_RemoveCP);
        m_Interactors.push(btn_RemoveCP);

        
        m_Window->addToRegion(p_Checkpoint.getPanel(), sgl::GWindow::REGION_EAST);
    }

    void Window::loadPanelPathfinder() 
    {
        Panel p_Pathfinder("Specify Pathfinder");
        addSpacer(p_Pathfinder);


        sgl::GButton* btn_pfBFS = new sgl::GButton("Run BFS");
        btn_pfBFS->setActionListener([this] 
        {
            WindowData& data = this->m_Data;

            UpdatePathfinderEvent event(ButtonCode::pf_BFS);
            data.EventCallback(event);
        });
        p_Pathfinder.addInteractor(btn_pfBFS);
        m_Interactors.push(btn_pfBFS);


        sgl::GButton* btn_pfDFS = new sgl::GButton("Run DFS");
        btn_pfDFS->setActionListener([this] 
        {
            WindowData& data = this->m_Data;

            UpdatePathfinderEvent event(ButtonCode::pf_DFS);
            data.EventCallback(event);
        });
        p_Pathfinder.addInteractor(btn_pfDFS);
        m_Interactors.push(btn_pfDFS);


        m_Window->addToRegion(p_Pathfinder.getPanel(), sgl::GWindow::REGION_EAST);
    }

    void Window::loadPanelGenerator() 
    {
        Panel p_Generator("Specify Generator");
        addSpacer(p_Generator);


        sgl::GLabel* lbl_desc = new sgl::GLabel("Length of Maze Graph");
        p_Generator.addInteractor(lbl_desc);
        m_Interactors.push(lbl_desc);
        std::string lengthRange = std::to_string(MINIMUM_MAZE_LENGTH) + "-" + std::to_string(MAXIMUM_MAZE_LENGTH);
        sgl::GLabel* lbl_bounds = new sgl::GLabel("(Integer from " + lengthRange + ")");
        p_Generator.addInteractor(lbl_bounds);
        m_Interactors.push(lbl_bounds);

        tf_MazeLength = new sgl::GTextField(std::to_string(DEFAULT_MAZE_LENGTH));
        p_Generator.addInteractor(tf_MazeLength);
        m_Interactors.push(tf_MazeLength);


        addSpacer(p_Generator);


        sgl::GButton* btn_genOpen = new sgl::GButton("Run Open");
        btn_genOpen->setActionListener([this] 
        {
            WindowData& data = this->m_Data; 

            UpdateGeneratorEvent event(ButtonCode::gen_Open);
            data.EventCallback(event);
        });
        p_Generator.addInteractor(btn_genOpen);
        m_Interactors.push(btn_genOpen);

        
        sgl::GButton* btn_genDFS = new sgl::GButton("Run DFS");
        btn_genDFS->setActionListener([this] 
        {
            WindowData& data = this->m_Data; 

            UpdateGeneratorEvent event(ButtonCode::gen_DFS);
            data.EventCallback(event);
        });
        p_Generator.addInteractor(btn_genDFS);
        m_Interactors.push(btn_genDFS);


        m_Window->addToRegion(p_Generator.getPanel(), sgl::GWindow::REGION_EAST);
    }

    void Window::addSpacer(Panel& panel) 
    {
        sgl::GLabel* lbl_spacer = new sgl::GLabel();
        panel.addInteractor(lbl_spacer);
        m_Interactors.push(lbl_spacer);
    }

}