#include "O2/Testing/Playtests/LibSdlPlaytests.h"
#include "O2/Testing/Playtests/WindowPlaytests.h"
#include "O2/Testing/Playtests/LibImGuiPlaytests.h"
#include "O2/Testing/Playtests/PlaytestMenuWindow.h"
#include "O2/Core/Events.h"
#include <iostream>
using namespace std;
using namespace o2;

void playtestMenuWindowTest()
{
    PlaytestMenuWindow playtestMenuWindow;
    while (playtestMenuWindow.isOpen()) {
        Events::update();
        playtestMenuWindow.update();
        playtestMenuWindow.display();
    }
}

int main(int argc, char* argv[])
{
    playtestMenuWindowTest();
    //my::LibImGuiPlaytests::drawTree();
    //my::LibImGuiPlaytests::glWindowAndImGuiWindow();
    //my::LibImGuiPlaytests::simpleGui();
    //my::WindowPlaytests::twoWindows();
    //my::WindowPlaytests::simpleWindow();
    //my::LibSdlPlaytests::twoOpenGlWindows();
    //my::LibSdlPlaytests::openGlWindow(); 
    //my::LibSdlPlaytests::simpleWindow();
    return 0;
}