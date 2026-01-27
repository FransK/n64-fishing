#pragma once

#include "math/Box3d.h"
#include "math/Vector2.h"

#include "ColliderTypeData.h"

using namespace Math;

namespace Collision
{
    typedef void (*BoundingBoxCalculator)(ColliderTypeData *data, Vector2 *rotation, Box3D *box);
    typedef void (*MinkowskiSum)(const ColliderTypeData *data, const Vector3 *direction, Vector3 *output);

    struct ColliderType
    {
        MinkowskiSum minkowskiSum{};
        BoundingBoxCalculator boundingBoxCalculator{};
        ColliderTypeData data{};
        float bounce{};
        float friction{};
    };
}