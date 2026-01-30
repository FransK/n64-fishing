#pragma once

#include "Collider.h"

namespace Collision
{
    class Cylinder
    {
    public:
        static Box3D BoundingBox(const ColliderTypeData &data, const Vector2 &rotation);
        static Vector3 MinkowskiSum(const ColliderTypeData &data, const Vector3 &direction);
    };
}