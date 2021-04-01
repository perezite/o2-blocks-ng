#include "Board.h"
#include "BlockyCollider.h"
#include "Transformable.h"
#include "Input.h"
#include "Memory.h"
#include "Math.h"
#include <algorithm>
#include <math.h>
#include <limits.h>

using namespace std;
using namespace sb;

namespace blocks
{
    vector<BlockyCollider*> BlockyCollider::Colliders;

    bool BlockyCollider::hasCollision(const vector<Vector2i>& leftPositions, const vector<Vector2i>& rightPositions)
    {
        for (size_t i = 0; i < leftPositions.size(); i++) {
            for (size_t j = 0; j < rightPositions.size(); j++) {
                if (leftPositions[i] == rightPositions[j])
                    return true;
            }
        }

        return false;
    }

    void BlockyCollider::transformPositions(const vector<Vector2i>& oldPositions, const Transform& transform, vector<Vector2i>& newPositions) const
    {
        newPositions.clear();
        newPositions.reserve(oldPositions.size());

        for (size_t i = 0; i < oldPositions.size(); i++) {
            Vector2f temp = transform * toVector2f(oldPositions[i]);
            newPositions.push_back(toVector2i(temp));
        }
    }

    bool BlockyCollider::wouldCollide(const Transform& globalTransform)
    {
        vector<Vector2i> newGlobalPositions;
        transformPositions(_localPositions, globalTransform, newGlobalPositions);

        for (size_t i = 0; i < Colliders.size(); i++) {
            if (Colliders[i] != this) {
                bool hasCollision = this->hasCollision(newGlobalPositions, Colliders[i]->getGlobalPositions());
                if (hasCollision)
                    return true;
            }
        }

        return false;
    }

    void BlockyCollider::getGlobalPositions(const Vector2i& deltaPosition, vector<Vector2i>& result)
    {
        const vector<Vector2i>& globalPositions = getGlobalPositions();
        result.reserve(globalPositions.size()); result.clear();

        for (size_t i = 0; i < globalPositions.size(); i++)
            result.push_back(globalPositions[i] + deltaPosition);
    }

    void BlockyCollider::computeBounds(const vector<Vector2i>& positions, IntRect& result)
    {
        int minX = INT_MAX, minY = INT_MAX;
        int maxX = INT_MIN, maxY = INT_MIN;
        
        for (size_t i = 0; i < positions.size(); i++) {
            minX = min(positions[i].x, minX);
            minY = min(positions[i].y, minY);
            maxX = max(positions[i].x, maxX);
            maxY = max(positions[i].y, maxY);
        }

        result.left = minX; result.width = maxX - minX + 1;
        result.bottom = minY; result.height = maxY - minY + 1;
    }

    BlockyCollider::BlockyCollider(Transformable& parent) : _globalPositionsNeedUpdate(true), 
        _entity(parent), _globalBoundsNeedUpdate(true)
    {
        Colliders.push_back(this);
    }

    BlockyCollider::~BlockyCollider()
    {
        Colliders.erase(remove(Colliders.begin(), Colliders.end(), this), Colliders.end());
    }

    const vector<Vector2i>& BlockyCollider::getGlobalPositions()
    {
        if (_globalPositionsNeedUpdate) {
            Transform globalTransform = _parentEntityGlobalTransform * _entityLocalTransform.getTransform();
            transformPositions(_localPositions, globalTransform, _globalPositions);
            _globalPositionsNeedUpdate = false;
        }

        return _globalPositions;
    }

    void BlockyCollider::update(const Transform& parentEntityTransform, const vector<Vector2i>& localPositions)
    {
        _parentEntityGlobalTransform = parentEntityTransform;
        _entityLocalTransform = _entity;
        _localPositions = localPositions;
        _globalPositionsNeedUpdate = true;
        _globalBoundsNeedUpdate = true;
    }
   
    bool BlockyCollider::wouldCollide(const Vector2i& deltaPosition, float deltaRadians)
    {
        Vector2f entityPosition = _entityLocalTransform.getPosition() + toVector2f(deltaPosition);
        float entityRotation = _entityLocalTransform.getRotation() + deltaRadians;
        Transform localTransform(entityPosition, 1, entityRotation);
        Transform globalTransform = _parentEntityGlobalTransform * localTransform;

        return wouldCollide(globalTransform);
    }

    //const IntRect& BlockyCollider::getGlobalBounds(const Vector2i& deltaPosition)
    //{
    //    if (_globalBoundsNeedUpdate) {
    //        vector<Vector2i> globalPositions; 
    //        getGlobalPositions(deltaPosition, globalPositions);
    //        computeBounds(globalPositions, _globalBounds);
    //    }

    //    return _globalBounds;
    //}

    const IntRect BlockyCollider::getGlobalBounds(const Vector2i& deltaPosition, float deltaRadians)
    {
        Vector2i bottomLeft, topRight;
        sb::computeBounds(_localPositions, bottomLeft, topRight);
        FloatRect localBounds(toVector2f(bottomLeft), toVector2f(topRight));

        Vector2f entityPosition = _entityLocalTransform.getPosition() + toVector2f(deltaPosition);
        float entityRotation = _entityLocalTransform.getRotation() + deltaRadians;
        Transform localTransform(entityPosition, 1, entityRotation);
        Transform globalTransform = _parentEntityGlobalTransform * localTransform;

        FloatRect globalBounds = globalTransform * localBounds;
        return IntRect((int)globalBounds.left, (int)globalBounds.bottom, 
            (int)globalBounds.width, (int)globalBounds.height);

        //if (_globalBoundsNeedUpdate)
        //    computeBounds(getGlobalPositions(), _globalBounds);

        //Transform localTransform = Transform(toVector2f(deltaPosition), 0, deltaRadians);
        //Transform globalTransform = _parentEntityGlobalTransform * localTransform;

        //FloatRect temp((float)_globalBounds.left, (float)_globalBounds.bottom, 
        //    (float)_globalBounds.width, (float)_globalBounds.height);
        //temp = globalTransform * temp;
        //return IntRect((int)temp.left, (int)temp.bottom, (int)temp.width, (int)temp.height);

    }
}