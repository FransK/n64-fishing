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