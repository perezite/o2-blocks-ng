#pragma once

#include "Transformable.h"
#include "Tetromino.h"

namespace blocks 
{
    class Board;

	class BoardCollisionLogic
	{
        Board& _parent;
        sb::Transformable _lastTetromino;
        bool _mustResetTetrominoCollisionData;
        bool _isTetrominoDead;
        bool _isTetrominoStuck;

    protected:
        Tetromino& getTetromino();

        bool tetrominoIsOutsideBounds();

        bool tetrominoCollidesWithBlock();

        bool hasTetrominoCollision();

        bool resolveTetrominoCollisionStep();

        void resolveTetrominoCollisions();

        void updateTetrominoCollisions();

	public:
        BoardCollisionLogic(Board& parent) : 
            _parent(parent) { }

        bool isTetrominoDead() const { return _isTetrominoDead; }

        bool isTetrominoStuck() const { return _isTetrominoStuck; }

        void resetTetrominoCollisionData();

		void update();
	};
}