#include "GameSettings.h"
#include "GameSettingsInterface.h"
#include <mutex>

GameSettingsInterface *instance = nullptr;

GameSettingsInterface *getGameSettingsInterface()
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

void setGameSettingsInterface(GameSettingsInterface *game_settings)
{
    instance = game_settings;
}
