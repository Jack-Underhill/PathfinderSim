#ifndef _Window_h_
#define _Window_h_

#include <string>
#include <functional>

#include "gwindow.h"
#include "gevent.h"
#include "ginteractor.h"
#include "gbutton.h"
#include "glabel.h"
#include "gtextfield.h"

#include "Constants.h"

#include "SimulationDisplay.h"
#include "panel.h"
#include "event.h"
#include "buttonevent.h"
#include "buttoncodes.h"

namespace PFSim {

    struct WindowProps
    {
        std::string Title;
        unsigned int Width, Height;

        WindowProps(const std::string& title = "Pathfinder Simulator",
                    unsigned int width = WINDOW_WIDTH,
                    unsigned int height = WINDOW_HEIGHT)
                    : Title(title), Width(width), Height(height)
        {
        }
    };
    
    class Window
    {
        using EventCallbackFn = std::function<void(Event&)>;

    public:
        Window(const WindowProps& props = WindowProps());
        ~Window();

        bool isRunning() { return m_Window->isOpen(); }

        unsigned int getWidth() { return m_Data.width; };
        unsigned int getHeight() { return m_Data.height; };
        sgl::GWindow* getNativeWindow() const { return m_Window; };
        SimulationDisplay*& getSimulationDisplay() { return m_SimDisplay; };
        std::string getInputMazeLength() { return gtf_MazeLength->getText(); }

		void setEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
        void setInputMazeLength(int length) { gtf_MazeLength->setText( std::to_string(length) ); }

    private:
        sgl::GWindow* m_Window;
        SimulationDisplay* m_SimDisplay;
        sgl::GTextField* gtf_MazeLength;
        std::stack<sgl::GInteractor*> m_Interactors;

        struct WindowData
        {
            std::string title;
            unsigned int width, height;

            EventCallbackFn EventCallback;
        };
        WindowData m_Data;


        void Init(const WindowProps& props);
        void loadPanelCheckpoint();
        void loadPanelPathfinder();
        void loadPanelGenerator();
        void addSpacer(Panel& panel);
    };

}

#endif