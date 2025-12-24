#pragma once

#include "../math/vector2.h"
#include "../math/vector3.h"
#include "../math/box3d.h"
#include "gjk.h"

using namespace Math;

namespace Collision
{
    struct Collider
    {
        struct Type
        {
            typedef void (*BoundingBoxCalculator)(void *data, Vector2 *rotation, Box3D *box);

            union Data
            {
                struct
                {
                    float radius;
                } sphere;
                struct
                {
                    float radius;
                    float halfHeight;
                } cylinder;
            };

            GJK::MinkowskiSum minkowskiSum;
            BoundingBoxCalculator boundingBoxCalculator;
            Data *data;
            float bounce;
            float friction;
        };

        int entityId;
        Vector3 position;
        Vector2 rotation;
        Vector3 velocity;
        Vector3 center;
        Type *type;
        Box3D boundingBox;
    };
}