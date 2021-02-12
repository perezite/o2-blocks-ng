#pragma once

#include "Sprite.h"
#include "Rect.h"
#include "GameAssets.h"

namespace blocks
{
    class Block : public sb::Sprite
    {
        GameAssets& _assets;

    public:
        Block(GameAssets& assets) :
            _assets(assets)
        { }

        void setType(char type);
    };
}