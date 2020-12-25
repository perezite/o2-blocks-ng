#pragma once

#include "Sprite.h"
#include "Rect.h"

namespace blocks
{
    class Block : public sb::Sprite
    {

    protected:
        sb::Texture& getBlockTextureSheet();

    public:
        void setType(char type);
    };
}