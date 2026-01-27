#pragma once

#include <functional>
#include <libdragon.h>
#include "input/InputState.h"
#include "scene/PlayerAi.h"

class AIInputStrategy
{
public:
    explicit AIInputStrategy(PlayerAi *ai) : mAi(ai) {}

    InputState update();

private:
    PlayerAi *mAi;
};