#pragma once
#include <cmath>

namespace Math
{
    struct Vector3
    {
        float x, y, z;

        static void add(Vector3 *a, Vector3 *b, Vector3 *out)
        {
            out->x = a->x + b->x;
            out->y = a->y + b->y;
            out->z = a->z + b->z;
        }

        static void addScaled(Vector3 *a, Vector3 *normal, float scale, Vector3 *out)
        {
            out->x = a->x + normal->x * scale;
            out->y = a->y + normal->y * scale;
            out->z = a->z + normal->z * scale;
        }

        static void normAndScale(Vector3 *in, Vector3 *out, float scale)
        {
            const float x = in->x;
            const float y = in->y;
            const float z = in->z;

            const float len2 = x * x + y * y + z * z;

            if (len2 <= 1e-20f)
            {
                out->x = 0.0f;
                out->y = 0.0f;
                out->z = 0.0f;
                return;
            }

            const float invLen = 1.0f / sqrtf(len2);
            const float s = scale * invLen;

            out->x = x * s;
            out->y = y * s;
            out->z = z * s;
        }

        static void scale(Vector3 *in, Vector3 *out, float scale)
        {
            out->x = in->x * scale;
            out->y = in->y * scale;
            out->z = in->z * scale;
        }
    };
}