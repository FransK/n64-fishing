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

    enum struct ColliderFlags : uint32_t
    {
        IS_ATTACK_TRIGGER = (1 << 0),
    };

    struct Collider
    {
        int entityId{};
        Actor *actor{};  // Non-owning; lifetime managed by Player
        Vector3 center{};
        ColliderType type{};
        Box3D boundingBox{};
        float scale{};
        bool hasGravity{true};
        bool isTrigger{false};
        bool isFixed{false};
        uint16_t collisionLayers{};
        uint16_t collisionGroup{};
        uint32_t flags{};

        void update(float timeStep);
        void recalcBB();
        void constrainPosition();
        bool hasFlag(ColliderFlags flag) const { return (flags & static_cast<uint32_t>(flag)) != 0; }
        void setFlag(ColliderFlags flag) { flags |= static_cast<uint32_t>(flag); }
        Vector3 minkowskiSumWorld(const Vector3 &direction);
    };
}