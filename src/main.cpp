#include "Application.h"

using namespace PFSim;

int main() {
    Application* app = new Application();
    app->run();
    delete app;

    return 0;
}