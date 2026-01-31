#include "CollisionScene.h"

#include <algorithm>
#include <vector>

#include "debug/DebugDraw.h"
#include "math/Vector3.h"

#include "ActorFlags.h"
#include "Player.h"
#include "ColliderEdge.h"
#include "Epa.h"
#include "Gjk.h"

using namespace Collision;
using namespace Math;

void CollisionScene::add(Collider &&object, bool isActive)
{
    auto objectPtr = std::make_shared<Collider>(std::move(object));
    colliders.push_back(objectPtr);
    if (isActive)
    {
        activeColliders.push_back(objectPtr);
    }
}

void CollisionScene::remove(int entityId)
{
    for (auto iter = activeColliders.begin(); iter != activeColliders.end(); ++iter)
    {
        if ((*iter)->entityId == entityId)
        {
            activeColliders.erase(iter);
            break;
        }
    }
    for (auto iter = colliders.begin(); iter != colliders.end(); ++iter)
    {
        if ((*iter)->entityId == entityId)
        {
            colliders.erase(iter);
            return;
        }
    }
}

void CollisionScene::activate(int entityId)
{
    for (auto c : activeColliders)
    {
        if (c->entityId == entityId)
        {
            return;
        }
    }

    for (auto c : colliders)
    {
        if (c->entityId == entityId)
        {
            activeColliders.push_back(c);
            return;
        }
    }
}

void CollisionScene::deactivate(int entityId)
{
    for (auto iter = activeColliders.begin(); iter != activeColliders.end(); ++iter)
    {
        if ((*iter)->entityId == entityId)
        {
            activeColliders.erase(iter);
            return;
        }
    }
}

void CollisionScene::update(float fixedTimeStep)
{
    /* Integrate objects */
    for (auto c : activeColliders)
    {
        c->update(fixedTimeStep);
        c->recalcBB();
    }

    /* Solve collisions */
    runCollision();

    /* Clamp to world */
    for (auto c : activeColliders)
    {
        constrainToWorld(c.get());
        c->constrainPosition();
    }
}

void CollisionScene::debugDraw()
{
    for (auto c : activeColliders)
    {
        Debug::drawBox(c->boundingBox);
    }
}

void CollisionScene::runCollision()
{
    // === Sweep and Prune === //
    int edgeCount = activeColliders.size() * 2;
    std::vector<ColliderEdge> colliderEdges(edgeCount);
    ColliderEdge *currEdge = &colliderEdges[0];

    // Prune along x axis by looking at min and max x of each BB
    for (size_t i = 0; i < activeColliders.size(); i++)
    {
        currEdge->isStartEdge = 1;
        currEdge->objectIndex = i;
        currEdge->x = (short)(activeColliders[i]->boundingBox.min.x);

        currEdge += 1;

        currEdge->isStartEdge = 0;
        currEdge->objectIndex = i;
        currEdge->x = (short)(activeColliders[i]->boundingBox.max.x);

        currEdge += 1;
    }

    // Sort by x position of each edge
    std::sort(colliderEdges.begin(), colliderEdges.end());

    std::vector<uint16_t> activeObjects(activeColliders.size());
    int activeObjectCount = 0;

    for (int edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++)
    {
        ColliderEdge edge = colliderEdges[edgeIndex];

        if (edge.isStartEdge)
        {
            Collider *a = activeColliders[edge.objectIndex].get();
            for (int activeIndex = 0; activeIndex < activeObjectCount; activeIndex++)
            {
                Collider *b = activeColliders[activeObjects[activeIndex]].get();

                // === AABB === //
                if (hasOverlap(a->boundingBox, b->boundingBox))
                {
                    // === Check Collider Shapes == //
                    collide(a, b);
                }
            }

            activeObjects[activeObjectCount] = edge.objectIndex;
            activeObjectCount++;
        }
        else
        {
            // End edge, remove active object
            int foundIndex = -1;

            for (int activeIndex = 0; activeIndex < activeObjectCount; activeIndex++)
            {
                if (activeObjects[activeIndex] == edge.objectIndex)
                {
                    foundIndex = activeIndex;
                    break;
                }
            }

            assert(foundIndex != -1);

            // Remove item by replacing with the last one
            activeObjects[foundIndex] = activeObjects[activeObjectCount - 1];
            activeObjectCount--;
        }
    }
}

void CollisionScene::collide(Collider *a, Collider *b)
{
    if (!(a->collisionLayers & b->collisionLayers))
    {
        return;
    }

    if (a->collisionGroup && a->collisionGroup == b->collisionGroup)
    {
        return;
    }

    if (a->isTrigger && b->isTrigger)
    {
        return;
    }

    Simplex simplex{};
    if (!GJK::checkForOverlap(&simplex, a, b, &Vec3Right))
    {
        return;
    }

    if (a->isTrigger || b->isTrigger)
    {
        for (int i = 0; i < 4; i++)
        {
            if (a->isTrigger &&
                a->hasFlag(ColliderFlags::IS_ATTACK_TRIGGER) &&
                b->actor->hasFlag(ActorFlags::IS_PLAYER))
            {
                b->actor->setFlag(ActorFlags::IS_STUNNED);
                break;
            }
            if (b->isTrigger &&
                b->hasFlag(ColliderFlags::IS_ATTACK_TRIGGER) &&
                a->actor->hasFlag(ActorFlags::IS_PLAYER))
            {
                a->actor->setFlag(ActorFlags::IS_STUNNED);
                break;
            }
        }
        return;
    }

    EpaResult result;
    if (!EpaResult::solve(&simplex, a, b, &result))
    {
        return;
    }

    float friction = a->type.friction < b->type.friction ? a->type.friction : b->type.friction;
    float bounce = a->type.bounce > b->type.bounce ? a->type.bounce : b->type.bounce;

    correctOverlap(b, &result, -0.6f, friction, bounce);
    correctOverlap(a, &result, 0.6f, friction, bounce);
}

void CollisionScene::correctOverlap(Collider *object, EpaResult *result, float ratio, float friction, float bounce)
{
    Vector3 position = object->actor->getPosition();
    position += result->normal * result->penetration * ratio;
    object->actor->setPosition(position);

    correctVelocity(object, result, ratio, friction, bounce);
}

void CollisionScene::correctVelocity(Collider *object, EpaResult *result, float ratio, float friction, float bounce)
{
    Vector3 velocity = object->actor->getVelocity();
    float velocityDot = dot(velocity, result->normal);

    if ((velocityDot < 0) == (ratio < 0))
    {
        Vector3 tangentVelocity = velocity + result->normal * -velocityDot;
        tangentVelocity *= (1.0f - friction);
        velocity = tangentVelocity + result->normal * velocityDot * -bounce;
        object->actor->setVelocity(velocity);
    }
}

void CollisionScene::constrainToWorld(Collider *object)
{
    if (object->isTrigger)
    {
        return;
    }

    if (object->actor->getPosition().y < 0.0f)
    {
        EpaResult result;

        Vector3 position = object->actor->getPosition();
        result.contactA = position;
        result.contactB = position;
        result.contactB.y = 0.0f;
        result.normal = Vec3Up;
        result.penetration = position.y;
        correctOverlap(object, &result, -1.0f, object->type.friction, object->type.bounce);
    }
}