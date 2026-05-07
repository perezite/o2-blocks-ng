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
		void WindowPlaytests::simpleWindow()
		{
            cout << "Expected behaviour: " << endl;
			cout << "- A simple window with a light-blue background is displayed " << endl;
			cout << "Technical details: " << endl;
			cout << "- The window uses SDL and OpenGL for rendering" << endl;
			cout << "- The window uses a separate Events class for Event handling" << endl;

			Window window(800, 600, "Simple Window");

			while (true)
			{
				Events::update();
				window.update();
				if (!window.isOpen())
					break;
				window.clear();
				window.display();
			}
		}
	}
}