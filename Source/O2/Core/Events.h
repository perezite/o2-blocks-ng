#pragma once
#include "SdlContext.h"
#include <vector>

namespace o2
{
	class Events
	{
		static SdlContext _sdlContext;
		static std::vector<uint32_t> _windowQuitEvents;
	public:
		static void update();
		static bool isWindowCloseRequested(uint32_t windowId);
	};
}