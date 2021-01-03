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
    }

    void Board::update(Window& window)
    {
        _backdrop.update(window.getCamera());
    }

    void Board::draw(DrawTarget& target, DrawStates drawStates)
    {
        target.draw(_backdrop);
        target.draw(_block);
    }
}