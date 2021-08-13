#pragma once

#include "TweenDev.h"
#include "Stopwatch.h"

namespace sb
{
	namespace v1
	{
		template <class T>
		class Tweener {
			v2::Tween<T>& _tween;
			float _secondsElapsed;

		public:
			Tweener(v2::Tween<T>& tween) : _tween(tween), _secondsElapsed(0)
			{ }

			inline void update(float ds) { _secondsElapsed += ds; }

			inline T getValue() {
				return _tween.getValue(_secondsElapsed);
			}
		};

		typedef Tweener<float> Tweenerf;
		typedef Tweener<Vector2f> Tweener2f;
	}
}