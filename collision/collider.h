#pragma once

#include "../math/vector2.h"
#include "../math/vector3.h"
#include "../math/box3d.h"
#include "colliderTypeData.h"
#include "gjk.h"

using namespace Math;

namespace Collision
{
    constexpr float PLAYING_R = 60.f;
    constexpr float PLAYING_R2 = PLAYING_R * PLAYING_R;

    typedef void (*BoundingBoxCalculator)(ColliderTypeData *data, Vector2 *rotation, Box3D *box);

    struct ColliderType
    {
        GJK::MinkowskiSum minkowskiSum{};
        BoundingBoxCalculator boundingBoxCalculator{};
        ColliderTypeData data{};
        float bounce{};
        float friction{};
    };

    struct Collider
    {
        int entityId{};
        Vector3 position{};
        Vector2 rotation{};
        Vector3 velocity{};
        Vector3 center{};
        ColliderType type{};
        Box3D boundingBox{};
        float scale{};

        void update(float timeStep);
        void recalcBB();
        void constrainPosition();
    };
}