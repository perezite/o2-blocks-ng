#include "WindowPlaytests.h"
#include "../../Window/Window.h"
#include"../../Core/Events.h"
#include <SDL3/SDL.h>
#include <iostream>
using namespace std;

namespace o2 
{
	namespace my 
	{
		void WindowPlaytests::twoWindows()
		{
			cout << "Expected behaviour: " << endl;
			cout << "- Two windows with a red/blue background and different sizes are displayed " << endl;
			cout << "Technical details: " << endl;
			cout << "- The windows use SDL and OpenGL for rendering" << endl;
			cout << "- The windows use a separate Events class for Event handling" << endl;

			Window firstWindow(800, 600, "First window");
			Window secondWindow(400, 300, "First window");

			while (true)
			{
				Events::update();
				firstWindow.update();
				secondWindow.update();
				if (!firstWindow.isOpen() && !secondWindow.isOpen())
					break;
				firstWindow.clear(255, 0, 0);
				secondWindow.clear(0, 0, 255);
				firstWindow.display();
				secondWindow.display();
			}
		}

		void WindowPlaytests::simpleWindow()
		{
            cout << "Expected behaviour: " << endl;
			cout << "- A simple window with a light-blue background is displayed " << endl;
			cout << "Technical details: " << endl;
			cout << "- The window uses SDL and OpenGL for rendering" << endl;
			cout << "- The window uses a separate Events class for Event handling" << endl;

			Window window(800, 600, "Simple Window");

			while (true) {
				Events::update();
				window.update();
				if (!window.isOpen())
					break;
				window.clear(51, 204, 255);
				window.display();
			}
		}
	}
}