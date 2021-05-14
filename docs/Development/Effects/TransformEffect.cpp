namespace easings 
{
	typedef float(*Easing)(float);
	...
}

namespace 
{
	Easing moveEasing = easings::sineOut;
}

namespace blocks 
{
	class Tetromino 
	{
		void input()
		{
			if (Input::isKeyGoingDown(KeyCode::Left))
				_transformTweener.move(-1, 0, easings::sineOut, 0.2f);
			if (Input::isKeyGoingDown(KeyCode::Right))
				_transformTweener.move(1, 0, easings::sineOut, 0.2f);
		}
		
	public:
		void update() 
		{
			input();
		}
	}
}

namespace blocks
{
	class TransformTweener
	{
		Transformable _targetTransform;

	public:
		float getTargetRotation();
		
		Transformable& getTargetTransform() { return _targetTransform; }
	}
}

namespace orbis
{
	inline float signum(float value) 
	{
		if (value == 0)
			return 0;
		
		return value > 0 ? 1 : -1;
	}
}

namespace blocks 
{	
	class BoardCollisionLogic 
	{		
		Tetromino& _lastTetromino;

		float getRotation(Tetromino& tetromino) 
		{
			return tetromino.getTransformTweener().getTargetRotation();
		}
		
		void rotate(Tetromino& tetromino, float angle) 
		{
			tetromino.getTransformTweener().rotate(angle);
		}
		
		bool resolveTetrominoCollisionStep()
		{
			...
			
			float deltaAngle = getRotation(getTetromino) - getRotation(_lastTetromino);
			
			...
			
			if (canResolveCollision)
				rotate(tetromino, -signum(deltaAngleSteps) * 90 * toRadians);
		}
	}
}