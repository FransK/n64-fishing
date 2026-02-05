#include "CollisionScene.h"

#include <algorithm>
#include "containers/Vector.h"

#include "debug/DebugDraw.h"
#include "math/Vector3.h"
#include "scene/Player.h"

#include "ActorFlags.h"
#include "Player.h"
#include "ColliderEdge.h"
#include "Epa.h"
#include "Gjk.h"

using namespace Collision;
using namespace Math;

void CollisionScene::add(std::unique_ptr<Collider> object, bool isActive)
{
    if (isActive)
    {
        mActiveColliders.push_back(std::move(object));
    }
    else
    {
        mInactiveColliders.push_back(std::move(object));
    }
}

void CollisionScene::remove(int entityId)
{
    for (auto iter = mActiveColliders.begin(); iter != mActiveColliders.end(); ++iter)
    {
        if ((*iter)->entityId == entityId)
        {
            mActiveColliders.erase(iter);
            return;
        }
    }
    for (auto iter = mInactiveColliders.begin(); iter != mInactiveColliders.end(); ++iter)
    {
        if ((*iter)->entityId == entityId)
        {
            mInactiveColliders.erase(iter);
            return;
        }
    }
}

void CollisionScene::activate(int entityId)
{
    auto it = std::find_if(mInactiveColliders.begin(), mInactiveColliders.end(),
                           [entityId](const auto &c)
                           { return c->entityId == entityId; });

    if (it != mInactiveColliders.end())
    {
        mActiveColliders.push_back(std::move(*it));
        mInactiveColliders.erase(it);
    }
}

void CollisionScene::deactivate(int entityId)
{
    auto it = std::find_if(mActiveColliders.begin(), mActiveColliders.end(),
                           [entityId](const auto &c)
                           { return c->entityId == entityId; });

    if (it != mActiveColliders.end())
    {
        mInactiveColliders.push_back(std::move(*it));
        mActiveColliders.erase(it);
    }
}

void CollisionScene::update(float fixedTimeStep)
{
    /* Integrate objects */
    for (const auto &c : mActiveColliders)
    {
        c->update(fixedTimeStep);
        c->recalcBB();
    }

    /* Solve collisions */
    runCollision();

    /* Clamp to world */
    for (const auto &c : mActiveColliders)
    {
        constrainToWorld(c);
        c->constrainPosition();
    }
}

void CollisionScene::debugDraw(const Debug::DebugDraw &drawer) const
{
    for (const auto &c : mActiveColliders)
    {
        drawer.drawBox(c->boundingBox);
    }
}

void CollisionScene::runCollision()
{
    // === Sweep and Prune === //
    int edgeCount = mActiveColliders.size() * 2;
    Containers::vector<ColliderEdge> colliderEdges(edgeCount);
    ColliderEdge *currEdge = &colliderEdges[0];

    // Prune along x axis by looking at min and max x of each BB
    for (size_t i = 0; i < mActiveColliders.size(); i++)
    {
        currEdge->isStartEdge = 1;
        currEdge->objectIndex = i;
        currEdge->x = (short)(mActiveColliders[i]->boundingBox.min.x);

        currEdge += 1;

        currEdge->isStartEdge = 0;
        currEdge->objectIndex = i;
        currEdge->x = (short)(mActiveColliders[i]->boundingBox.max.x);

        currEdge += 1;
    }

    // Sort by x position of each edge
    std::sort(colliderEdges.begin(), colliderEdges.end());

    Containers::vector<uint16_t> activeObjects(mActiveColliders.size());
    int activeObjectCount = 0;

    for (int edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++)
    {
        ColliderEdge edge = colliderEdges[edgeIndex];

        if (edge.isStartEdge)
        {
            const std::unique_ptr<Collider> &a = mActiveColliders[edge.objectIndex];
            for (int activeIndex = 0; activeIndex < activeObjectCount; activeIndex++)
            {
                const std::unique_ptr<Collider> &b = mActiveColliders[activeObjects[activeIndex]];

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

void CollisionScene::collide(const std::unique_ptr<Collider> &a, const std::unique_ptr<Collider> &b)
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

void CollisionScene::correctOverlap(const std::unique_ptr<Collider> &object, EpaResult *result, float ratio, float friction, float bounce)
{
    Vector3 position = object->actor->getPosition();
    position += result->normal * result->penetration * ratio;
    object->actor->setPosition(position);

    correctVelocity(object, result, ratio, friction, bounce);
}

void CollisionScene::correctVelocity(const std::unique_ptr<Collider> &object, EpaResult *result, float ratio, float friction, float bounce)
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

void CollisionScene::constrainToWorld(const std::unique_ptr<Collider> &object)
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

void CollisionScene::onPlayerStateChange(const Player &player)
{
    switch (player.lastActionState())
    {
    case PlayerStateEnum::STATE_ATTACKING:
        deactivate(player.attackActorEntityId());
        break;
    case PlayerStateEnum::STATE_IDLE:
    case PlayerStateEnum::STATE_WALKING:
    case PlayerStateEnum::STATE_STUNNED:
    case PlayerStateEnum::STATE_CASTING:
    case PlayerStateEnum::STATE_FISHING:
        break;
    }

    switch (player.actionState())
    {
    case PlayerStateEnum::STATE_ATTACKING:
        activate(player.attackActorEntityId());
        break;
    case PlayerStateEnum::STATE_IDLE:
    case PlayerStateEnum::STATE_WALKING:
    case PlayerStateEnum::STATE_STUNNED:
    case PlayerStateEnum::STATE_CASTING:
    case PlayerStateEnum::STATE_FISHING:
        break;
    }
}