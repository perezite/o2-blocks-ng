#pragma once

#include "TweenV2.h"
#include "Memory.h"
#include <vector>

namespace sb 
{	
	template <class T>
	class TweenChain
	{
		T _startValue;

		std::vector<v2::Tween<T>> _tweens;

	protected:
		const T& getPreviousTargetValue()
		{
			if (!_tweens.empty())
				return last(_tweens)->getTargetValue();

			return _startValue;
		}

	public:
		TweenChain(const T& startValue)
			: _startValue(startValue)
		{ }

		inline TweenChain& to(const T& targetValue, float duration, TweenFunction tweenFunction)
		{
			v2::Tween<T> tween(getPreviousTargetValue(), targetValue, duration, tweenFunction);
			_tweens.push_back(tween);
			return *this;
		}
	};

	typedef TweenChain<float> TweenChainf;
	typedef TweenChain<Vector2f> TweenChain2f;
}