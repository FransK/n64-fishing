#pragma once

#include <t3d/t3d.h>
#include "../main.h"
#include "camera.h"
#include "player.h"

namespace FranSoft
{
    constexpr uint8_t colorAmbient[4] = {0xAA, 0xAA, 0xAA, 0xFF};
    constexpr uint8_t colorDir[4] = {0xFF, 0xAA, 0xAA, 0xFF};
}

class Scene
{
private:
    T3DViewport mViewport{};
    Camera mCamera{};
    T3DVec3 mLightDirVec{};
    Player mPlayer{};
    InputState mInputState{};

    void read_inputs(PlyNum plyNum);

public:
    void update_fixed(float deltaTime);
    void update(float deltaTime);

    Scene();
    ~Scene() = default;
};