#pragma once

#include <cstdint>
#include "collider.h"
#include "colliderTypeData.h"
#include "../math/vector3.h"

using namespace Math;

namespace Collision
{
    constexpr uint16_t MaxSimplexSize = 4;

    struct Simplex
    {
        Vector3 points[MaxSimplexSize];
        Vector3 objectAPoint[MaxSimplexSize];
        int16_t nPoints;

        Simplex(int16_t points = 0) : nPoints(points) {};
        int check(Vector3 *nextDirection);
    };

    struct GJK
    {
        typedef void (*MinkowskiSum)(ColliderTypeData *data, Vector3 *direction, Vector3 *output);

        static int checkForOverlap(Simplex *simplex, Collider *a, Collider *b, const Vector3 &firstDirection);
    };
}