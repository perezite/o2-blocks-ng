#pragma once 

#include "Drawable.h"
#include "Transformable.h"
#include "BlockyCollider.h"
#include "Tetromino.h"
#include "GameAssets.h"
#include "DrawBatch.h"
#include "Window.h"
#include "Block.h"
#include <vector>

namespace blocks 
{
    class Board : public sb::Drawable, public sb::Transformable
    {
        Tetromino _tetromino;
        std::vector<Block*> _blocks;
        BlockyCollider _blocksCollider;
        sb::DrawBatch _batch;

    public:
        Board(GameAssets& assets, size_t width, size_t height);

        virtual ~Board();

        void updateColliders(sb::Transform transform);

        void update(sb::Window& window);

        virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault());
    };
}