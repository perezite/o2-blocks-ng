#pragma once

#include "Sprite.h"
#include "Rect.h"

namespace blocks
{
    class Block : public sb::Sprite
    {
    public:
        Block();

        void setType(char type);
    };
}