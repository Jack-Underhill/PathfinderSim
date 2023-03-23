#include "Window.h"

namespace PFSim {

    Window::Window(const WindowProps& props) 
    { 
        Init(props); 
        m_SimDisplay = new SimulationDisplay(m_Window);
        m_StatDisplay = new StatisticsDisplay(m_Window);
    }

    Window::~Window()
    {
        while(!m_Interactors.empty()) 
        {
            delete m_Interactors.top();
            m_Interactors.pop();
        }

        delete m_SimDisplay;
        delete m_StatDisplay;

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
        loadPanelSpeed();
        loadPanelExtras();
        loadMouseListeners();

        // Draw cell names
        loadCellNames();
    }

    void Window::loadPanelCheckpoint() 
    {
        Panel p_Checkpoint("Specify Checkpoints");
        addSpacer(p_Checkpoint);


        btn_AddCP = new sgl::GButton("Add Checkpoint");
        btn_AddCP->setActionListener([this] 
        {
            WindowData& data = this->m_Data;

            ButtonEvent event(ButtonCode::cp_Add);
            data.EventCallback(event);
        });
        p_Checkpoint.addInteractor(btn_AddCP);
        m_Interactors.push(btn_AddCP);
        

        btn_RemoveCP = new sgl::GButton("Remove Checkpoint");
        btn_RemoveCP->setActionListener([this] 
        {
            WindowData& data = this->m_Data; 

            ButtonEvent event(ButtonCode::cp_Subtract);
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


        sgl::GButton* btn_pfDFS = new sgl::GButton("Run DFS");
        btn_pfDFS->setActionListener([this] 
        {
            WindowData& data = this->m_Data;

            ButtonEvent event(ButtonCode::pf_DFS);
            data.EventCallback(event);
        });
        p_Pathfinder.addInteractor(btn_pfDFS);
        m_Interactors.push(btn_pfDFS);


        sgl::GButton* btn_pfBFS = new sgl::GButton("Run BFS");
        btn_pfBFS->setActionListener([this] 
        {
            WindowData& data = this->m_Data;

            ButtonEvent event(ButtonCode::pf_BFS);
            data.EventCallback(event);
        });
        p_Pathfinder.addInteractor(btn_pfBFS);
        m_Interactors.push(btn_pfBFS);


        sgl::GButton* btn_pfAStar = new sgl::GButton("Run AStar");
        btn_pfAStar->setActionListener([this] 
        {
            WindowData& data = this->m_Data;

            ButtonEvent event(ButtonCode::pf_AStar);
            data.EventCallback(event);
        });
        p_Pathfinder.addInteractor(btn_pfAStar);
        m_Interactors.push(btn_pfAStar);


        sgl::GButton* btn_pfSHP_BFS = new sgl::GButton("Run SHP-BFS");
        btn_pfSHP_BFS->setActionListener([this] 
        {
            WindowData& data = this->m_Data;

            ButtonEvent event(ButtonCode::pf_SHP_BFS);
            data.EventCallback(event);
        });
        p_Pathfinder.addInteractor(btn_pfSHP_BFS);
        m_Interactors.push(btn_pfSHP_BFS);


        sgl::GButton* btn_pfSHP_AStar = new sgl::GButton("Run SHP-AStar");
        btn_pfSHP_AStar->setActionListener([this] 
        {
            WindowData& data = this->m_Data;

            ButtonEvent event(ButtonCode::pf_SHP_AStar);
            data.EventCallback(event);
        });
        p_Pathfinder.addInteractor(btn_pfSHP_AStar);
        m_Interactors.push(btn_pfSHP_AStar);


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

            ButtonEvent event(ButtonCode::gen_Open);
            data.EventCallback(event);
        });
        p_Generator.addInteractor(btn_genOpen);
        m_Interactors.push(btn_genOpen);

        
        sgl::GButton* btn_genDFS = new sgl::GButton("Run DFS Maze");
        btn_genDFS->setActionListener([this] 
        {
            WindowData& data = this->m_Data; 

            ButtonEvent event(ButtonCode::gen_DFS);
            data.EventCallback(event);
        });
        p_Generator.addInteractor(btn_genDFS);
        m_Interactors.push(btn_genDFS);

        
        sgl::GButton* btn_genPrim = new sgl::GButton("Run Prim's Maze");
        btn_genPrim->setActionListener([this] 
        {
            WindowData& data = this->m_Data; 

            ButtonEvent event(ButtonCode::gen_Prims);
            data.EventCallback(event);
        });
        p_Generator.addInteractor(btn_genPrim);
        m_Interactors.push(btn_genPrim);


        m_Window->addToRegion(p_Generator.getPanel(), sgl::GWindow::REGION_EAST);
    }
    
