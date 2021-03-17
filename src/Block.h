#pragma once

#include "Sprite.h"
#include "Rect.h"
#include "Texture.h"
#include "GameAssets.h"

namespace blocks
{
    enum class BlockType { I };

    class Block : public sb::Sprite
    {
        sb::Texture& _blockTextures;

    public:
        Block(sb::Texture& blockTextures, BlockType type = BlockType::I);

        void setType(BlockType type);
    };

}