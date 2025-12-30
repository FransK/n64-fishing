#include "scene.h"
#include "../debug/debugDraw.h"
#include "../math/vector3.h"

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
    for (size_t i = 0; i < colliders.size(); i++)
    {
        for (size_t j = 0; j < colliders.size(); j++)
        {
            if (i == j)
            {
                continue;
            }

            if (Box3D::hasOverlap(colliders[i]->boundingBox, colliders[j]->boundingBox))
            {
                debugf("Overlapping colliders: %d and %d\n", colliders[i]->entityId, colliders[j]->entityId);
            }
        }
    }

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