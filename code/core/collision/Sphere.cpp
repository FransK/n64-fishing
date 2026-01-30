#include <cmath>

#include "Sphere.h"

using namespace Collision;

Vector3 Sphere::MinkowskiSum(const ColliderTypeData &data, const Vector3 &direction)
{
    Vector3 output;
    float radius = data.sphere.radius;

    float distance = fabsf(direction.x);
    output.x = direction.x > 0.0f ? radius : -radius;
    output.y = 0.0f;
    output.z = 0.0f;

    for (int i = 1; i < 3; ++i)
    {
        float distanceCheck = fabsf(direction[i]);

        if (distanceCheck > distance)
        {
            distance = distanceCheck;
            output = Vec3Zero;
            if (direction[i] > 0.0f)
            {
                output[i] = radius;
            }
            else
            {
                output[i] = -radius;
            }
        }
    }
    return output;
}

Box3D Sphere::BoundingBox(const ColliderTypeData &data, const Vector2 &rotation)
{
    Box3D box;
    box.max = Vec3One * data.sphere.radius;
    box.min = Vec3One * -data.sphere.radius;
    return box;
}