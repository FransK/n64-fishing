#pragma once

namespace Math
{
    struct Vector3
    {
        float x, y, z;

        static void addScaled(Vector3 *a, Vector3 *normal, float scale, Vector3 *out)
        {
            out->x = a->x + normal->x * scale;
            out->y = a->y + normal->y * scale;
            out->z = a->z + normal->z * scale;
        }
    };
}