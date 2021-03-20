#pragma once

#include "Stopwatch.h"

namespace blocks 
{
	class Ticker
	{
		sb::Stopwatch _stopwatch;

		float _intervalSeconds;

		float _timerSeconds;

	public:
		Ticker(float intervalSeconds)
			: _intervalSeconds(intervalSeconds) 
		{ }

		bool hasTicks(); 
	};
}