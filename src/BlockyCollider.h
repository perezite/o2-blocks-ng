#pragma once

#include "Block.h"
#include "Vector2.h"
#include "Transform.h"
#include <vector>

namespace blocks
{
    class Board;

    class BlockyCollider 
    {
        static std::vector<BlockyCollider*> Colliders;

        std::vector<sb::Vector2i> _globalPositions;

        sb::Transform _globalTransform;

        std::vector<sb::Vector2i> _localPositions;

    protected:
        bool hasCollision(const std::vector<sb::Vector2i>& leftPositions, const std::vector<sb::Vector2i>& rightPositions);

        //void movePositions(const std::vector<sb::Vector2i>& oldPositions, const sb::Vector2i& displacement, std::vector<sb::Vector2i>& newPositions);

        void transformPositions(const std::vector<sb::Vector2i>& oldPositions, const sb::Transform& transform, std::vector<sb::Vector2i>& resultPositions);


        bool wouldCollide(const sb::Transform& globalTransform);

    public:
        BlockyCollider();

        virtual ~BlockyCollider();

        void update(const sb::Transform& globalTransform, const std::vector<sb::Vector2i>& localPositions);

        template <class T>
        void update(const sb::Transform& globalTransform, const std::vector<T*>& entities) {
            std::vector<sb::Vector2i> positions;
            positions.reserve(entities.size());

            for (size_t i = 0; i < entities.size(); i++) {
                const sb::Vector2f& entityPos = entities[i]->getPosition();
                sb::Vector2i position = sb::Vector2i((int)entityPos.x, (int)entityPos.y);
                positions.push_back(position);
            }

            update(globalTransform, positions);
        }

        bool wouldCollide(const sb::Vector2i& displacement);

        bool wouldCollide(float radians);
    };
}