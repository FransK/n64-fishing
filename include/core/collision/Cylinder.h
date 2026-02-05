#pragma once

#include "Collider.h"

namespace Collision
{
    class Cylinder
    {
    public:
        static Math::Box3D BoundingBox(const ColliderTypeData &data, const Math::Vector2 &rotation);
        static Math::Vector3 MinkowskiSum(const ColliderTypeData &data, const Math::Vector3 &direction);
    };
}