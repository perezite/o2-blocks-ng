#pragma once

#include "Math.h"

namespace sb 
{
	namespace tweenFunctions
	{
		inline float linear(float x)
		{
			return x;
		}

		inline float smoothstep(float x)
		{
			return x * x * (3 - 2 * x);
		}
	}
}