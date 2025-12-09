#pragma once

#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>

extern "C"
{
#include "../../core.h"
#include "../../minigame.h"
}

constexpr uint32_t SCREEN_WIDTH = 320;
constexpr uint32_t SCREEN_HEIGHT = 240;

constexpr int FONT_BILLBOARD = 1;

struct InputState
{
    T3DVec3 move{};
    bool fish{};
    bool attack{};
};