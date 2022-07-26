#include "MainUI.h"

void MainUI::write(string s) {
    cout << s << endl;
}

string MainUI::read() {
    string s;
    getline(cin, s);
    return s;
};