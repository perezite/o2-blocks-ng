#include "Board.h"
#include "Block.h"
#include "Window.h"
#include "Memory.h"
#include "DrawTarget.h"

using namespace sb;

namespace blocks
{
    void Board::respawnTetromino()
    {
        safeDelete(_tetromino);

        _tetromino = new Tetromino(_assets.squareTextureAtlas, 
            IntRect(0, 0, _size.x, _size.y), TetrominoType::T);

        _tetromino->setPosition(5, 16);
    }

    Board::Board(GameAssets& assets, size_t width, size_t height) : 
        _assets(assets), _tetromino(NULL), _size(width, height), _blocksCollider(*this)
    { 
        _blocks.push_back(new Block(assets.blockTextureAtlas));
        _blocks[0]->setPosition(3, 14);
        respawnTetromino();

        //_blocks[0]->setPosition(3, 3);
        //_tetromino.setPosition(1, 1);
    }

    Board::~Board() 
    {
        deleteAll(_blocks); 
        delete _tetromino;
    }

    void Board::updateColliders(Transform parentTransform)
    {
        _blocksCollider.update(parentTransform, _blocks);
        _tetromino->updateColliders(parentTransform * getTransform());
    }

    void Board::update(Window& window)
    {
        _tetromino->update();

        if (_tetromino->isDead())
            respawnTetromino();
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