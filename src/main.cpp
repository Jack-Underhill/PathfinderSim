#include "MainController.h"

int main() {
    MazeNode m;
    MainUI v;

    MainController c(&v, &m);
    c.run();

    return 0;
}