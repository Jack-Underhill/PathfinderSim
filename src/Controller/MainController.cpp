#include "MainController.h"

MainController::MainController(MainUI* _ui, MazeNode* _n) {
    ui = _ui;
    n = _n;
}

void MainController::run() {
    ui->write("whats your name?");
    ui->write(n->reverseString(ui->read()));
    ui->read();
}