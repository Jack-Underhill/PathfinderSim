#ifndef _Constants_h_
#define _Constants_h_

#include <string>

namespace PFSim {

    const int WALL_WIDTH = 2;
    const int DEFAULT_MAZE_LENGTH = 10;  
    const int MINIMUM_MAZE_LENGTH = 4;
    const int MAXIMUM_MAZE_LENGTH = 70;
    const int CHECKPOINT_LIMIT = 5;
    const double DEFAULT_ANIMATION_SPEED = 0.015;// second per frame

    const std::string BACKGROUND_WINDOW_COLOR = "black";
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 840;
    
    const int DISPLAY_SIZE = 700;
    const int DISPLAY_TOP_BUFFER = 100;
    const int DISPLAY_BOTTOM_BUFFER = WINDOW_HEIGHT - DISPLAY_TOP_BUFFER;
    const int DISPLAY_LEFT_BUFFER = (WINDOW_WIDTH - DISPLAY_SIZE) / 2;
    const int DISPLAY_RIGHT_BUFFER = DISPLAY_LEFT_BUFFER + DISPLAY_SIZE;

}

#endif