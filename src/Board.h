#pragma once 

#include "Drawable.h"
#include "Transformable.h"
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
        GameAssets _assets;
        sb::Vector2i _size;
        Tetromino* _tetromino;
        std::vector<Block*> _blocks;
        sb::DrawBatch _batch;

    protected:
        void respawnTetromino();

    public:
        Board(GameAssets& assets, size_t width, size_t height);

        virtual ~Board();

        void update(sb::Window& window);

        virtual void draw(sb::DrawTarget& target, sb::DrawStates drawStates = sb::DrawStates::getDefault());
    };
}