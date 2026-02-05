#include "GlobalSettings.h"
#include "GlobalSettingsInterface.h"
#include <mutex>

GlobalSettingsInterface *instance = nullptr;

GlobalSettingsInterface *getGlobalSettingsInterface()
{
    static std::once_flag initFlag;
    std::call_once(initFlag, []()
                   {
        if (!instance)
        {
            static GlobalSettings settings;
            instance = &settings;
        } });

    return instance;
}
