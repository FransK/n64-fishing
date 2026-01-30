#include "Vector3.h"
#include <cmath>

namespace Math
{
    Vector3 normalize(const Vector3 &in)
    {
        const float denom = in.x * in.x + in.y * in.y + in.z * in.z;

        if (denom == 0.0f)
        {
            return Vec3Zero;
        }

        float invSqrt = 1.0f / sqrtf(denom);
        return in * invSqrt;
    }

    Vector3 perp(const Vector3 &in)
    {
        if (fabsf(in.x) > fabsf(in.z))
        {
            return cross(in, Vec3Forward);
        }

        return cross(in, Vec3Right);
    }
}