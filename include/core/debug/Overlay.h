/**
 * @copyright 2024 - Max Bebök
 * @license MIT
 */
#pragma once

#include <cstdint>
#include "debug/DebugDraw.h"

class Scene;

namespace Debug
{
    class Overlay
    {
    public:
        void draw(Scene &scene, uint32_t vertCount, float deltaTime, const DebugDraw &drawer) const;
    };
}