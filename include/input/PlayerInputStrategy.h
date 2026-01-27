#pragma once

#include <libdragon.h>
#include "input/InputState.h"
#include <functional>

class PlayerInputStrategy
{
public:
    explicit PlayerInputStrategy(joypad_port_t port) : mPort(port) {}

    InputState update();

private:
    joypad_port_t mPort;
};