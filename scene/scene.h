#pragma once

#include <t3d/t3d.h>
#include "../main.h"
#include "camera.h"
#include "player.h"

namespace FranSoft
{
    constexpr uint8_t colorAmbient[4] = {0xAA, 0xAA, 0xAA, 0xFF};
    constexpr uint8_t colorDir[4] = {0xFF, 0xAA, 0xAA, 0xFF};

    constexpr color_t colors[] = {
        PLAYERCOLOR_1,
        PLAYERCOLOR_2,
        PLAYERCOLOR_3,
        PLAYERCOLOR_4,
    };
}

class Scene
{
private:
    Player mPlayers[MAXPLAYERS];
    InputState mInputState[MAXPLAYERS];

    rdpq_font_t *mFontBillboard{};

    T3DViewport mViewport{};
    Camera mCamera{};
    T3DVec3 mLightDirVec{};

    void read_inputs(PlyNum plyNum);
    void process_attacks(PlyNum attacker);

public:
    void update_fixed(float deltaTime);
    void update(float deltaTime);

    Scene();
    ~Scene();
};