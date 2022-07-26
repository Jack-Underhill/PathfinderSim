#ifndef _MainController_h_
#define _MainController_h_

#include "MazeNode.h"
#include "MainUI.h"

using namespace std;

class MainController {
public:
    MainController(MainUI* _ui, MazeNode* _n);
    void run();

private:
    MainUI* ui;
    MazeNode* n;
};

#endif