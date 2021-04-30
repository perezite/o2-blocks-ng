#include "Tetromino.h"
#include "DrawTarget.h"
#include "Rect.h"
#include "Memory.h"
#include "Input.h"
#include "Math.h"
#include "Configuration.h"
#include "VectorHelper.h"

using namespace std;
using namespace sb;

namespace
{
    bool autodropEnabled = false;
    const vector<Vector2i> TShapeSquarePositions = { Vector2i(0, 0), Vector2i(-1, 0), Vector2i(1, 0), Vector2i(0, 1) };
    const vector<Vector2i> SimpleShapeSquarePositions = { Vector2i(0, 0), Vector2i(1, 0) };
    const vector<Vector2i> VerySimpleShapeSquarePositions = { Vector2i(0, 0) };
}

namespace blocks
{
    void Tetromino::setSquares(const vector<Vector2i>& squarePositions, size_t texPosX, size_t texPosY)
    {
        _squareSprite.setTexture(_squareTextures.getTextureSheet(), _squareTextures.getTextureArea(texPosX, texPosY));
        _squarePositions.clear();
        _squarePositions.assign(squarePositions.begin(), squarePositions.end());
    }

    void Tetromino::checkMoveInput(sb::KeyCode keyCode, int deltaX, int deltaY)
    {
        if (Input::isKeyGoingDown(keyCode))
            translate((float)deltaX, (float)deltaY);
    }

    void Tetromino::harddrop()
    {
        // Todo
    }

    void Tetromino::updateInput()
    {
        checkMoveInput(KeyCode::Left, -1, 0);
        checkMoveInput(KeyCode::Right, +1, 0);
        checkMoveInput(KeyCode::Up, 0, +1);
        checkMoveInput(KeyCode::Down, 0, -1);

        if (Input::isKeyGoingDown(KeyCode::Space))
            harddrop();

        if (Input::isKeyGoingDown(KeyCode::r))
            rotate(-90 * ToRadians);

        #if _DEBUG
            if(Input::isKeyGoingDown(KeyCode::d))
                autodropEnabled = !autodropEnabled;
        #endif
    }

    void Tetromino::autodrop()
    {
        while (_autodropChronometer.hasTicks())
            checkMoveInput(KeyCode::Down, 0, -1);
    }

    Tetromino::Tetromino(TextureAtlas& squareTextures, TetrominoType type)
        : _squareTextures(squareTextures), _autodropChronometer(configuration::autodropSeconds)
    {
        setType(type);
    }

    void Tetromino::getTransformedSquarePositions(vector<Vector2i>& result)
    {
        result.clear(); result.reserve(_squarePositions.size());

        for (size_t i = 0; i < _squarePositions.size(); i++)
        {
            Vector2f temp = getTransform() * toVector2f(_squarePositions[i]);
            result.push_back(toVector2i(temp));
        }
    }

    void Tetromino::setType(TetrominoType type)
    {
        if (type == TetrominoType::T)
            setSquares(TShapeSquarePositions, 0, 0);
        else if (type == TetrominoType::Simple)
            setSquares(SimpleShapeSquarePositions, 0, 0);
        else if (type == TetrominoType::VerySimple)
            setSquares(VerySimpleShapeSquarePositions, 0, 0);
        else
            SB_ERROR("Invalid tetromino type " << (int)type);
    }

    void Tetromino::draw(DrawTarget& target, DrawStates drawStates)
    {
        // we transform the origin, such that rotations and scaling are applied relative to the center of the tetromino
        static const Transform originTransform(-.5f);
        static const Transform inverseOriginTransform(.5f);
        drawStates.transform *= inverseOriginTransform * getTransform() * originTransform;

        for (size_t i = 0; i < _squarePositions.size(); i++) {
            _squareSprite.setPosition(toVector2f(_squarePositions[i]));
            target.draw(_squareSprite, drawStates);
        }
    }

    void Tetromino::update()
    {
        updateInput();
        
        if (autodropEnabled)
            autodrop();
    }
}