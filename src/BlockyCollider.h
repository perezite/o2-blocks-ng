#pragma once

#include "Block.h"
#include "Vector2.h"
#include "Transform.h"
#include "Transformable.h"
#include "VectorHelper.h"
#include <vector>

namespace blocks
{
    class Board;

    class BlockyCollider 
    {
        static std::vector<BlockyCollider*> Colliders;

        std::vector<sb::Vector2i> _globalPositions;

        sb::Transform _currentGlobalTransform;

        sb::Transformable _lastLocalTransform;

        sb::Transformable& _entity;

    protected:
        static bool hasCollision(const std::vector<sb::Vector2i>& leftPositions, const std::vector<sb::Vector2i>& rightPositions);

        void transformPositions(const std::vector<sb::Vector2i>& oldPositions, const sb::Transform& transform, std::vector<sb::Vector2i>& resultPositions) const;

        void computeGlobalPositions(const sb::Transform& globalTransform, const std::vector<sb::Vector2i>& localPositions);

    public:
        static void resolveCollisions();
        
        BlockyCollider(sb::Transformable& parent);

        virtual ~BlockyCollider();

        void update(const sb::Transform& globalTransform, const std::vector<sb::Vector2i>& localPositions);

        template <class T>
        void update(const sb::Transform& globalTransform, const std::vector<T*>& entities) {
            std::vector<sb::Vector2i> positions;
            positions.reserve(entities.size());

            for (size_t i = 0; i < entities.size(); i++) {
                const sb::Vector2f& entityPos = entities[i]->getPosition();
                sb::Vector2i position = toVector2i(entityPos);
                positions.push_back(position);
            }

            update(globalTransform, positions);
        }
    };
}