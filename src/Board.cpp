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
        _tetromino.setPosition(5, 16);
    }

    void Board::update(Window& window)
    {
    }

    void Board::draw(DrawTarget& target, DrawStates drawStates)
    {
        target.draw(_block);
        target.draw(_tetromino);
    }
}