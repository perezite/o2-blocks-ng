#include "Board.h"
#include "Block.h"
#include "Window.h"
#include "DrawTarget.h"

using namespace sb;

namespace blocks
{
    void Board::start()
    {
        _block.setType('i');
        _block.setPosition(3, 14);
        _tetromino.setPosition(5, 16);
    }

    void Board::update(Window& window)
    {
        _tetromino.update();
    }

    void Board::draw(DrawTarget& target, DrawStates drawStates)
    {
        target.draw(_block);
        target.draw(_tetromino);
    }
}