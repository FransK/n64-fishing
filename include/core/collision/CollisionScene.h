#pragma once

#include <vector>

#include "Collider.h"
#include "Epa.h"

namespace Collision
{
    class CollisionScene
    {
    public:
        CollisionScene() = default;

        void add(Collider *object, bool isActive = true);
        void remove(Collider *object);
        void activate(Collider *object);
        void deactivate(Collider *object);

        void update(float fixedTimeStep);
        void debugDraw();

    private:
        std::vector<Collider *> colliders{};
        std::vector<Collider *> activeColliders{};

        void runCollision();
        void constrainToWorld(Collider *object);
        void collide(Collider *a, Collider *b);
        void correctOverlap(Collider *object, EpaResult *result, float ratio, float friction, float bounce);
        void correctVelocity(Collider *object, EpaResult *result, float ratio, float friction, float bounce);
    };
}