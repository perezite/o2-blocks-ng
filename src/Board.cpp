#include "Board.h"
#include "Block.h"
#include "Window.h"
#include "Memory.h"
#include "DrawTarget.h"

using namespace sb;

namespace blocks
{
    Board::Board(GameAssets& assets) :
        _tetromino(assets.squareTextures, TetrominoType::T), 
        _collider(*this)
    { 
        _blocks.push_back(new Block(assets.blockTextures));
        _blocks[0]->setPosition(3, 14);
        _tetromino.setPosition(5, 16);
    }

    //Board::Board(GameAssets& assets) :
    //    _tetromino(assets.squareTextures, TetrominoType::Simple),
    //    _collider(*this)
    //{
    //    _blocks.push_back(new Block(assets.blockTextures));
    //    _blocks[0]->setPosition(0, 0);
    //    _tetromino.setPosition(1, 0);
    //}

    Board::~Board() { deleteAll(_blocks); }

    void Board::start() { }

    void Board::updateColliders(Transform transform)
    {
        Transform parentTransform = transform;
        transform *= getTransform();
        _collider.update(parentTransform, transform, _blocks);
        _tetromino.updateColliders(transform);
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