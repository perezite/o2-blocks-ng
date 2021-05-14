namespace 
{
	Tween2f moveEffect(float dx, float dy) {
		return Tween2f().sineOut(Vector2f(dx, dy));
	}
}

class Tetromino 
{
	void input()
	{
		if (Input::isKeyGoingDown(KeyCode::Left))
			_transformEffect.move(moveEffect(-1, 0));
		if (Input::isKeyGoingDown(KeyCode::Right))
			_transformEffect.move(moveEffect(0, 1));
	}
	
public:
	void update() 
	{
		input();
	}
}

class TransformEffect
{
	Transformable _targetTransform;

public:
	float getTargetRotation();
	
	Transformable& getTargetTransform() { return _targetTransform; }
}

class BoardCollisionLogic 
{		
	Tetromino& _lastTetromino;

	float getTargetRotation(Tetromino& tetromino) 
	{
		return tetormino.getTransformEffect().getTargetRotation();
	}

	float targetDeltaRotation(Tetromino& tetromino1, Tetromino& tetromino2) 
	{ 
		return getTargetRotation(tetromino1) - getTargetRotation(tetromino2);
	}
	
	template <class TResult, typename F>
	TResult delta(Tetromino& tetromino1, Tetromino& tetromino2, F f)
	{
		return f(tetromino1) - f(tetromino2);
	}
	
	bool resolveTetrominoCollisionStep()
	{
		...
		// float deltaAngle = targetDeltaRotation(getTetromino(), _lastTetromino);
		float deltaAngle = delta(getTetromino(), _lastTetromino, getTargetRotation);
		...
	}
}