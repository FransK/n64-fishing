#pragma once

#include <any>
#include <cstddef>

class GlobalSettingsInterface
{
public:
    virtual ~GlobalSettingsInterface() = default;

    std::any getGlobalSettingValue(size_t key)
    {
        return getGlobalSetting(key);
    }

    void setGlobalSettingValue(size_t key, std::any value)
    {
        setGlobalSetting(key, value);
    }

    void clearGlobalSettingKey(size_t key)
    {
        clearGlobalSetting(key);
    }

private:
    virtual std::any getGlobalSetting(size_t key) = 0;
    virtual void setGlobalSetting(size_t key, std::any value) = 0;
    virtual void clearGlobalSetting(size_t key) = 0;
};

GlobalSettingsInterface *getGlobalSettingsInterface();
void setGlobalSettingsInterface(GlobalSettingsInterface *globalSettings);

extern GlobalSettingsInterface *globalSettingsInstance;