#include "Board.h"
#include "BlockyCollider.h"
#include "Transformable.h"
#include "Input.h"
#include "Memory.h"
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

    void BlockyCollider::computeGlobalPositions(const sb::Transform& globalTransform, const std::vector<sb::Vector2i>& localPositions)
    {
        transformPositions(localPositions, globalTransform, _globalPositions);
    }

    void BlockyCollider::resolveCollisions()
    {
        vector<BlockyCollider*> colliders(Colliders);
        bool anyCollision = false;

        do {
            anyCollision = false;
            vector<BlockyCollider*> restoredColliders;
            for (size_t i = 0; i < colliders.size(); i++) {
                for (size_t j = 0; j < Colliders.size(); j++) {
                    if (colliders[i] == Colliders[j])
                        continue;
                    
                    bool hasCollision = BlockyCollider::hasCollision(
                        colliders[i]->_globalPositions, Colliders[j]->_globalPositions);
                    if (hasCollision) {
                        anyCollision = true;
                        colliders[i]->_entity = colliders[i]->_lastLocalTransform;
                        restoredColliders.push_back(colliders[i]);
                    }
                }
            }

            erase(colliders, restoredColliders);
        }
        while (anyCollision);

        for (size_t i = 0; i < Colliders.size(); i++)
            Colliders[i]->_lastLocalTransform = Colliders[i]->_entity;
    }

    BlockyCollider::BlockyCollider(Transformable& parent) : _lastLocalTransform(parent), _entity(parent)
    {
        Colliders.push_back(this);
    }

    BlockyCollider::~BlockyCollider()
    {
        Colliders.erase(remove(Colliders.begin(), Colliders.end(), this), Colliders.end());
    }

    void BlockyCollider::update(const Transform& globalTransform, const vector<Vector2i>& localPositions)
    {
        _currentGlobalTransform = globalTransform;
        computeGlobalPositions(globalTransform, localPositions);
    }
}