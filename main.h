#pragma once

#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>

#include <rspq_constants.h>
#if RSPQ_PROFILE
#include <rspq_profile.h>
#endif

extern "C"
{
#include "../../core.h"
#include "../../minigame.h"
}

struct InputState
{
    T3DVec3 newDir{};
    float speed{};
};