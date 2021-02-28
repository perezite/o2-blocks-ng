#include "Board.h"
#include "BoardCollider.h"

using namespace sb;

namespace blocks
{
    void BoardCollider::updateCollider(const sb::Transform& globalBoardTransform, const std::vector<Block>& boardBlocks)
    {
        globalPositions.clear();

        for (size_t i = 0; i < boardBlocks.size(); i++) {
            Vector2f globalBlockPos = globalBoardTransform * boardBlocks[i].getPosition();
            globalPositions.push_back(Vector2i((int)globalBlockPos.x, (int)globalBlockPos.y));
        }
    }
}