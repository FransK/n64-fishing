#pragma once

#include <array>
#include <cstdint>

#include "Config.h"
#include "GlobalSettingsInterface.h"

enum class GameSettingsKeys
{
    PLAYER_COUNT,
    PLAYER_JOYPADS,
    AI_DIFFICULTY,
    SUBTICK,
    EXIT_GAME,
    WINNING_PLAYERS
};

size_t setPlayers(bool *enabledconts, std::array<Core::PlayerJoypad, JOYPAD_PORT_COUNT> &playerJoypads);

void resetGameSettings(GlobalSettingsInterface *gameSettings);