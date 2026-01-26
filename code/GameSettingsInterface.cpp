#include "GameSettings.h"
#include "GameSettingsInterface.h"

GameSettingsInterface *instance = nullptr;

GameSettingsInterface *get_game_settings_interface()
{
    static bool init = []()
    {
        if (!instance)
        {
            static GameSettings settings;
            instance = &settings;
        }
        return true;
    }();

    return instance;
}

void set_game_settings_interface(GameSettingsInterface *game_settings)
{
    instance = game_settings;
}