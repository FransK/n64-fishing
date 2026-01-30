#pragma once

#include "math/Box3d.h"
#include "math/Vector2.h"

#include "ColliderTypeData.h"

using namespace Math;

namespace Collision
{
    typedef Box3D (*BoundingBoxCalculator)(const ColliderTypeData &data, const Vector2 &rotation);
    typedef Vector3 (*MinkowskiSum)(const ColliderTypeData &data, const Vector3 &direction);

    struct ColliderType
    {
        MinkowskiSum minkowskiSum{};
        BoundingBoxCalculator boundingBoxCalculator{};
        ColliderTypeData data{};
        float bounce{};
        float friction{};
    };
}