#pragma once

#include <vector>
#include "collider.h"

using namespace Collision;

namespace Collision
{
    class Scene
    {
    private:
        std::vector<Collider *> colliders;

        void updateCollider(Collider *object, float fixedTimeStep);

    public:
        Scene();
        ~Scene();

        void add(Collider *object);
        void remove(Collider *object);

        void update(float fixedTimeStep);
        void debugDraw(bool showMesh, bool showSpheres);
    };
}