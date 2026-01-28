#pragma once

#include <libdragon.h>
#include "GlobalSettingsInterface.h"
#include "GlobalSettingTable.h"

class GlobalSettings : public GlobalSettingsInterface
{
public:
    GlobalSettings() : globalSettingsTable({}) {}

    GlobalSettings(const GlobalSettings &) = delete;
    GlobalSettings(GlobalSettings &&) = delete;
    GlobalSettings &operator=(const GlobalSettings &) = delete;
    GlobalSettings &operator=(GlobalSettings &&) = delete;

private:
    std::any getGlobalSetting(size_t key) override
    {
        auto it = globalSettingsTable.find(key);
        if (it != globalSettingsTable.end())
        {
            return it->second;
        }
        return std::any{};
    }

    void setGlobalSetting(size_t key, std::any value) override
    {
        globalSettingsTable[key] = std::move(value);
    }

    void clearGlobalSetting(size_t key) override
    {
        globalSettingsTable.erase(key);
    }

    Core::GlobalSettingTable globalSettingsTable;
};