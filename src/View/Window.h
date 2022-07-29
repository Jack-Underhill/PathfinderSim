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

#include "panel.h"
#include "event.h"
#include "buttonevent.h"
#include "textfieldevent.h"
#include "textfieldcodes.h"
#include "buttoncodes.h"

namespace PFSim {

    struct WindowProps
    {
        std::string Title;
        unsigned int Width, Height;

        WindowProps(const std::string& title = "Pathfinder Simulator",
                    unsigned int width = 1280,
                    unsigned int height = 820)
                    : Title(title), Width(width), Height(height)
        {
        }
    };
    
    class Window
    {
        const std::string BACKGROUND_COLOR = "black";
        const int DEFAULT_MAZE_LENGTH = 10;
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        Window(const WindowProps& props = WindowProps()) { Init(props); };
        ~Window();

        unsigned int getWidth() { return m_Data.width; };
        unsigned int getHeight() { return m_Data.height; };

		void setEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

    private:
        void Init(const WindowProps& props);
        void loadPanelCheckpoint();
        void loadPanelPathfinder();
        void loadPanelGenerator();
        void addSpacer(Panel& panel);


        sgl::GWindow* m_Window;

        struct WindowData
        {
            std::string title;
            unsigned int width, height;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

}

#endif