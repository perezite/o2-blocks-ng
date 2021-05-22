#pragma once

#include "TweenV2.h"

namespace sb 
{	
	template <class T>
	class TweenChain
	{
		T _startValue;

	public:
		TweenChain(const T& startValue)
			: _startValue(startValue), _targetValue(targetValue), _duration(duration),
			_tweenFunction(tweenFunction), _isStarted(true)
		{ }
	};

	typedef TweenChain<float> TweenChainf;
	typedef TweenChain<Vector2f> TweenChain2f;
}