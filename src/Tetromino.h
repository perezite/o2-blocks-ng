#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Transformable.h"

namespace blocks 
{
    class Tetromino : public sb::Drawable, public sb::Transformable
    {
        sb::Texture& _squareTextures;

        sb::Sprite _squareSprite;

        sb::Vector2i _squarePositions[4];

    public:
        Tetromino(sb::Texture& squareTextures);

        void setType(char type);

        virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault());

        void update();
    };
}