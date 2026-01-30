#pragma once

#include "Vector3.h"

namespace Math
{
    struct Box3D
    {
        Vector3 min;
        Vector3 max;

        friend bool hasOverlap(const Box3D &a, const Box3D &b)
        {
            return a.min.x <= b.max.x && a.max.x >= b.min.x &&
                   a.min.y <= b.max.y && a.max.y >= b.min.y &&
                   a.min.z <= b.max.z && a.max.z >= b.min.z;
        }

        friend bool contains(const Box3D &box, const Vector3 &point)
        {
            return point.x >= box.min.x && point.x <= box.max.x &&
                   point.y >= box.min.y && point.y <= box.max.y &&
                   point.z >= box.min.z && point.z <= box.max.z;
        }

        friend Vector3 center(const Box3D &box)
        {
            Vector3 result;
            result.x = (box.min.x + box.max.x) * 0.5f;
            result.y = (box.min.y + box.max.y) * 0.5f;
            result.z = (box.min.z + box.max.z) * 0.5f;
            return result;
        }

        friend Vector3 extents(const Box3D &box)
        {
            Vector3 result;
            result.x = (box.max.x - box.min.x) * 0.5f;
            result.y = (box.max.y - box.min.y) * 0.5f;
            result.z = (box.max.z - box.min.z) * 0.5f;
            return result;
        }
    };
}
