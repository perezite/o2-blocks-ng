#include "Block.h"
#include "Rect.h"
#include "Logger.h"
#include "GameAssets.h"

using namespace sb;

namespace
{
    const sb::Vector2i BlocksTextureSheetSize(128);

    IntRect getBlockTextureArea(unsigned int x, unsigned int y)
    {
        return IntRect(x * BlocksTextureSheetSize.x, y * BlocksTextureSheetSize.y, 
            BlocksTextureSheetSize.x, BlocksTextureSheetSize.y);
    }
}

namespace blocks
{
    void Block::setType(char type)
    {
        if (type == 'i')
            setTexture(_assets.blockTextures, getBlockTextureArea(1, 2));
        else
            SB_ERROR("Invalid tetromino type " << type);
    }
}
