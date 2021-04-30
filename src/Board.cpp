#include "Board.h"
#include "Block.h"
#include "Window.h"
#include "Memory.h"
#include "Math.h"
#include "Rect.h"
#include "DrawTarget.h"
#include "VectorHelper.h"

using namespace sb;
using namespace std;

namespace blocks
{
    void Board::respawnTetromino()
    {
        safeDelete(_tetromino);

        _tetromino = new Tetromino(_assets.squareTextureAtlas, TetrominoType::T);
        _tetromino->setPosition(5, 16);
        //_tetromino->setPosition(0, 0);

    }

    void Board::getBlockPositions(std::vector<sb::Vector2i>& result)
    {
        result.clear(); result.reserve(_blocks.size());

        for (size_t i = 0; i < _blocks.size(); i++)
        {
            Vector2i blockPosition = toVector2i(_blocks[i]->getPosition());
            result.push_back(blockPosition);
        }
    }

    bool Board::tetrominoIsOutsideBounds()
    {
        vector<Vector2i> positions; _tetromino->getTransformedSquarePositions(positions);
        IntRect boardRect(0, 0, _size.x, _size.y);

        for (size_t i = 0; i < positions.size(); i++)
        {
            if (!boardRect.contains(positions[i]))
                return true;
        }

        return false;
    }

    bool Board::tetrominoCollidesWithBlock()
    {
        vector<Vector2i> blockPositions; getBlockPositions(blockPositions);
        vector<Vector2i> tetrominoPositions; _tetromino->getTransformedSquarePositions(tetrominoPositions);

        for (size_t i = 0; i < blockPositions.size(); i++)
        {
            for (size_t j = 0; j < tetrominoPositions.size(); j++)
            {
                if (blockPositions[i] == tetrominoPositions[j])
                    return true;
            }
        }

        return false;

    }

    bool Board::hasTetrominoCollision()
    {
        bool hasCollision = tetrominoCollidesWithBlock();
        hasCollision |= tetrominoIsOutsideBounds();

        return hasCollision;
    }

    void Board::resetTetrominoCollisions()
    {
        _lastTetromino = (Transformable)*_tetromino;
        _isTetrominoDead = false;
        _isTetrominoStuck = false;
    }

    bool Board::resolveTetrominoCollisionStep()
    {
        float deltaAngle = (_tetromino->getRotation() - _lastTetromino.getRotation());
        int deltaAngleSteps = (int)round(deltaAngle * ToDegrees / 90);
        Vector2i deltaPosition = toVector2i(_tetromino->getPosition() - _lastTetromino.getPosition());
        bool canResolveCollision = deltaAngleSteps != 0 || deltaPosition != Vector2i(0);

        if (canResolveCollision)
        {
            if (deltaAngleSteps != 0)
                _tetromino->rotate(deltaAngleSteps > 0 ? -90 * ToRadians : 90 * ToRadians);
            else if (deltaPosition.y < 0)
            {
                _tetromino->translate(0, 1);
                _isTetrominoDead = true;        // downwards collisions kill the tetromino
            }
            else if (deltaPosition.y > 0)
                _tetromino->translate(0, -1);
            else if (deltaPosition.x != 0)
                _tetromino->translate(deltaPosition.x > 0 ? Vector2f(-1, 0) : Vector2f(1, 0));
        }

        return canResolveCollision;
    }

    void Board::resolveTetrominoCollisions()
    {
        while (hasTetrominoCollision())
        {
            bool couldResolveCollision = resolveTetrominoCollisionStep();
            if (!couldResolveCollision)
            {
                _isTetrominoStuck = true;
                break;
            }
        }
    }

    void Board::updateTetrominoCollisions()
    {
        if (_mustResetTetrominoCollisions)
        {
            resetTetrominoCollisions();
            _mustResetTetrominoCollisions = false;
        }
    
        resolveTetrominoCollisions();

        _lastTetromino = (Transformable)*_tetromino;

        if (_isTetrominoDead)
        {
            cout << "respawn" << endl;
            _mustResetTetrominoCollisions = true;
            respawnTetromino();
        }

        if (_isTetrominoStuck)
        {
            _mustResetTetrominoCollisions = true;
            while (true)
            {
                cout << "Game over!!" << endl;
                cin.get();
            }
        }
    }

    Board::Board(GameAssets& assets, size_t width, size_t height) : 
        _assets(assets), _size(width, height), _tetromino(NULL), 
        _mustResetTetrominoCollisions(true), _isTetrominoDead(false),
        _isTetrominoStuck(false)
    { 
        _blocks.push_back(new Block(assets.blockTextureAtlas));
        _blocks[0]->setPosition(3, 14);
        respawnTetromino();

        //_blocks[0]->setPosition(3, 3);
        //_tetromino.setPosition(1, 1);
    }

    Board::~Board() 
    {
        deleteAll(_blocks); 
        delete _tetromino;
    }

    void Board::update(Window& window)
    {
        _tetromino->update();
        updateTetrominoCollisions();
    }

    void Board::draw(DrawTarget& target, DrawStates states)
    {
        states.transform *= getTransform();
        target.draw(_tetromino, states);

        for (size_t i = 0; i < _blocks.size(); i++)
            _batch.draw(*_blocks[i], states);

        target.draw(_batch);
    }
}