#pragma once

#include "math/Box3d.h"
#include "math/Vector2.h"

#include "ColliderTypeData.h"

namespace Collision
{
    typedef Math::Box3D (*BoundingBoxCalculator)(const ColliderTypeData &data, const Math::Vector2 &rotation);
    typedef Math::Vector3 (*MinkowskiSum)(const ColliderTypeData &data, const Math::Vector3 &direction);

    struct ColliderType
    {
        MinkowskiSum minkowskiSum{};
        BoundingBoxCalculator boundingBoxCalculator{};
        ColliderTypeData data{};
        float bounce{};
        float friction{};
    };
}