    void Window::loadPanelSpeed()
    {
        Panel p_Speed("Animation Speed");

        addSpacer(p_Speed);

        int defaultSpeedValue = 50;
        std::stringstream ss;
        ss << defaultSpeedValue << "%";
        
        lbl_SpeedValue = new sgl::GLabel(ss.str());
        p_Speed.addInteractor(lbl_SpeedValue);
        m_Interactors.push(lbl_SpeedValue);

        sld_Speed = new sgl::GSlider(1, 100, defaultSpeedValue);
        sld_Speed->setActionListener([this] 
        {
            WindowData& data = this->m_Data; 

            SliderMovedEvent event(SliderCode::AnimationSpeed);
            data.EventCallback(event);
        });
        p_Speed.addInteractor(sld_Speed);
        m_Interactors.push(sld_Speed);

        m_Window->addToRegion(p_Speed.getPanel(), sgl::GWindow::REGION_EAST);
    }
    
    void Window::loadPanelExtras()
    {
        Panel p_Checkboxes("Extra Settings");
        addSpacer(p_Checkboxes);

        // sgl::GLabel* lbl_desc = new sgl::GLabel("Triggered from moving cells after simulated.");
        // p_Checkboxes.addInteractor(lbl_desc);
        // m_Interactors.push(lbl_desc);

        cb_InstantSim = new sgl::GCheckBox("Enable Instant Repathing");
        cb_InstantSim->setActionListener([this](sgl::GEvent e)
        {
            WindowData& data = this->m_Data; 

            CheckboxClickedEvent event(CheckboxCode::InstantRepathing, this->cb_InstantSim->isChecked());
            data.EventCallback(event);
        });
        p_Checkboxes.addInteractor(cb_InstantSim);
        m_Interactors.push(cb_InstantSim);
        
        addSpacer(p_Checkboxes);
        
        sgl::GButton* btn_ClearObstacles = new sgl::GButton("Clear Obstacles");
        btn_ClearObstacles->setActionListener([this] 
        {
            WindowData& data = this->m_Data; 

            ButtonEvent event(ButtonCode::clear_Obstacles);
            data.EventCallback(event);
        });
        p_Checkboxes.addInteractor(btn_ClearObstacles);
        m_Interactors.push(btn_ClearObstacles);

        m_Window->addToRegion(p_Checkboxes.getPanel(), sgl::GWindow::REGION_EAST);
    }

    void Window::addSpacer(Panel& panel) 
    {
        sgl::GLabel* lbl_spacer = new sgl::GLabel();
        panel.addInteractor(lbl_spacer);
        m_Interactors.push(lbl_spacer);
    }
    
    void Window::loadMouseListeners()
    {
        m_Window->setMouseListener([this](sgl::GEvent e)
        {
            if(e.getEventType() == sgl::MOUSE_PRESSED)
            {
                WindowData& data = this->m_Data; 

                MouseButtonPressedEvent event(MouseCode::MousePressed, e.getX(), e.getY());
                data.EventCallback(event);
            }
            else if(e.getEventType() == sgl::MOUSE_RELEASED) 
            {
                WindowData& data = this->m_Data; 

                MouseButtonReleasedEvent event(MouseCode::MouseReleased, e.getX(), e.getY());
                data.EventCallback(event);
            }
            else if(e.getEventType() == sgl::MOUSE_MOVED) 
            {
                WindowData& data = this->m_Data; 

                MouseMovedEvent event(MouseCode::MouseMoved, e.getX(), e.getY());
                data.EventCallback(event);
            }
        });
    }

    void Window::setSpeedText(int value) 
    { 
        std::stringstream ss;
        ss << value << "%";
        lbl_SpeedValue->setText(ss.str()); 
    }
    

    void Window::loadCellNames()
    {
        int fontSize = DISPLAY_TOP_BUFFER / 8;

        std::string fontStr = FONT_STYLE + "-" + std::to_string(fontSize);
        m_Window->setFont(fontStr);


        int iterator = 0;

        drawCellName(GenerationCell, iterator);
        drawCellName(BlankCell, iterator);
        drawCellName(StartCell, iterator);
        drawCellName(EndCell, iterator);
        drawCellName(CheckpointCell, iterator);
        drawCellName(WallCell, iterator);

        drawCellName(BlankCell, iterator, true, false, false);
        drawCellName(BlankCell, iterator, false, true, false);
        drawCellName(BlankCell, iterator, false, false, true);
    }
    
    void Window::drawCellName(CellType type, int& iterator, bool isNext, bool isVisited, bool isPath)
    {
        std::string color = getNodeColor(type, isNext, isVisited, isPath);
        std::string name = getTypeString(type, isNext, isVisited, isPath);

        int spacing = DISPLAY_SIZE / 11;
        int size = DISPLAY_SIZE / 15;

        int xForCell = DISPLAY_LEFT_BUFFER / 5;
        int xForName = xForCell + (size * 3 / 2);

        int y = (iterator * spacing) + DISPLAY_TOP_BUFFER + (spacing);

        m_Window->setColor(color);
        m_Window->drawString(name, xForName, y + (spacing / 2));
        m_Window->fillRect(xForCell, y, size, size);

        iterator++;
    }

} //namespace PFSim