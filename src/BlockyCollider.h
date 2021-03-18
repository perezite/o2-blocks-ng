#pragma once

#include "Block.h"
#include "Vector2.h"
#include "Transform.h"
#include "Transformable.h"
#include <vector>

namespace blocks
{
    class Board;

    class BlockyCollider 
    {
        static std::vector<BlockyCollider*> Colliders;

        std::vector<sb::Vector2i> _globalPositions;

        sb::Transform _globalTransform;

        sb::Transform _parentTransform;

        std::vector<sb::Vector2i> _localPositions;

        sb::Transformable& _parent;

    protected:
        bool hasCollision(const std::vector<sb::Vector2i>& leftPositions, const std::vector<sb::Vector2i>& rightPositions);

        //void movePositions(const std::vector<sb::Vector2i>& oldPositions, const sb::Vector2i& displacement, std::vector<sb::Vector2i>& newPositions);

        void transformPositions(const std::vector<sb::Vector2i>& oldPositions, const sb::Transform& transform, std::vector<sb::Vector2i>& resultPositions);


        bool wouldCollide(const sb::Transform& globalTransform);

    public:
        BlockyCollider(sb::Transformable& parent);

        virtual ~BlockyCollider();

        void update(const sb::Transform& parentTransform, const sb::Transform& globalTransform, const std::vector<sb::Vector2i>& localPositions);

        template <class T>
        void update(const sb::Transform& parentTransform, const sb::Transform& globalTransform, const std::vector<T*>& entities) {
            std::vector<sb::Vector2i> positions;
            positions.reserve(entities.size());

            for (size_t i = 0; i < entities.size(); i++) {
                const sb::Vector2f& entityPos = entities[i]->getPosition();
                sb::Vector2i position = sb::Vector2i((int)entityPos.x, (int)entityPos.y);
                positions.push_back(position);
            }

            update(parentTransform, globalTransform, positions);
        }

        bool wouldCollide(const sb::Vector2i& displacement);

        bool wouldCollide(float radians);

        bool wouldCollide(const sb::Vector2i& displacement, float radiansRotation);
    };
}