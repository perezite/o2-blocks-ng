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
			Stopwatch _watch;

		public:
			Tweener(v2::Tween<T>& tween) : _tween(tween)
			{ }

			inline T getValue() {
				return _tween.getValue(_watch.getElapsedSeconds());
			}
		};

		typedef Tweener<float> Tweenerf;
		typedef Tweener<Vector2f> Tweener2f;
	}
}