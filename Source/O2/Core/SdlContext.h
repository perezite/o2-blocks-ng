#pragma once
#include <SDL3/SDL.h>
#include <stdexcept>

namespace o2
{
	class SdlContext
	{
		static int _instanceCount;
	public:
		SdlContext();
		~SdlContext();
	};

	inline static void sdlCheck(const bool okCondition) {
		if (!okCondition) throw std::runtime_error(SDL_GetError());
	}
}