#pragma once 

#include "Drawable.h"
#include "Transformable.h"
#include "Backdrop.h"
#include "Tetromino.h"
#include "GameAssets.h"
#include "Block.h"

namespace blocks 
{
    class Board : public sb::Drawable, public sb::Transformable
    {
        Block _block;
        // Tetromino _tetromino;

    public:
        Board(GameAssets& assets) 
            : _block(assets)
        { }

        void start();

        void update(sb::Window& window);

        virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault());
    };
}