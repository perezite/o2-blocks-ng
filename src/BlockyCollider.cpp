#include "Board.h"
#include "BlockyCollider.h"
#include "Transformable.h"
#include "Input.h"
#include "Memory.h"
#include <algorithm>
#include <math.h>

using namespace std;
using namespace sb;

namespace {
    bool minX(const Vector2i& left, const Vector2i& right) { return left.x < right.x; }
    bool minY(const Vector2i& left, const Vector2i& right) { return left.y < right.y; }
    bool maxX(const Vector2i& left, const Vector2i& right) { return left.x > right.x; }
    bool maxY(const Vector2i& left, const Vector2i& right) { return left.y > right.y; }
}

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

    BlockyCollider::BlockyCollider(Transformable& parent) : _globalPositionsNeedUpdate(true), 
        _entity(parent), _globalBoundsNeedUpdate(true)
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
            Transform globalTransform = _parentEntityGlobalTransform * _entityLocalTransform.getTransform();
            transformPositions(_localPositions, globalTransform, _globalPositions);
            _globalPositionsNeedUpdate = false;
        }

        return _globalPositions;
    }

    const vector<Vector2i> BlockyCollider::getGlobalPositions(const Vector2i& deltaPosition)
    {
        vector<Vector2i> positions(getGlobalPositions());

        for (size_t i = 0; i < positions.size(); i++)
            positions[i] += deltaPosition;

        return positions;
    }

    void BlockyCollider::update(const Transform& parentEntityTransform, const vector<Vector2i>& localPositions)
    {
        _parentEntityGlobalTransform = parentEntityTransform;
        _entityLocalTransform = _entity;
        _localPositions = localPositions;
        _globalPositionsNeedUpdate = true;
        _globalBoundsNeedUpdate = true;
    }
   
    bool BlockyCollider::wouldCollide(const sb::Vector2i& deltaPosition, float deltaRadians)
    {
        Vector2f entityPosition = _entityLocalTransform.getPosition() + toVector2f(deltaPosition);
        float entityRotation = _entityLocalTransform.getRotation() + deltaRadians;
        Transform localTransform(entityPosition, 1, entityRotation);
        Transform globalTransform = _parentEntityGlobalTransform * localTransform;

        return wouldCollide(globalTransform);
    }

    sb::IntRect BlockyCollider::getGlobalBounds(const sb::Vector2i& deltaPosition)
    {
        if (_globalBoundsNeedUpdate) {
            const vector<Vector2i>& globalPositions = getGlobalPositions();
            const Vector2i& theMinX = *min_element(globalPositions, minX);
        }
    }
}