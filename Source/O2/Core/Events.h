#pragma once
#include "SdlContext.h"
#include "StlHelper.h"
#include <vector>

namespace o2
{
	class Events
	{
		static SdlContext _sdlContext;
		static std::vector<uint32_t> _windowQuitEvents;
	public:
		static void update();
		inline static bool isWindowCloseRequested(uint32_t windowId) 
		{
			return contains(_windowQuitEvents, windowId); 
		}
	};
}