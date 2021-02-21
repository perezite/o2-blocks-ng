#include "Tetromino.h"
#include "DrawTarget.h"
#include "Rect.h"
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

    //void test(const sb::Vector2i (&source)[4], sb::Vector2i(&destination)[4]) 
    //{
    //    copy(begin(source), end(source), begin(destination));
    //}
}

namespace blocks
{
    void Tetromino::setSquarePositions(const sb::Vector2i(&squarePositions)[4])
    {
        copy(begin(squarePositions), end(squarePositions), begin(_squarePositions));
    }

    Tetromino::Tetromino(Texture& squareTextures) :
        _squareTextures(squareTextures),
        _squareSprite(squareTextures)
    {
        setType('t');
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

    void Tetromino::setType(char type)
    {
        if (type == 't') {
            _squareSprite.setTexture(_squareTextures, getSquareTextureArea(0, 0));
            setSquarePositions( { Vector2i(0, 0), Vector2i(-1, 0), Vector2i(1, 0), Vector2i(0, 1) } );
        }
        else
            SB_ERROR("Invalid tetromino type " << type);
    }
}