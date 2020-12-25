#include "Block.h"
#include "Rect.h"
#include "Logger.h"

using namespace sb;

namespace
{
    const sb::Vector2i BlocksTexturesSize(128);

    IntRect getBlockTextureArea(unsigned int x, unsigned int y)
    {
        return IntRect(x * BlocksTexturesSize.x, y * BlocksTexturesSize.y, 
            BlocksTexturesSize.x, BlocksTexturesSize.y);
    }
}

namespace blocks
{
    sb::Texture& Block::getBlockTextureSheet()
    {
        static sb::Texture blockTextureSheet("Textures/Blocks.png");
        return blockTextureSheet;
    }

    void Block::setType(char type)
    {
        if (type == 'i')
            setTexture(getBlockTextureSheet(), getBlockTextureArea(1, 2));
        else
            SB_ERROR("Invalid tetromino type " << type);
    }
}
