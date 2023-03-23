#ifndef _ButtonCodes_h_
#define _ButtonCodes_h_

namespace PFSim {

    enum ButtonCode
    {
        cp_Add          = 0,
        cp_Subtract     = 1,

        pf_BFS          = 2,
        pf_DFS          = 3,
        pf_AStar        = 4,
        pf_SHP_BFS      = 5,
        pf_SHP_AStar    = 6,

        gen_Open        = 7,
        gen_DFS         = 8, 
        gen_Prims       = 9, 
        
        clear_Obstacles = 10
    };

}


#endif