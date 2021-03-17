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
    Block::Block(sb::Texture& blockTextures, BlockType type)
        : _blockTextures(blockTextures)
    {
        setType(type);
    }

    void Block::setType(BlockType type)
    {
        if (type == BlockType::I)
            setTexture(_blockTextures, getBlockTextureArea(1, 2));
        else
            SB_ERROR("Invalid tetromino type " << (int)type);
    }
}
