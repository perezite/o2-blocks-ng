#pragma once

#include "Sprite.h"
#include "Rect.h"
#include "Texture.h"
#include "GameAssets.h"

namespace blocks
{
    class Block : public sb::Sprite
    {
        sb::Texture& _blockTextures;

    public:
        Block(sb::Texture& blockTextures) :
            _blockTextures(blockTextures)
        { }

        void setType(char type);
    };
}