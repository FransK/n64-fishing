#pragma once

#include <memory>
#include "Scene.h"

namespace Services
{
    class SceneLoadStrategy
    {
    public:
        virtual ~SceneLoadStrategy() = default;

        virtual std::unique_ptr<Scene> loadScene() = 0;
    };
}