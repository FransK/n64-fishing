#pragma once

#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>
#include "math/vector2.h"

extern "C"
{
#include "../../core.h"
#include "../../minigame.h"
}

#define FS_BASE_PATH "rom:/n64-fishing/"

using namespace Math;

constexpr uint32_t SCREEN_WIDTH = 320;
constexpr uint32_t SCREEN_HEIGHT = 240;
constexpr uint32_t TIMER_Y = 210;
constexpr uint32_t SCORE_X = 50;
constexpr uint32_t SCORE_Y = 220;
constexpr uint32_t SCORE_X_SPACING = (SCREEN_WIDTH - 2 * SCORE_X) / (MAXPLAYERS - 1);

constexpr int FONT_BILLBOARD = 1;
constexpr int FONT_TEXT = 2;

static int NextEntityId = 1;

static int GetNextEntityId()
{
    return NextEntityId++;
}

struct InputState
{
    Vector2 move{};
    bool fish{};
    bool attack{};
};