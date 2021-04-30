#include "Board.h"
#include "Block.h"
#include "Window.h"
#include "Memory.h"
#include "Math.h"
#include "Rect.h"
#include "DrawTarget.h"
#include "VectorHelper.h"

using namespace sb;
using namespace std;

namespace blocks
{
    void Board::respawnTetromino()
    {
        safeDelete(_tetromino);

        _tetromino = new Tetromino(_assets.squareTextureAtlas, TetrominoType::T);
        _tetromino->setPosition(5, 16);
        
        _collisionLogic.resetTetrominoData();
    }

    void Board::updateSelf()
    {
        if (_collisionLogic.isTetrominoDead())
        {
            cout << "respawn from position " << _tetromino->getPosition().x << " " << _tetromino->getPosition().y << endl;
            respawnTetromino();
        }

        if (_collisionLogic.isTetrominoStuck())
        {
            while (true)
            {
                cout << "Game over!!" << endl;
                cin.get();
            }
        }
    }

    void Board::harddropTetromino()
    {
        do
        {
            _tetromino->translate(0, -1);
        } while (!_collisionLogic.hasTetrominoCollision());
    }

    Board::Board(GameAssets& assets, size_t width, size_t height) : 
        _assets(assets), _size(width, height), _tetromino(NULL), 
        _collisionLogic(*this)
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

    void Board::getBlockPositions(std::vector<sb::Vector2i>& result)
    {
        result.clear(); result.reserve(_blocks.size());

        for (size_t i = 0; i < _blocks.size(); i++)
        {
            Vector2i blockPosition = toVector2i(_blocks[i]->getPosition());
            result.push_back(blockPosition);
        }
    }

    void Board::update(Window& window)
    {
        _tetromino->update();
        if (Input::isKeyGoingDown(KeyCode::Space))
            harddropTetromino();
        _collisionLogic.update();
        updateSelf();
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