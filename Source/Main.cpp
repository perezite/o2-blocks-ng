#include "O2/Testing/Playtests/LibSdlPlaytests.h"
#include "O2/Testing/Playtests/WindowPlaytests.h"
#include "O2/Testing/Playtests/LibImGuiPlaytests.h"
#include "O2/Testing/Playtests/PlaytestMenuWindow.h"
#include "O2/Testing/Playtests/PlaytestMenuWindowPlaytest.h"
using namespace o2;

int main(int argc, char* argv[])
{
    my::PlaytestMenuWindowPlaytest::showWindow();
    //my::LibImGuiPlaytests::treeView();
    //my::LibImGuiPlaytests::glWindowAndImGuiWindow();
    //my::LibImGuiPlaytests::simpleGui();
    //my::WindowPlaytests::twoWindows();
    //my::WindowPlaytests::simpleWindow();
    //my::LibSdlPlaytests::twoOpenGlWindows();
    //my::LibSdlPlaytests::openGlWindow(); 
    //my::LibSdlPlaytests::simpleWindow();
    return 0;
}