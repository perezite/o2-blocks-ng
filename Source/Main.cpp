#include "O2/Testing/Playtests/LibSdlPlaytests.h"
#include "O2/Testing/Playtests/WindowPlaytests.h"
#include "O2/Testing/Playtests/LibImGuiPlaytests.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    o2::my::LibImGuiPlaytests::drawTree();
    //o2::my::LibImGuiPlaytests::glWindowAndImGuiWindow();
    // o2::my::LibImGuiPlaytests::simpleGui();
    // o2::my::WindowPlaytests::twoWindows();
    // o2::my::WindowPlaytests::simpleWindow();
    // o2::my::LibSdlPlaytests::twoOpenGlWindows();
    // o2::my::LibSdlPlaytests::openGlWindow(); 
    // o2::my::LibSdlPlaytests::simpleWindow();
    return 0;
}