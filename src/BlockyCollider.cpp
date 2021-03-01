#include "Board.h"
#include "BlockyCollider.h"
#include <algorithm>

using namespace std;
using namespace sb;

namespace blocks
{
    std::vector<BlockyCollider*> BlockyCollider::Colliders;

    bool BlockyCollider::hasCollisions(const std::vector<sb::Vector2i>& leftPositions, const std::vector<sb::Vector2i>& rightPositions)
    {
        for (size_t i = 0; i < leftPositions.size(); i++) {
            for (size_t j = 0; j < rightPositions.size(); j++) {
                if (leftPositions[i] == rightPositions[j])
                    return true;
            }
        }

        return false;
    }

    void BlockyCollider::displacePositions(const std::vector<sb::Vector2i>& oldPositions, const sb::Vector2i& displacement, std::vector<sb::Vector2i>& newPositions)
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
        Colliders.erase(std::remove(Colliders.begin(), Colliders.end(), this), Colliders.end());
    }

    void BlockyCollider::update(const sb::Transform& globalTransform, const vector<Vector2i>& positions)
    {
        _globalPositions.clear();

        for (size_t i = 0; i < positions.size(); i++) {
            Vector2f globalPosition = globalTransform * Vector2f((float)positions[i].x, (float)positions[i].y);
            _globalPositions.push_back(Vector2i((int)globalPosition.x, (int)globalPosition.y));
        }
    }
    
    bool BlockyCollider::wouldCollide(const sb::Vector2i& displacement)
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