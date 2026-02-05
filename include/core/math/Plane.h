#pragma once

#include <cmath>

#include "Vector3.h"

namespace Math
{
    struct Plane
    {
        Vector3 normal;
        float d;

        friend float distanceToPlane(const Plane &plane, const Vector3 &point)
        {
            return dot(plane.normal, point) + plane.d;
        }
    };

    Vector3 calculateBarycentricCoords(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &point);
    Vector3 evaluateBarycentricCoords(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &baryCoords);
    float calculateLerp(const Vector3 &a, const Vector3 &b, const Vector3 &point);
}
