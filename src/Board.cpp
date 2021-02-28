#include "Board.h"
#include "Block.h"
#include "Window.h"
#include "DrawTarget.h"

using namespace sb;

namespace blocks
{
    Board::Board(GameAssets& assets) :
        _tetromino(assets.squareTextures)
    { 
        _blocks.push_back(Block(assets.blockTextures));
        _blocks[0].setType('i');
        _blocks[0].setPosition(3, 14);
        _tetromino.setPosition(5, 16);
    }

    void Board::start() { }

    void Board::update(Window& window)
    {
        _tetromino.update();
    }

    void Board::draw(DrawTarget& target, DrawStates drawStates)
    {
        for (size_t i = 0; i < _blocks.size(); i++)
            target.draw(_blocks[i]);
        target.draw(_tetromino);
    }
}