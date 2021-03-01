#include "Tetromino.h"
#include "DrawTarget.h"
#include "Rect.h"
#include "Memory.h"
#include "Input.h"
#include <algorithm>
#include <iterator>

using namespace sb;
using namespace std;

namespace
{
    const sb::Vector2ui SquareTextureSize(128);

    const std::vector<sb::Vector2i> TShapeSquarePositions = { Vector2i(0, 0), Vector2i(-1, 0), Vector2i(1, 0), Vector2i(0, 1) };

    IntRect getSquareTextureArea(size_t x, size_t y) {
        return IntRect(x * SquareTextureSize.x, y * SquareTextureSize.y,
            SquareTextureSize.x, SquareTextureSize.y);
    }
}

namespace blocks
{
    void Tetromino::setSquares(const std::vector<sb::Vector2i>& squarePositions, size_t texPosX, size_t texPosY)
    {
        _squareSprite.setTexture(_squareTextures, getSquareTextureArea(texPosX, texPosY));
        _squarePositions.clear();
        _squarePositions.assign(TShapeSquarePositions.begin(), TShapeSquarePositions.end());
    }

    Tetromino::Tetromino(Texture& squareTextures) :
        _squareTextures(squareTextures),
        _squareSprite(squareTextures)
    {
        setType('t');
    }

    void Tetromino::setType(char type)
    {
        if (type == 't')
            setSquares(TShapeSquarePositions, 0, 0);
        else
            SB_ERROR("Invalid tetromino type " << type);
    }

    void Tetromino::draw(sb::DrawTarget& target, sb::DrawStates drawStates)
    {
        drawStates.transform *= getTransform();

        for (size_t i = 0; i < 4; i++) {
            _squareSprite.setPosition((float)_squarePositions[i].x, (float)_squarePositions[i].y);
            target.draw(_squareSprite, drawStates);
        }
    }

    void Tetromino::updateColliders(sb::Transform transform)
    {
        transform *= getTransform();
        _collider.update(transform, _squarePositions);
    }

    void Tetromino::update()
    {
        if (Input::isKeyGoingDown(KeyCode::Left))
            translate(-1,  0);
        if (Input::isKeyGoingDown(KeyCode::Right))
            translate(+1,  0);
        if (Input::isKeyGoingDown(KeyCode::Down))
            translate( 0, -1);
    }
}