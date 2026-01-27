#pragma once

#include <cstdint>

#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Box3d.h"
#include "scene/Actor.h"

#include "ColliderType.h"

using namespace Math;

namespace Collision
{
    constexpr float GRAVITY_CONSTANT = -10.f * 64.f;
    constexpr float PLAYING_R = 60.f;
    constexpr float PLAYING_R2 = PLAYING_R * PLAYING_R;

    enum struct CollisionLayers
    {
        CollisionLayerTangible = (1 << 0),
    };

    struct Collider
    {
        int entityId{};
        Actor *actor{};
        Vector3 center{};
        ColliderType type{};
        Box3D boundingBox{};
        float scale{};
        bool hasGravity{true};
        bool isTrigger{false};
        bool isFixed{false};
        uint16_t collisionLayers{};
        uint16_t collisionGroup{};

        void update(float timeStep);
        void recalcBB();
        void constrainPosition();
        void minkowskiSumWorld(const Vector3 *direction, Vector3 *output);
    };
}