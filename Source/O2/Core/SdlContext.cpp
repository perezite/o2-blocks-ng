#include "SdlContext.h"
#include <SDL3/SDL.h>
#include <iostream>
using namespace std;

namespace o2
{
	int SdlContext::_instanceCount = 0;

	SdlContext::SdlContext()
	{
		if (_instanceCount == 0)
		{
			if (!SDL_Init(SDL_INIT_VIDEO))
				throw "Failed to create SDL context: %s\n", SDL_GetError();
		}

		_instanceCount++;
	}
	
	SdlContext::~SdlContext()
	{
		_instanceCount--;
		if (_instanceCount == 0)
			SDL_Quit();
	}
}
