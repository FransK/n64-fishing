#pragma once

#include <functional>
#include <libdragon.h>
#include "input/PlayerInputState.h"
#include "scene/PlayerAi.h"

class AIInputStrategy
{
public:
    explicit AIInputStrategy(PlayerAi *ai) : mAi(ai) {}

    PlayerInputState getInputState();

private:
    PlayerAi *mAi;
};