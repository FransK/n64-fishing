#pragma once

#include "scene/Scene.h"
#include <cstdint>
#include <memory>

namespace
{
    constexpr int32_t INVALID_SCENE_ID = -1;
}

class World
{
public:
    World();
    ~World();
    World(const World &other) = delete;
    World &operator=(const World &other) = delete;

    void fixedloop(float deltatime);
    void loop(float deltatime);
    void reset();

    // Use mNextSceneId to locate and load the scene
    void loadNextScene() { debugf("World::loadNextScene - Not implemented"); };

    // Free all the memory associated with the current scene
    void unloadCurrentScene() { debugf("World::unloadCurrentScene - Not implemented"); };

private:
    std::unique_ptr<Scene> mScene{nullptr};
    int32_t mNextSceneId{INVALID_SCENE_ID};
};