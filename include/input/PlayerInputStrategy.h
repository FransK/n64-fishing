#pragma once

#include <libdragon.h>
#include "input/PlayerInputState.h"
#include <functional>

class PlayerInputStrategy
{
public:
    explicit PlayerInputStrategy(joypad_port_t port) : mPort(port) {}

    PlayerInputState inputState() const;

private:
    joypad_port_t mPort;
};