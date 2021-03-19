#pragma once

#include "Sprite.h"
#include "Rect.h"
#include "Texture.h"
#include "GameAssets.h"
#include "TextureAtlas.h"

namespace blocks
{
    enum class BlockType { I };

    class Block : public sb::Sprite
    {
        TextureAtlas& _blockTextures;

    public:
        Block(TextureAtlas& blockTextures, BlockType type = BlockType::I);

        void setType(BlockType type);
    };

}