#pragma once
#include "SdlContext.h"
#include "StlHelper.h"
#include <SDL3/SDL.h>
#include <vector>

namespace o2
{
	class Events
	{
		static SdlContext _sdlContext;
		static std::vector<SDL_Event> _sdlEvents;
		static std::vector<uint32_t> _windowQuitEvents;
	public:
		static void update();
		inline static const std::vector<SDL_Event>& getSdlEvents() { return _sdlEvents; }
		inline static bool isWindowCloseRequested(uint32_t windowId) 
		{
			return contains(_windowQuitEvents, windowId); 
		}
	};
}