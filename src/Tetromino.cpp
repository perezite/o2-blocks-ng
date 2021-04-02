#include "Tetromino.h"
#include "DrawTarget.h"
#include "Rect.h"
#include "Memory.h"
#include "Input.h"
#include "Math.h"
#include "Configuration.h"

using namespace std;
using namespace sb;

namespace
{
    bool autodropEnabled = false;
    const vector<Vector2i> TShapeSquarePositions = { Vector2i(0, 0), Vector2i(-1, 0), Vector2i(1, 0), Vector2i(0, 1) };
    const vector<Vector2i> SimpleShapeSquarePositions = { Vector2i(0, 0), Vector2i(1, 0) };
}

namespace blocks
{
    void Tetromino::setSquares(const vector<Vector2i>& squarePositions, size_t texPosX, size_t texPosY)
    {
        _squareSprite.setTexture(_squareTextures.getTextureSheet(), _squareTextures.getTextureArea(texPosX, texPosY));
        _squarePositions.clear();
        _squarePositions.assign(squarePositions.begin(), squarePositions.end());
    }

    bool Tetromino::canTransform(const sb::Vector2i& deltaPosition, float deltaRadians)
    {
        bool wouldCollide = _collider.wouldCollide(deltaPosition, deltaRadians);
        auto globalBounds = _collider.getGlobalBounds(deltaPosition, deltaRadians);
        bool wouldLeaveBounds = !_movementBounds.contains(globalBounds);

        return !wouldCollide && !wouldLeaveBounds;
    }

    bool Tetromino::tryMove(const Vector2i& delta)
    {
        bool canMove = canTransform(delta);
        if (canMove)
            translate(toVector2f(delta));

        return canMove;
    }

    void Tetromino::tryRotate(float deltaRadians)
    {
        if (canTransform(0, deltaRadians))
            rotate(deltaRadians);
    }

    bool Tetromino::checkMoveInput(sb::KeyCode keyCode, int deltaX, int deltaY)
    {
        if (Input::isKeyGoingDown(keyCode))
            return tryMove(Vector2i(deltaX, deltaY));

        return true;
    }

    void Tetromino::drop() {
        if (!tryMove(Vector2i(0, -1)))
            _isDead = true;
    }

    void Tetromino::harddrop()
    {
        bool canMove;
        int deltaY = 0;

        do {
            Vector2i deltaPosition(0, --deltaY);
            canMove = canTransform(deltaPosition);
        } while (canMove);

        translate(toVector2f(0, deltaY + 1));
        _isDead = true;
    }

    void Tetromino::updateInput()
    {
        checkMoveInput(KeyCode::Left, -1, 0);
        checkMoveInput(KeyCode::Right, +1, 0);
        checkMoveInput(KeyCode::Up, 0, +1);

        if (Input::isKeyGoingDown(KeyCode::Down))
            drop();

        if (Input::isKeyGoingDown(KeyCode::Space))
            harddrop();

        if (Input::isKeyGoingDown(KeyCode::r))
            tryRotate(-90 * ToRadians);

        #if _DEBUG
            if(Input::isKeyGoingDown(KeyCode::d))
                autodropEnabled = !autodropEnabled;
        #endif
    }

    void Tetromino::autodrop()
    {
        while (_autodropChronometer.hasTicks()) {
            drop();
        }
    }

    Tetromino::Tetromino(TextureAtlas& squareTextures, const sb::IntRect& movementBounds, TetrominoType type)
        : _squareTextures(squareTextures), _collider(*this), _autodropChronometer(configuration::autodropSeconds),
          _movementBounds(movementBounds), _isDead(false)
    {
        setType(type);
    }

    void Tetromino::setType(TetrominoType type)
    {
        if (type == TetrominoType::T)
            setSquares(TShapeSquarePositions, 0, 0);
        else if (type == TetrominoType::Simple)
            setSquares(SimpleShapeSquarePositions, 0, 0);
        else
            SB_ERROR("Invalid tetromino type " << (int)type);
    }

    void Tetromino::draw(DrawTarget& target, DrawStates drawStates)
    {
        // we transform the origin, such that rotations and scaling are applied relative to the center of the tetromino
        static const Transform originTransform(-.5f);
        static const Transform inverseOriginTransform(.5f);
        drawStates.transform *= inverseOriginTransform * getTransform() * originTransform;

        for (size_t i = 0; i < _squarePositions.size(); i++) {
            _squareSprite.setPosition(toVector2f(_squarePositions[i]));
            target.draw(_squareSprite, drawStates);
        }
    }

    void Tetromino::updateColliders(Transform parentTransform)
    {
        _collider.update(parentTransform, _squarePositions);
    }

    void Tetromino::update()
    {
        updateInput();
        
        if (autodropEnabled)
            autodrop();
    }
}