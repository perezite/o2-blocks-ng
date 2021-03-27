#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Transformable.h"
#include "BlockyCollider.h"
#include "Ticker.h"
#include "Vector2.h"
#include "Input.h"

namespace blocks
{
    enum class TetrominoType { T, Simple };

    class Tetromino : public sb::Drawable, public sb::Transformable
    {
        TextureAtlas& _squareTextures;

        sb::Sprite _squareSprite;

        std::vector<sb::Vector2i> _squarePositions;

        BlockyCollider _collider;

        Ticker _autodropTicker;

    protected:
        void setSquares(const std::vector<sb::Vector2i>& squarePositions, size_t texPosX, size_t texPosY);

        void input();

        void autodrop();

        void harddrop();

    public:
        Tetromino(TextureAtlas& squareTextures, TetrominoType type = TetrominoType::Simple);

        void setType(TetrominoType type);

        virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault());

        void updateBlockyPhysics(sb::Transform transform);

        void update();
    };
}