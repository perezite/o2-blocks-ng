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
    const sb::Vector2i SquareTextureSize(128);

    IntRect getSquareTextureArea(unsigned int x, unsigned int y)
    {
        return IntRect(x * SquareTextureSize.x, y * SquareTextureSize.y,
            SquareTextureSize.x, SquareTextureSize.y);
    }
}

namespace blocks
{
    Tetromino::Tetromino(Texture& squareTextures) :
        _squareTextures(squareTextures),
        _squareSprite(squareTextures)
    {
        setType('t');
    }

    void Tetromino::setType(char type)
    {
        if (type == 't') {
            _squareSprite.setTexture(_squareTextures, getSquareTextureArea(0, 0));
            Vector2i newSquarePositions[4] = { Vector2i(0, 0), Vector2i(-1, 0), Vector2i(1, 0), Vector2i(0, 1) };
            copyAll(newSquarePositions, _squarePositions);
        }
        else
            SB_ERROR("Invalid tetromino type " << type);
    }

    void Tetromino::draw(sb::DrawTarget& target, sb::DrawStates drawStates)
    {
        drawStates.transform *= getTransform();

        for (size_t i = 0; i < 4; i++)
        {
            _squareSprite.setPosition((float)_squarePositions[i].x, (float)_squarePositions[i].y);
            target.draw(_squareSprite, drawStates);
        }
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