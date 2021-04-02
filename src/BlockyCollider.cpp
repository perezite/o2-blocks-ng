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

    void BlockyCollider::getGlobalPositions(const Vector2i& deltaPosition, float deltaRadians, vector<Vector2i>& result)
    {
        Vector2f entityPosition = _entityLocalTransform.getPosition() + toVector2f(deltaPosition);
        float entityRotation = _entityLocalTransform.getRotation() + deltaRadians;
        Transform localTransform(entityPosition, 1, entityRotation);
        Transform globalTransform = _parentEntityGlobalTransform * localTransform;

        transformPositions(_localPositions, globalTransform, result);
    }

    bool BlockyCollider::wouldCollide(const vector<Vector2i>& globalPositions)
    {
        for (size_t i = 0; i < Colliders.size(); i++) {
            if (Colliders[i] != this) {
                bool hasCollision = this->hasCollision(globalPositions, Colliders[i]->getGlobalPositions());
                if (hasCollision)
                    return true;
            }
        }

        return false;
    }

    BlockyCollider::BlockyCollider(Transformable& parent) : _globalPositionsNeedUpdate(true), 
        _entity(parent)
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
    }
   
    bool BlockyCollider::wouldCollide(const Vector2i& deltaPosition, float deltaRadians)
    {
        vector<Vector2i> globalPositions;
        getGlobalPositions(deltaPosition, deltaRadians, globalPositions);

        return wouldCollide(globalPositions);
    }

    const IntRect BlockyCollider::getGlobalBounds(const Vector2i& deltaPosition, float deltaRadians)
    {
        vector<Vector2i> globalPositions;
        getGlobalPositions(deltaPosition, deltaRadians, globalPositions);
        IntRect globalBounds = computeBounds(globalPositions);

        return globalBounds;
    }
}