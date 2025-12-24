#include "scene.h"
#include "../math/vector3.h"

using namespace Collision;
using namespace Math;

void Scene::updateCollider(Collider *object, float fixedTimeStep)
{
    Vector3::addScaled(&object->position, &object->velocity, fixedTimeStep, &object->position);
}

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
        updateCollider(c, fixedTimeStep);
    }

    /* Solve collisions */

    /* Clamp to world */
}

void Scene::debugDraw(bool showMesh, bool showSpheres) {}