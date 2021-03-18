#include "Board.h"
#include "BlockyCollider.h"
#include "Transformable.h"
#include "Input.h"
#include <algorithm>
#include <math.h>

using namespace std;
using namespace sb;

namespace {
    Vector2f toVector2f(const Vector2i v) {
        return Vector2f(float(v.x), float(v.y));
    }

    Vector2i toVector2i(const Vector2f v) {
        return Vector2i((int)round(v.x), (int)round(v.y));
    }
}

namespace blocks
{
    vector<BlockyCollider*> BlockyCollider::Colliders;

    bool BlockyCollider::hasCollision(const vector<Vector2i>& leftPositions, 
        const vector<Vector2i>& rightPositions)
    {
        for (size_t i = 0; i < leftPositions.size(); i++) {
            for (size_t j = 0; j < rightPositions.size(); j++) {
                if (leftPositions[i] == rightPositions[j])
                    return true;
            }
        }

        return false;
    }

    void BlockyCollider::transformPositions(const vector<Vector2i>& oldPositions, const Transform& transform, vector<Vector2i>& newPositions) 
    {
        newPositions.clear();
        newPositions.reserve(oldPositions.size());

        for (size_t i = 0; i < oldPositions.size(); i++) {
            Vector2f temp = transform * toVector2f(oldPositions[i]);
            newPositions.push_back(toVector2i(temp));
        }
    }

    bool BlockyCollider::wouldCollide(const sb::Transform& globalTransform)
    {
        vector<Vector2i> newGlobalPositions;
        transformPositions(_localPositions, globalTransform, newGlobalPositions);

        for (size_t i = 0; i < Colliders.size(); i++) {
            if (Colliders[i] != this) {
                bool hasCollision = this->hasCollision(newGlobalPositions, Colliders[i]->_globalPositions);
                if (hasCollision)
                    return true;
            }
        }

        return false;
    }

    BlockyCollider::BlockyCollider(Transformable& parent) : _parent(parent)
    {
        Colliders.push_back(this);
    }

    BlockyCollider::~BlockyCollider()
    {
        Colliders.erase(remove(Colliders.begin(), Colliders.end(), this), Colliders.end());
    }

    void BlockyCollider::update(const Transform& parentTransform, const Transform& globalTransform, const vector<Vector2i>& localPositions)
    {
        _globalTransform = globalTransform;
        _parentTransform = parentTransform;
        _localPositions = localPositions;
        transformPositions(localPositions, globalTransform, _globalPositions);

        if (Input::isKeyGoingDown(KeyCode::c) && _globalPositions.size() > 1) {
            for (size_t i = 0; i < _globalPositions.size(); i++)
                cout << "(" << _globalPositions[i].x << ", " << _globalPositions[i].y << ") ";
            cout << endl;
        }
    }
    
    bool BlockyCollider::wouldCollide(const Vector2i& displacement)
    {
        Transform newTransform(_globalTransform);
        newTransform.translate(toVector2f(displacement));
        return wouldCollide(newTransform);
    }

    bool BlockyCollider::wouldCollide(float radians)
    {
        Transform newTransform(_globalTransform);
        newTransform.rotate(radians);
        return wouldCollide(newTransform);
    }

    bool BlockyCollider::wouldCollide(const sb::Vector2i& displacement, float radians)
    {
        Vector2f entityPosition = _parent.getPosition() + toVector2f(displacement);
        float entityRotation = _parent.getRotation() + radians;
        Transform localTransform(entityPosition, Vector2f(1), entityRotation);
        Transform globalTransform = _parentTransform * localTransform;

        return wouldCollide(globalTransform);

        //Transform newTransform(_parentTransform);
        //newTransform *= Transform(position, Vector2f(1, 1), radians);
        //return wouldCollide(newTransform);
    }    
}