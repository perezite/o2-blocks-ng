#include "Block.h"
#include "Rect.h"
#include "Logger.h"
#include "GameAssets.h"

using namespace sb;

namespace blocks
{
    Block::Block(TextureAtlas& blockTextures, BlockType type)
        : _blockTextures(blockTextures)
    {
        setType(type);
    }

    void Block::setType(BlockType type)
    {
        if (type == BlockType::I)
            setTexture(_blockTextures.getTextureSheet(), _blockTextures.getTextureArea(1, 2));
        else
            SB_ERROR("Invalid tetromino type " << (int)type);
    }
}
