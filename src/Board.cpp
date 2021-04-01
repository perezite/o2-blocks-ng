#include "Board.h"
#include "Block.h"
#include "Window.h"
#include "Memory.h"
#include "DrawTarget.h"

using namespace sb;

namespace blocks
{
    Board::Board(GameAssets& assets, size_t width, size_t height) :
        _tetromino(assets.squareTextureAtlas, IntRect(0, 0, width, height), TetrominoType::Simple /*TetrominoType::T*/), 
        _blocksCollider(*this)
    { 
        _blocks.push_back(new Block(assets.blockTextureAtlas));
        //_blocks[0]->setPosition(3, 14);
        //_tetromino.setPosition(5, 16);
        _blocks[0]->setPosition(3, 3);
        _tetromino.setPosition(1, 1);
    }

    Board::~Board() { deleteAll(_blocks); }

    void Board::updateColliders(Transform parentTransform)
    {
        _blocksCollider.update(parentTransform, _blocks);
        _tetromino.updateColliders(parentTransform * getTransform());
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