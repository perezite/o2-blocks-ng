#pragma once

#include "Block.h"
#include "Vector2.h"
#include "Transform.h"
#include "Transformable.h"
#include "VectorHelper.h"
#include "Rect.h"
#include <vector>

namespace blocks
{
    class Board;

    class BlockyCollider 
    {
        static std::vector<BlockyCollider*> Colliders;

        std::vector<sb::Vector2i> _localPositions;

        std::vector<sb::Vector2i> _globalPositions;

        bool _globalPositionsNeedUpdate;

        sb::Transformable _entityLocalTransform;

        sb::Transform _parentEntityGlobalTransform;

        sb::Transformable& _entity;

        sb::IntRect _globalBounds;

        bool _globalBoundsNeedUpdate;

    protected:
        bool hasCollision(const std::vector<sb::Vector2i>& leftPositions, const std::vector<sb::Vector2i>& rightPositions);

        void transformPositions(const std::vector<sb::Vector2i>& oldPositions, const sb::Transform& transform, std::vector<sb::Vector2i>& resultPositions) const;

        bool wouldCollide(const sb::Transform& globalTransform);

        void getGlobalPositions(const sb::Vector2i& deltaPosition, std::vector<sb::Vector2i>& result);

        void computeBounds(const std::vector<sb::Vector2i>& positions, sb::IntRect& result);

    public:
        BlockyCollider(sb::Transformable& parent);

        virtual ~BlockyCollider();

        const std::vector<sb::Vector2i>& getGlobalPositions();

        void update(const sb::Transform& parentTransform, const std::vector<sb::Vector2i>& localPositions);

        template <class T>
        void update(const sb::Transform& parentTransform, const std::vector<T*>& entities) {
            std::vector<sb::Vector2i> positions;
            positions.reserve(entities.size());

            for (size_t i = 0; i < entities.size(); i++) {
                const sb::Vector2f& entityPos = entities[i]->getPosition();
                sb::Vector2i position = toVector2i(entityPos);
                positions.push_back(position);
            }

            update(parentTransform, positions);
        }

        bool wouldCollide(const sb::Vector2i& deltaPosition, float deltaRadians);

        const sb::IntRect& getGlobalBounds(const sb::Vector2i& deltaPosition);
    };
}