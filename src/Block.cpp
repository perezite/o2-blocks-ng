#include "Block.h"
#include "Rect.h"
#include "Logger.h"
#include "GameAssets.h"

using namespace sb;

namespace
{
    const sb::Vector2i SquareTextureSize(128);

    IntRect getBlockTextureArea(unsigned int x, unsigned int y)
    {
        return IntRect(x * SquareTextureSize.x, y * SquareTextureSize.y, 
            SquareTextureSize.x, SquareTextureSize.y);
    }
}

namespace blocks
{
    void Block::setType(char type)
    {
        if (type == 'i')
            setTexture(_blockTextures, getBlockTextureArea(1, 2));
        else
            SB_ERROR("Invalid tetromino type " << type);
    }
}
