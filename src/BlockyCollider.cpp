#include "Board.h"
#include "BlockyCollider.h"
#include <algorithm>

using namespace std;
using namespace sb;

namespace blocks
{
    std::vector<BlockyCollider*> BlockyCollider::Colliders;

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
}