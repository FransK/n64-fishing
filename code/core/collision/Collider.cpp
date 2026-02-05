#include "Collider.h"

using namespace Collision;
using namespace Math;

void Collider::update(float timeStep)
{
    Vector3 position = actor->getPosition();
    Vector3 velocity = actor->getVelocity();

    if (hasGravity)
    {
        velocity.y += timeStep * GRAVITY_CONSTANT;
        actor->setVelocity(velocity);
    }

    position += velocity * timeStep;
    actor->setPosition(position);
}

void Collider::recalcBB()
{
    Vector3 position = actor->getPosition();
    Vector2 rotation = actor->getRotation();
    boundingBox = type.boundingBoxCalculator(type.data, rotation);

    Vector3 offset;
    if (scale != 1.0f)
    {
        boundingBox.min *= scale;
        boundingBox.max *= scale;
        offset = position + center * scale;
    }
    else
    {
        offset = position + center;
    }

    boundingBox.min += offset;
    boundingBox.max += offset;
}

void Collider::constrainPosition()
{
    Vector3 position = actor->getPosition();
    float squared_position = position.x * position.x + position.z * position.z;

    if (squared_position > PLAYING_R2)
    {
        position = normAndScale(position, PLAYING_R);
        actor->setPosition(position);
    }
}

Vector3 Collider::minkowskiSumWorld(const Vector3 &direction)
{
    Vector3 output = type.minkowskiSum(type.data, direction);

    if (scale != 1.0f)
    {
        output *= scale;
        output += center * scale;
    }
    else
    {
        output += center;
    }

    Vector3 position = actor->getPosition();
    output += position;

    return output;
}