#include "Board.h"
#include "BlockyCollider.h"
#include "Transformable.h"
#include "Input.h"
#include <algorithm>
#include <math.h>

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

    bool BlockyCollider::wouldCollide(const sb::Transform& globalTransform)
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

    bool BlockyCollider::wouldCollide(const sb::Vector2i& deltaPosition, float deltaRadians)
    {
        Vector2f entityPosition = _entity.getPosition() + toVector2f(deltaPosition);
        float entityRotation = _entity.getRotation() + deltaRadians;
        Transform localTransform(entityPosition, 1, entityRotation);
        Transform globalTransform = _parentEntityTransform * localTransform;

        return wouldCollide(globalTransform);
    }

    BlockyCollider::BlockyCollider(Transformable& parent) : _globalPositionsNeedUpdate(true), _entity(parent)
    {
        Colliders.push_back(this);
    }

    BlockyCollider::~BlockyCollider()
    {
        Colliders.erase(remove(Colliders.begin(), Colliders.end(), this), Colliders.end());
    }

    const std::vector<sb::Vector2i>& BlockyCollider::getGlobalPositions()
    {
        if (_globalPositionsNeedUpdate) {
            Transform globalTransform = _parentEntityTransform * _entity.getTransform();
            transformPositions(_localPositions, globalTransform, _globalPositions);
            _globalPositionsNeedUpdate = false;
        }

        return _globalPositions;
    }

    void BlockyCollider::update(const Transform& parentEntityTransform, const vector<Vector2i>& localPositions)
    {
        _parentEntityTransform = parentEntityTransform;
        _localPositions = localPositions;
        _globalPositionsNeedUpdate = true;
    }
   

    bool BlockyCollider::wouldCollide(const sb::Vector2i& deltaPosition)
    {
        return wouldCollide(deltaPosition, 0);
    }

    bool BlockyCollider::wouldCollide(float deltaRadians)
    {
        return wouldCollide(0, deltaRadians);
    }
}