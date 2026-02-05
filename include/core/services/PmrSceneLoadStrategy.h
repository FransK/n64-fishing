#pragma once

#include "SceneLoadStrategy.h"

namespace Services
{
    class PmrSceneLoadStrategy : public SceneLoadStrategy
    {
    public:
        std::unique_ptr<Scene> loadScene() override { debugf("PmrSceneLoadStrategy::loadScene() - Not Implemented"); };
    };
}