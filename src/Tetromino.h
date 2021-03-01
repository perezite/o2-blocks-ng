#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Transformable.h"
#include "BlockyCollider.h"

namespace blocks 
{
    class Tetromino : public sb::Drawable, public sb::Transformable
    {
        sb::Texture& _squareTextures;

        sb::Sprite _squareSprite;

        std::vector<sb::Vector2i> _squarePositions;

        BlockyCollider _collider;

    protected:
        void setSquares(const std::vector<sb::Vector2i>& squarePositions, size_t texPosX, size_t texPosY);

        void tryMove(int x, int y);

    public:
        Tetromino(sb::Texture& squareTextures);

        void setType(char type);

        virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault());

        void updateColliders(sb::Transform transform);

        void update();
    };
}