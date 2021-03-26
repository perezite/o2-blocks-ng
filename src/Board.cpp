#include "Board.h"
#include "Block.h"
#include "Window.h"
#include "Memory.h"
#include "DrawTarget.h"

using namespace sb;

namespace blocks
{
    //Board::Board(GameAssets& assets) :
    //    _tetromino(assets.squareTextureAtlas, TetrominoType::T), 
    //    _collider(*this)
    //{ 
    //    _blocks.push_back(new Block(assets.blockTextureAtlas));
    //    _blocks[0]->setPosition(3, 14);
    //    _tetromino.setPosition(5, 16);
    //}

    Board::Board(GameAssets& assets) :
        _tetromino(assets.squareTextureAtlas, TetrominoType::Simple), 
        _collider(*this)
    { 
        _blocks.push_back(new Block(assets.blockTextureAtlas));
        _blocks[0]->setPosition(1, 1);
        _tetromino.setPosition(0, 0);
    }

    Board::~Board() { deleteAll(_blocks); }

    void Board::updateBlockyPhysics(sb::Transform transform)
    {
        transform *= getTransform();
        _collider.update(transform, _blocks);
        _tetromino.updateBlockyPhysics(transform);
    }

    void Board::update(Window& window)
    {
        _tetromino.update();
    }

    void Board::draw(DrawTarget& target, DrawStates states)
    {
        states.transform *= getTransform();
        target.draw(_tetromino, states);

        for (size_t i = 0; i < _blocks.size(); i++)
            _batch.draw(*_blocks[i], states);

        target.draw(_batch);
    }
}