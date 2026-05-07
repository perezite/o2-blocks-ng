#include "O2/Testing/Playtests/LibSdlPlaytests.h"
#include "O2/Testing/Playtests/WindowPlaytests.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    o2::my::WindowPlaytests::simpleWindow();
    // o2::my::LibSdlPlaytests::openGlWindow(); 
    // o2::my::LibSdlPlaytests::simpleWindow();
    return 0;
}