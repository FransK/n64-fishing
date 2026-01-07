#include <algorithm>
#include <vector>

#include "../debug/debugDraw.h"
#include "../math/vector3.h"

#include "colliderEdge.h"
#include "gjk.h"
#include "scene.h"

using namespace Collision;
using namespace Math;

Scene::Scene() {}
Scene::~Scene() {}

void Scene::add(Collider *object)
{
    colliders.push_back(object);
}

void Scene::remove(Collider *object)
{
    for (auto iter = colliders.begin(); iter != colliders.end(); ++iter)
    {
        if (*iter == object)
        {
            return (void)colliders.erase(iter);
        }
    }
}

void Scene::update(float fixedTimeStep)
{
    /* Integrate objects */
    for (auto *c : colliders)
    {
        c->update(fixedTimeStep);
        c->recalcBB();
    }

    /* Solve collisions */
    runCollision();

    /* Clamp to world */
    for (auto *c : colliders)
    {
        c->constrainPosition();
    }
}

void Scene::debugDraw()
{
    for (auto *c : colliders)
    {
        Debug::drawBox(c->boundingBox);
    }
}

void Scene::runCollision()
{
    // === Sweep and Prune === //
    int edgeCount = colliders.size() * 2;
    std::vector<ColliderEdge> colliderEdges(edgeCount);
    ColliderEdge *currEdge = &colliderEdges[0];

    // Prune along x axis by looking at min and max x of each BB
    for (size_t i = 0; i < colliders.size(); i++)
    {
        currEdge->isStartEdge = 1;
        currEdge->objectIndex = i;
        currEdge->x = (short)(colliders[i]->boundingBox.min.x);

        currEdge += 1;

        currEdge->isStartEdge = 0;
        currEdge->objectIndex = i;
        currEdge->x = (short)(colliders[i]->boundingBox.max.x);

        currEdge += 1;
    }

    // Sort by x position of each edge
    std::sort(colliderEdges.begin(), colliderEdges.end());

    std::vector<uint16_t> activeObjects(colliders.size());
    int activeObjectCount = 0;

    for (int edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++)
    {
        ColliderEdge edge = colliderEdges[edgeIndex];

        if (edge.isStartEdge)
        {
            Collider *a = colliders[edge.objectIndex];

            for (int activeIndex = 0; activeIndex < activeObjectCount; activeIndex++)
            {
                Collider *b = colliders[activeObjects[activeIndex]];

                // === AABB === //
                if (Box3D::hasOverlap(a->boundingBox, b->boundingBox))
                {
                    // === Check Collider Shapes == //
                    debugf("Overlapping AABB: %d and %d\n", a->entityId, b->entityId);
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

void Scene::collide(Collider *a, Collider *b)
{
    if (!(a->collisionLayers & b->collisionLayers))
    {
        return;
    }

    if (a->collisionGroup && a->collisionGroup == b->collisionGroup)
    {
        return;
    }

    Simplex simplex{};
    if (!GJK::checkForOverlap(&simplex, a, b, &Vec3Right))
    {
        debugf("No colliders overlap: %d and %d\n", a->entityId, b->entityId);
        return;
    }
    debugf("Overlapping colliders: %d and %d\n", a->entityId, b->entityId);
}