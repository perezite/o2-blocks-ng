#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Transformable.h"
#include "TextureAtlas.h"
#include "Chronometer.h"
#include "Vector2.h"
#include "Input.h"
#include "Rect.h"

namespace blocks
{
    enum class TetrominoType { T, Simple, VerySimple };

    class Tetromino : public sb::Drawable, public sb::Transformable
    {
        TextureAtlas& _squareTextures;

        sb::Sprite _squareSprite;

        std::vector<sb::Vector2i> _squarePositions;

        Chronometer _autodropChronometer;

    protected:
        void setSquares(const std::vector<sb::Vector2i>& squarePositions, size_t texPosX, size_t texPosY);

        void harddrop();

        void autodrop();

        void updateInput();

        void checkMoveInput(sb::KeyCode keyCode, int deltaX, int deltaY);

    public:
        Tetromino(TextureAtlas& squareTextures, TetrominoType type = TetrominoType::Simple);

        void getTransformedSquarePositions(std::vector<sb::Vector2i>& result);

        void setType(TetrominoType type);

        virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault());

        void update();
    };
}