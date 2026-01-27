#include "GameSettings.h"
#include "GameSettingsInterface.h"
#include <mutex>

GameSettingsInterface *instance = nullptr;

GameSettingsInterface *get_game_settings_interface()
{
    static std::once_flag initFlag;
    std::call_once(initFlag, []()
                   {
        if (!instance)
        {
            static GameSettings settings;
            instance = &settings;
        } });

    return instance;
}

void set_game_settings_interface(GameSettingsInterface *game_settings)
{
    instance = game_settings;
}