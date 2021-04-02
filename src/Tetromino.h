#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Drawable.h"
#include "Transformable.h"
#include "BlockyCollider.h"
#include "Chronometer.h"
#include "Vector2.h"
#include "Input.h"
#include "Rect.h"

namespace blocks
{
    enum class TetrominoType { T, Simple };

    class Tetromino : public sb::Drawable, public sb::Transformable
    {
        TextureAtlas& _squareTextures;

        sb::Sprite _squareSprite;

        std::vector<sb::Vector2i> _squarePositions;

        BlockyCollider _collider;

        Chronometer _autodropChronometer;

        sb::IntRect _movementBounds;

        bool _isDead;

    protected:
        inline static bool isBelowGround(const sb::Vector2i& position) { return position.y < 0; }

        void setSquares(const std::vector<sb::Vector2i>& squarePositions, size_t texPosX, size_t texPosY);

        bool canTransform(const sb::Vector2i& deltaPosition, float deltaRadians = 0);

        bool tryMove(const sb::Vector2i& delta);

        void tryRotate(float deltaRadians);

        bool checkMoveInput(sb::KeyCode keyCode, int deltaX, int deltaY);

        void drop();

        void harddrop();

        void updateInput();

        void autodrop();

    public:
        Tetromino(TextureAtlas& squareTextures, const sb::IntRect& movementBounds, TetrominoType type = TetrominoType::Simple);

        bool isDead() const { return _isDead; }

        void setType(TetrominoType type);

        virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault());

        void updateColliders(sb::Transform transform);

        void update();
    };
}