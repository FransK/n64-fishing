#include "GameSettings.h"

size_t setPlayers(bool *enabledconts, std::array<Core::PlayerJoypad, JOYPAD_PORT_COUNT> &playerJoypads)
{
    size_t plynum = 0;

    // Attempt to find the first N left-most available controllers
    for (size_t i = 0; i < Core::MAX_PLAYERS; i++)
    {
        if (enabledconts[i])
        {
            playerJoypads[plynum].port = static_cast<joypad_port_t>(i);
            plynum++;
        }
    }
    return plynum;
}

void resetGameSettings(GlobalSettingsInterface *gameSettings)
{
    gameSettings->clearGlobalSettingKey(static_cast<size_t>(GameSettingsKeys::EXIT_GAME));
    gameSettings->clearGlobalSettingKey(static_cast<size_t>(GameSettingsKeys::WINNING_PLAYERS));
}