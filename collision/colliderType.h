#pragma once

#include "../math/vector2.h"
#include "../math/box3d.h"
#include "colliderTypeData.h"
#include "gjk.h"

namespace Collision
{
    typedef void (*BoundingBoxCalculator)(ColliderTypeData *data, Vector2 *rotation, Box3D *box);

    struct ColliderType
    {
        GJK::MinkowskiSum minkowskiSum{};
        BoundingBoxCalculator boundingBoxCalculator{};
        ColliderTypeData data{};
        float bounce{};
        float friction{};
    };
}