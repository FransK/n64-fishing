/**
 * @copyright 2024 - Max Bebök
 * @license MIT
 */
#pragma once

#include <t3d/t3dmath.h>
#include <vector>
#include <functional>

class Scene;

namespace Debug
{
    class Overlay
    {
    public:
        void draw(Scene &scene, uint32_t vertCount, float deltaTime);
    };
}