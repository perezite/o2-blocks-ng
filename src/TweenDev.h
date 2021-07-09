#pragma once

#include "TweenFunctionsDev.h"
#include "Stopwatch.h"
#include "Vector2.h"
#include "Memory.h"
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

	namespace v2
	{
		namespace my {
			template <class T> struct Waypoint {
				T targetValue;
				float duration;
				v1::TweenFunction tweenFunction;
				Waypoint(const T& targetValue_, float duration_, v1::TweenFunction tweenFunction_) : 
					targetValue(targetValue_), duration(duration_), tweenFunction(tweenFunction_) { }
			};
		}

		template <class T> class Tween
		{
			T _startValue;
			std::vector<my::Waypoint<T>> _waypoints;
		public:
			Tween(const T& startValue)
				: _startValue(startValue)
			{ }

			Tween<T>& to(const T& targetValue, float duration, v1::TweenFunction tweenFunction) {
				my::Waypoint<T> waypoint(targetValue, duration, tweenFunction);
				_waypoints.push_back(waypoint);
				return *this;
			}

			size_t getCurrentWaypointIndex(float elapsedSeconds, float& elapsedSecondsInWaypoint) {
				SB_ERROR_IF(elapsedSeconds < 0, "Argument must not be negative");
				SB_ERROR_IF(_waypoints.empty(), "No waypoints");

				float startSeconds = 0, endSeconds;
				for (size_t i = 0; i < _waypoints.size(); i++) {
					endSeconds = startSeconds + _waypoints[i].duration;
					if (elapsedSeconds >= startSeconds && elapsedSeconds < endSeconds) {
						elapsedSecondsInWaypoint = elapsedSeconds - startSeconds;
						return i;
					}
					startSeconds = endSeconds;
				}

				elapsedSecondsInWaypoint = last(_waypoints)->duration;
				return _waypoints.size() - 1;
			}
			
			inline float getProgress(my::Waypoint<T>& waypoint, float elapsedSecondsInWaypoint)
			{
				float progress = elapsedSecondsInWaypoint / waypoint.duration;
				return clamp(progress, 0, 1);
			}

			inline T getValue(float elapsedSeconds)
			{
				float elapsedSecondsInWaypoint;
				size_t currentWaypointIndex = getCurrentWaypointIndex(elapsedSeconds, elapsedSecondsInWaypoint);
				my::Waypoint<T>& currentWaypoint = _waypoints[currentWaypointIndex];
				float progress = getProgress(currentWaypoint, elapsedSecondsInWaypoint);
				float tweenValue = currentWaypoint.tweenFunction(progress);
				T& startValue = currentWaypointIndex == 0 ? _startValue : _waypoints[currentWaypointIndex - 1].targetValue;
				return lerp(tweenValue, startValue, currentWaypoint.targetValue);
			}
		};

		typedef Tween<float> Tweenf;
		typedef Tween<Vector2f> Tween2f;
	}
}