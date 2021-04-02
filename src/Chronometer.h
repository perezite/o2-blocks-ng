#pragma once

#include "Stopwatch.h"

namespace blocks 
{
	class Chronometer
	{
		sb::Stopwatch _stopwatch;

		float _intervalSeconds;

		float _timerSeconds;

	public:
		Chronometer(float intervalSeconds)
			: _intervalSeconds(intervalSeconds), _timerSeconds(0)
		{ }

		bool hasTicks(); 
	};
}