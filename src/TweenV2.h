#pragma once

#include "Stopwatch.h"
#include "Math.h"
#include "TweenFunctions.h"

namespace sb 
{
	typedef float(*TweenFunction)(float);

	template <class T>
	class TweenV2
	{
		T _startValue;

		T _targetValue;

		TweenFunction _tweenFunction;

		float _duration;

		Stopwatch _watch;

		bool _isStarted;

	public:
		TweenV2(const T& startValue, const T& targetValue, TweenFunction tweenFunction = tweenFunctions::linear, float duration = 1) 
			: _startValue(startValue), _targetValue(targetValue), _tweenFunction(tweenFunction), _duration(1), _isStarted(false)
		{
		}

		inline float getProgress()
		{
			if (!_isStarted)
				return 0;

			float progress = _watch.getElapsedSeconds() / _duration;
			return clamp(progress, 0, 1);
		}

		inline T getValue()
		{
			return lerp(getProgress(), _startValue, _targetValue);
		}

		void start()
		{
			_isStarted = true;
			_watch.reset();
		}
	};
}