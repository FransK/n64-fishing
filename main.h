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

struct InputState
{
    T3DVec3 move{};
    bool fish{};
    bool attack{};
};