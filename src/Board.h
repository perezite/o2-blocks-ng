#pragma once 

#include "Drawable.h"
#include "Transformable.h"
#include "BlockyCollider.h"
#include "Tetromino.h"
#include "GameAssets.h"
#include "Window.h"
#include "Block.h"
#include <vector>

namespace blocks 
{
    class Board : public sb::Drawable, public sb::Transformable
    {
        Tetromino _tetromino;
        std::vector<Block> _blocks;
        BlockyCollider _collider;

    public:
        Board(GameAssets& assets);

        void start();

        void updateColliders(sb::Transform transform);

        void update(sb::Window& window);

        virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault());
    };
}