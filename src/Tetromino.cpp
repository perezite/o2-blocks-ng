#include "Tetromino.h"
#include "DrawTarget.h"
#include "Rect.h"
#include "Memory.h"
#include "Input.h"
#include "Math.h"
#include "Settings.h"
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
    void Tetromino::setSquarePositions(const vector<Vector2i>& positions)
    {
        _squarePositions.clear();
        _squarePositions.assign(positions.begin(), positions.end());
    }

    void Tetromino::setSquarePositions(BlockType type)
    {
        if (type == BlockType::T)
            setSquarePositions(TShapeSquarePositions);
        else if (type == BlockType::Simple)
            setSquarePositions(SimpleShapeSquarePositions);
        else if (type == BlockType::VerySimple)
            setSquarePositions(VerySimpleShapeSquarePositions);
        else
            SB_ERROR("Invalid block type " << (int)type);
    }

    void Tetromino::checkMoveInput(sb::KeyCode keyCode, int deltaX, int deltaY)
    {
        if (Input::isKeyGoingDown(keyCode))
            translate((float)deltaX, (float)deltaY);
    }

    void Tetromino::updateInput()
    {
        checkMoveInput(KeyCode::Left, -1, 0);
        checkMoveInput(KeyCode::Right, +1, 0);
        checkMoveInput(KeyCode::Up, 0, +1);
        checkMoveInput(KeyCode::Down, 0, -1);

        if (Input::isKeyGoingDown(KeyCode::r))
            rotate(-90 * ToRadians);

        #if _DEBUG
            if(Input::isKeyGoingDown(KeyCode::d))
                autodropEnabled = !autodropEnabled;
        #endif
    }

    void Tetromino::autodrop()
    {
        while (_autodropTimer.hasTicks())
            translate(0, -1);
    }

    Tetromino::Tetromino(TextureAtlas& squareTextures, BlockType type)
        : _squareTextures(squareTextures), _autodropTimer(settings::autodropSeconds)
    {
        setType(type);
    }

    void Tetromino::getTransformedSquarePositions(vector<Vector2i>& result)
    {
        result.clear(); result.reserve(_squarePositions.size());

        for (size_t i = 0; i < _squarePositions.size(); i++) {
            Vector2f temp = getTransform() * toVector2f(_squarePositions[i]);
            result.push_back(toVector2i(temp));
        }
    }

    void Tetromino::setType(BlockType type)
    {
        _type = type;

        Vector2i texturePosition = BlockTypeHelper::getTexturePosition(type);
        _squareSprite.setTexture(_squareTextures.getTextureSheet(), _squareTextures.getTextureArea(texturePosition));
        
        setSquarePositions(type);
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