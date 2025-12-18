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
constexpr uint32_t TIMER_Y = 210;
constexpr uint32_t SCORE_X = 50;
constexpr uint32_t SCORE_Y = 220;
constexpr uint32_t SCORE_X_SPACING = (SCREEN_WIDTH - 2 * SCORE_X) / (MAXPLAYERS - 1);

constexpr int FONT_BILLBOARD = 1;
constexpr int FONT_TEXT = 2;

struct InputState
{
    T3DVec3 move{};
    bool fish{};
    bool attack{};
};