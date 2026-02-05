#pragma once

#include <memory>
#include "containers/Vector.h"

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
        Containers::vector<std::shared_ptr<Collider>> colliders{};       // TODO make unique
        Containers::vector<std::shared_ptr<Collider>> activeColliders{}; // TODO make unique

        void runCollision();
        void constrainToWorld(Collider *object);
        void collide(Collider *a, Collider *b);
        void correctOverlap(Collider *object, EpaResult *result, float ratio, float friction, float bounce);
        void correctVelocity(Collider *object, EpaResult *result, float ratio, float friction, float bounce);
    };
}