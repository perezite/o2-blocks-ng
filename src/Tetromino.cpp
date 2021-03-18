#include "Tetromino.h"
#include "DrawTarget.h"
#include "Rect.h"
#include "Memory.h"
#include "Input.h"
#include "Math.h"
#include <algorithm>
#include <iterator>

using namespace sb;
using namespace std;

namespace
{
    const Vector2ui SquareTextureSize(128);

    const vector<Vector2i> TShapeSquarePositions = { Vector2i(0, 0), Vector2i(-1, 0), Vector2i(1, 0), Vector2i(0, 1) };

    const vector<Vector2i> SimpleShapeSquarePositions = { Vector2i(0, 0), Vector2i(1, 0) };

    IntRect getSquareTextureArea(size_t x, size_t y) {
        return IntRect(x * SquareTextureSize.x, y * SquareTextureSize.y,
            SquareTextureSize.x, SquareTextureSize.y);
    }
}

namespace blocks
{
    void Tetromino::setSquares(const vector<Vector2i>& squarePositions, size_t texPosX, size_t texPosY)
    {
        _squareSprite.setTexture(_squareTextures, getSquareTextureArea(texPosX, texPosY));
        _squarePositions.clear();
        _squarePositions.assign(squarePositions.begin(), squarePositions.end());
    }

    void Tetromino::tryMove(int x, int y)
    {
        bool wouldCollide = _collider.wouldCollide(Vector2i(x, y), 0);
        if (!wouldCollide)
            translate(float(x), float(y));
    }

    void Tetromino::tryRotate(float radians) 
    {
        bool wouldCollide = _collider.wouldCollide(Vector2i(0), radians);
        if (!wouldCollide)
            rotate(radians);
    }

    Tetromino::Tetromino(Texture& squareTextures, TetrominoType type) 
        : _squareTextures(squareTextures), _squareSprite(squareTextures), _collider(*this)
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
        static const Transform originTransform(Vector2f(-.5f));
        static const Transform inverseOriginTransform(Vector2f(.5f));

        // we transform the origin, such that rotations and scaling are applied to the center but 
        // the drawable is still positioned relative to the original lower left corner
        drawStates.transform *= inverseOriginTransform * getTransform() * originTransform;

        for (size_t i = 0; i < _squarePositions.size(); i++) {
            _squareSprite.setPosition((float)_squarePositions[i].x, (float)_squarePositions[i].y);
            target.draw(_squareSprite, drawStates);
        }
    }

    void Tetromino::updateColliders(Transform transform)
    {
        Transform parentTransform = transform;
        transform *= getTransform();
        _collider.update(parentTransform, transform, _squarePositions);
    }

    void Tetromino::update()
    {
        if (Input::isKeyGoingDown(KeyCode::Left))
            tryMove(-1,  0);
        if (Input::isKeyGoingDown(KeyCode::Right))
            tryMove(+1,  0);
        if (Input::isKeyGoingDown(KeyCode::Up))
            tryMove( 0, +1);
        if (Input::isKeyGoingDown(KeyCode::Down))
            tryMove( 0, -1);
        if (Input::isKeyGoingDown(KeyCode::r))
            tryRotate(90 * ToRadians);
    }
}