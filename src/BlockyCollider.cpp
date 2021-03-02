#include "Board.h"
#include "BlockyCollider.h"
#include <algorithm>

using namespace std;
using namespace sb;

namespace blocks
{
    vector<BlockyCollider*> BlockyCollider::Colliders;

    bool BlockyCollider::hasCollisions(const vector<Vector2i>& leftPositions, const vector<Vector2i>& rightPositions)
    {
        for (size_t i = 0; i < leftPositions.size(); i++) {
            for (size_t j = 0; j < rightPositions.size(); j++) {
                if (leftPositions[i] == rightPositions[j])
                    return true;
            }
        }

        return false;
    }

    void BlockyCollider::displacePositions(const vector<Vector2i>& oldPositions, const Vector2i& displacement, vector<Vector2i>& newPositions)
    {
        newPositions.clear();
        newPositions.reserve(oldPositions.size());
        for (size_t i = 0; i < oldPositions.size(); i++)
            newPositions.push_back(oldPositions[i] + displacement);
    }

    BlockyCollider::BlockyCollider()
    {
        Colliders.push_back(this);
    }

    BlockyCollider::~BlockyCollider()
    {
        Colliders.erase(remove(Colliders.begin(), Colliders.end(), this), Colliders.end());
    }

    void BlockyCollider::update(const Transform& globalTransform, const vector<Vector2i>& positions)
    {
        _globalPositions.clear();

        for (size_t i = 0; i < positions.size(); i++) {
            Vector2f globalPosition = globalTransform * Vector2f((float)positions[i].x, (float)positions[i].y);
            _globalPositions.push_back(Vector2i((int)globalPosition.x, (int)globalPosition.y));
        }
    }
    
    bool BlockyCollider::wouldCollide(const Vector2i& displacement)
    {
        vector<Vector2i> newPositions;
        displacePositions(_globalPositions, displacement, newPositions);

        for (size_t i = 0; i < Colliders.size(); i++) {
            if (Colliders[i] != this) {
                bool hasCollisions = this->hasCollisions(newPositions, Colliders[i]->_globalPositions);
                if (hasCollisions)
                    return true;
            }
        }

        return false;
    }
}