#pragma once

#include "Stopwatch.h"
#include "Math.h"
#include "TweenFunctions.h"

namespace sb 
{
	template <class T>
	class TweenV2
	{
		T _startValue;

		T _targetValue;

		float _duration;
		
		TweenFunction _tweenFunction;

		Stopwatch _watch;

		bool _isStarted;

	public:
		TweenV2(const T& startValue, const T& targetValue, 
			float duration = 1, TweenFunction tweenFunction = tweenFunctions::linear) 
			: _startValue(startValue), _targetValue(targetValue), _duration(duration),
			  _tweenFunction(tweenFunction), _isStarted(true)
		{ }

		inline float getProgress()
		{
			if (!_isStarted)
				return 0;

			float progress = _watch.getElapsedSeconds() / _duration;
			return clamp(progress, 0, 1);
		}

		inline T getValue()
		{
			float tweenValue = _tweenFunction(getProgress());
			return lerp(tweenValue, _startValue, _targetValue);
		}

		void reset()
		{
			_isStarted = true;
			_watch.reset();
		}
	};
}