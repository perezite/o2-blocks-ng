#include "Events.h"
#include "StlHelper.h"
#include <SDL3/SDL.h>
#include <iostream>
using namespace std;

namespace o2 
{
	SdlContext Events::_sdlContext;
	std::vector<uint32_t> Events::_windowQuitEvents;

	void Events::update()
	{
		// clear current event list
		_windowQuitEvents.clear();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
					_windowQuitEvents.push_back(event.window.windowID);
				break;
			}
		}
	}

	bool Events::isWindowCloseRequested(uint32_t windowId)
	{
		return contains(_windowQuitEvents, windowId);
	}
}
