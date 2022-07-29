#ifndef _Constants_h_
#define _Constants_h_

#include <string>

namespace PFSim {

    const int DEFAULT_MAZE_LENGTH = 10;  

    const std::string BACKGROUND_WINDOW_COLOR = "black";
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 840;
    
    const int DISPLAY_SIZE = 700;
    const int DISPLAY_BOTTOM_BUFFER = 40;
    const int DISPLAY_TOP_BUFFER = WINDOW_HEIGHT - (DISPLAY_SIZE + DISPLAY_BOTTOM_BUFFER);
    const int DISPLAY_LEFT_BUFFER = (WINDOW_WIDTH - DISPLAY_SIZE) / 2;

}

#endif