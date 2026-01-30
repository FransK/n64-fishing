#include "Plane.h"

using namespace Math;

Vector3 Math::calculateBarycentricCoords(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &point)
{
    Vector3 output;

    Vector3 v0 = b - a;
    Vector3 v1 = c - a;
    Vector3 v2 = point - a;

    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;

    if (fabsf(denom) < 0.000001f)
    {
        if (d00 > d11)
        {
            // b is other point
            output.y = calculateLerp(a, b, point);
            output.x = 1.0f - output.y;
            output.z = 0.0f;
        }
        else
        {
            // c is other point
            output.z = calculateLerp(a, c, point);
            output.x = 1.0f - output.z;
            output.z = 0.0f;
        }
        return output;
    }

    float denomInv = 1.0f / (d00 * d11 - d01 * d01);
    output.y = (d11 * d20 - d01 * d21) * denomInv;
    output.z = (d00 * d21 - d01 * d20) * denomInv;
    output.x = 1.0f - output.y - output.z;

    return output;
}

Vector3 Math::evaluateBarycentricCoords(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &bary)
{
    Vector3 output = a * bary.x;
    output += b * bary.y;
    output += c * bary.z;
    return output;
}

float Math::calculateLerp(const Vector3 &a, const Vector3 &b, const Vector3 &point)
{
    Vector3 v0 = b - a;

    float denom = v0.magSqrd();

    if (denom < 0.00000001f)
    {
        return 0.5f;
    }

    Vector3 pointOffset = point - a;

    return dot(pointOffset, v0) / denom;
}
