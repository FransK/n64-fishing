#pragma once

#include <cstdint>
#include <memory>
#include "Scene.h"
#include "SceneLoadStrategy.h"

namespace Services
{
    class SceneLoader
    {
    public:
        SceneLoader(std::unique_ptr<SceneLoadStrategy> sceneLoadStrategy) : mLoadStrategy(std::move(sceneLoadStrategy)) {}

        std::unique_ptr<Scene> loadScene(int32_t sceneId) { debugf("SceneLoader::loadScene(%i) - Not implemented", sceneId); };

    private:
        std::unique_ptr<SceneLoadStrategy> mLoadStrategy{};
    };
}