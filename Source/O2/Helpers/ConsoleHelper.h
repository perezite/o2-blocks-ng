#include <iostream>
#ifdef WIN32
    #include <windows.h>
#endif
using namespace std;

namespace o2
{
    #ifdef WIN32

    void bringConsoleToForeground()
    {
        HWND console = GetConsoleWindow();
        if (!console) return;
        ShowWindow(console, SW_RESTORE);
        SetWindowPos( console, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        SetWindowPos(console,HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }

    #else

    void focusConsole()
    {
        clog << "o2::bringConsoleToForeground() is not implemented for this Platform" << endl;
    }

    #endif
}