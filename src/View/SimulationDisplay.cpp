#include "SimulationDisplay.h"

namespace PFSim {

    void SimulationDisplay::clearDisplay() 
    {
        m_Window->setColor(BACKGROUND_WINDOW_COLOR);
        m_Window->fillRect(DISPLAY_LEFT_BUFFER, DISPLAY_TOP_BUFFER, DISPLAY_SIZE, DISPLAY_SIZE);
    }

}