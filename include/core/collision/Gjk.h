#pragma once

#include <cstdint>
#include <memory>

#include "math/Vector3.h"

#include "Collider.h"
#include "ColliderTypeData.h"

namespace Collision
{
    constexpr uint16_t MaxSimplexSize = 4;
    constexpr uint16_t MaxGJKIterations = 16;

    struct Simplex
    {
        Math::Vector3 points[MaxSimplexSize];
        Math::Vector3 objectAPoint[MaxSimplexSize];
        int16_t nPoints;

        Simplex() : nPoints(0) {}

        /* Add the point in the Minkowski difference space (A - B) */
        Math::Vector3 *addPoint(Math::Vector3 *aPoint, Math::Vector3 *bPoint);

        void movePoint(int to, int from);
        int check(Math::Vector3 *nextDirection);
    };

    class GJK
    {
    public:
        static int checkForOverlap(Simplex *simplex, const std::unique_ptr<Collider> &a, const std::unique_ptr<Collider> &b, const Math::Vector3 *firstDirection);
    };
}