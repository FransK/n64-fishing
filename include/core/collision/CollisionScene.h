#pragma once

#include <memory>
#include <vector>

#include "Collider.h"
#include "Epa.h"

namespace Collision
{
    class CollisionScene
    {
    public:
        void add(Collider &&object, bool isActive = true);
        void remove(int entityId);
        void activate(int entityId);
        void deactivate(int entityId);

        void update(float fixedTimeStep);
        void debugDraw();

    private:
        std::vector<std::shared_ptr<Collider>> colliders{};
        std::vector<std::shared_ptr<Collider>> activeColliders{};

        void runCollision();
        void constrainToWorld(Collider *object);
        void collide(Collider *a, Collider *b);
        void correctOverlap(Collider *object, EpaResult *result, float ratio, float friction, float bounce);
        void correctVelocity(Collider *object, EpaResult *result, float ratio, float friction, float bounce);
    };
}