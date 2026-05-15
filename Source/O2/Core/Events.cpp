#include "Events.h"
#include <iostream>
using namespace std;

namespace o2 
{
	SdlContext Events::_sdlContext;
	std::vector<SDL_Event> Events:: _sdlEvents;
	std::vector<uint32_t> Events::_windowQuitEvents;


	void Events::update()
	{
		_windowQuitEvents.clear();
		_sdlEvents.clear();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			_sdlEvents.push_back(event);

			switch (event.type)
			{
				case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
					_windowQuitEvents.push_back(event.window.windowID);
				break;
			}
		}
	}
}
