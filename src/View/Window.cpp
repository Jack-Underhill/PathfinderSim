#include "Window.h"

namespace PFSim {

    Window::Window(const WindowProps& props) 
    { 
        Init(props); 
        m_SimDisplay = std::shared_ptr<SimulationDisplay>
                       (new SimulationDisplay(m_Window));
    }

    Window::~Window()
    {
        delete m_Window;
        m_Window = nullptr;
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


        sgl::GButton* btn_AddCP = new sgl::GButton("Add Checkpoint");
        btn_AddCP->setActionListener([this] 
        {
            WindowData& data = this->m_Data;

            UpdateCheckpointEvent event(ButtonCode::cp_Add);
            data.EventCallback(event);
        });
        p_Checkpoint.addInteractor(btn_AddCP);
        

        sgl::GButton* btn_SubtractCP = new sgl::GButton("Remove Checkpoint");
        btn_SubtractCP->setActionListener([this] 
        {
            WindowData& data = this->m_Data; 

            UpdateCheckpointEvent event(ButtonCode::cp_Subtract);
            data.EventCallback(event);
        });
        p_Checkpoint.addInteractor(btn_SubtractCP);

        
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


        sgl::GButton* btn_pfDFS = new sgl::GButton("Run DFS");
        btn_pfDFS->setActionListener([this] 
        {
            WindowData& data = this->m_Data;

            UpdatePathfinderEvent event(ButtonCode::pf_DFS);
            data.EventCallback(event);
        });
        p_Pathfinder.addInteractor(btn_pfDFS);


        m_Window->addToRegion(p_Pathfinder.getPanel(), sgl::GWindow::REGION_EAST);
    }

    void Window::loadPanelGenerator() 
    {
        Panel p_Generator("Specify Generator");
        addSpacer(p_Generator);


        sgl::GLabel* lbl_desc = new sgl::GLabel("Length of Maze Graph");
        p_Generator.addInteractor(lbl_desc);
        sgl::GLabel* lbl_bounds = new sgl::GLabel("Integer from 3-70");
        p_Generator.addInteractor(lbl_bounds);

        sgl::GTextField* txtf_MazeLength = new sgl::GTextField(std::to_string(DEFAULT_MAZE_LENGTH));
        txtf_MazeLength->setTextChangeListener([this]
        {
            WindowData& data = this->m_Data; 

            UpdateMazeLengthEvent event(TextFieldCode::MazeLength);
            data.EventCallback(event);
        });
        p_Generator.addInteractor(txtf_MazeLength);


        addSpacer(p_Generator);


        sgl::GButton* btn_genOpen = new sgl::GButton("Run Open");
        btn_genOpen->setActionListener([this] 
        {
            WindowData& data = this->m_Data; 

            UpdateGeneratorEvent event(ButtonCode::gen_Open);
            data.EventCallback(event);
        });
        p_Generator.addInteractor(btn_genOpen);

        
        sgl::GButton* btn_genDFS = new sgl::GButton("Run DFS");
        btn_genDFS->setActionListener([this] 
        {
            WindowData& data = this->m_Data; 

            UpdateGeneratorEvent event(ButtonCode::gen_DFS);
            data.EventCallback(event);
        });
        p_Generator.addInteractor(btn_genDFS);


        m_Window->addToRegion(p_Generator.getPanel(), sgl::GWindow::REGION_EAST);
    }

    void Window::addSpacer(Panel& panel) 
    {
        sgl::GLabel* lbl_spacer = new sgl::GLabel();
        panel.addInteractor(lbl_spacer);
    }

}