#ifndef _Constants_h_
#define _Constants_h_

#include <string>

namespace PFSim {

    const int WALL_WIDTH = 3;
    const int DEFAULT_MAZE_LENGTH = 15;  
    const int MINIMUM_MAZE_LENGTH = 5;
    const int MAXIMUM_MAZE_LENGTH = 50;
    const int CHECKPOINT_LIMIT = 5;

    const std::string BACKGROUND_WINDOW_COLOR = "black";
    const int WINDOW_WIDTH = 1750;
    const int WINDOW_HEIGHT = 950;
    
    const int DISPLAY_SIZE = WINDOW_HEIGHT * ( 5.0 / 6 );
    const int DISPLAY_TOP_BUFFER = WINDOW_HEIGHT / 8.0;
    const int DISPLAY_BOTTOM_BUFFER = DISPLAY_TOP_BUFFER + DISPLAY_SIZE;
    const int DISPLAY_LEFT_BUFFER = (WINDOW_WIDTH - DISPLAY_SIZE) / 2;
    const int DISPLAY_RIGHT_BUFFER = DISPLAY_LEFT_BUFFER + DISPLAY_SIZE;

}

#endif