#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Transformable.h"
#include "TextureAtlas.h"
#include "BlockType.h"
#include "Timer.h"
#include "Vector2.h"
#include "Input.h"
#include "Rect.h"

namespace blocks
{
    class Tetromino : public sb::Drawable, public sb::Transformable
    {
        BlockType _type;

        TextureAtlas& _squareTextures;

        sb::Sprite _squareSprite;

        std::vector<sb::Vector2i> _squarePositions;

        Timer _autodropTimer;

    protected:
        void setSquarePositions(const std::vector<sb::Vector2i>& positions);

        void setSquarePositions(BlockType type);

        void autodrop();

        void updateInput();

        void checkMoveInput(sb::KeyCode keyCode, int deltaX, int deltaY);

    public:
        Tetromino(TextureAtlas& squareTextures, BlockType type = BlockType::Simple);

        void getTransformedSquarePositions(std::vector<sb::Vector2i>& result);

        inline BlockType getType() { return _type; }

        void setType(BlockType type);

        virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault());

        void update();
    };
}