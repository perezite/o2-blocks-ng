#pragma once

#include "Block.h"
#include "Vector2.h"
#include "Transform.h"
#include <vector>

namespace blocks
{
    class Board;

    class BoardCollider 
    {
        Board& _board;
        std::vector<sb::Vector2i> globalPositions;

    public:
        BoardCollider(Board& board)
            : _board(board)
        { }

        void updateCollider(const sb::Transform& globalBoardTransform, const std::vector<Block>& boardBlocks);
    };
}