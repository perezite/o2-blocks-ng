#pragma once

#include "TweenFunctionsDev.h"
#include "Stopwatch.h"
#include "Vector2.h"
#include "Math.h"

namespace sb 
{
	namespace v1
	{
		template <class T>
		class Tween
		{
			T _startValue;

			T _targetValue;

			float _duration;

			TweenFunction _tweenFunction;

			Stopwatch _watch;

			bool _isStarted;

		public:
			Tween(const T& startValue, const T& targetValue,
				float duration = 1, TweenFunction tweenFunction = tweenFunctions::linear)
				: _startValue(startValue), _targetValue(targetValue), _duration(duration),
				_tweenFunction(tweenFunction), _isStarted(true)
			{ }

			inline const T& getTargetValue() const { return _targetValue; }

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

		typedef Tween<float> Tweenf;
		typedef Tween<Vector2f> Tween2f;
	}
}