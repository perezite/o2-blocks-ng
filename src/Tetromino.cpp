#include "Tetromino.h"
#include "DrawTarget.h"
#include "Rect.h"
#include "Memory.h"
#include "Input.h"
#include "Math.h"
#include "Configuration.h"
#include <algorithm>

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

    bool Tetromino::tryMove(const Vector2i& delta)
    {
        bool wouldCollide = _collider.wouldCollide(delta, 0);
        if (!wouldCollide)
            translate(toVector2f(delta));

        return wouldCollide;
    }

    void Tetromino::tryRotate(float deltaRadians)
    {
        if (!_collider.wouldCollide(0, deltaRadians))
            rotate(deltaRadians);
    }

    void Tetromino::checkMove(sb::KeyCode keyCode, int deltaX, int deltaY)
    {
        if (Input::isKeyGoingDown(keyCode))
            tryMove(Vector2i(deltaX, deltaY));
    }

    void Tetromino::input()
    {
        checkMove(KeyCode::Left, -1, 0);
        checkMove(KeyCode::Right, +1, 0);
        checkMove(KeyCode::Up, 0, +1);
        checkMove(KeyCode::Down, 0, -1);

        if (Input::isKeyGoingDown(KeyCode::r))
            tryRotate(-90 * ToRadians);

        if (Input::isKeyGoingDown(KeyCode::Space))
            harddrop();

        #if _DEBUG
            if(Input::isKeyGoingDown(KeyCode::d))
                autodropEnabled = !autodropEnabled;
        #endif
    }

    void Tetromino::autodrop()
    {
        while (_autodropTicker.hasTicks())
            tryMove(Vector2i(0, -1));
    }

    void Tetromino::harddrop()
    {
        bool collided = false;
        bool isBelowGround = false;
        int deltaY = 0;

        while (!collided && !isBelowGround) {
            Vector2i delta(0, --deltaY);
            collided = _collider.wouldCollide(delta, 0);
            isBelowGround = any(_collider.getGlobalPositions(delta), Tetromino::isBelowGround);
        }

        translate(toVector2f(0, deltaY + 1));
    }

    Tetromino::Tetromino(TextureAtlas& squareTextures, TetrominoType type)
        : _squareTextures(squareTextures), _collider(*this), _autodropTicker(configuration::autodropSeconds)
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
        // we transform the origin, such that rotations and scaling are applied relative to the center
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
        input();
        if (autodropEnabled)
            autodrop();
    }
}