#pragma once

#include "AssetData.h"
#include "AssetDataStrategy.h"

#include <memory>
#include <optional>
#include <string>

namespace Services
{
    class AssetReader
    {
    public:
        explicit AssetReader(std::unique_ptr<AssetDataStrategy> strategy);

        std::optional<AssetListData> loadAssetList(const std::string &path);
        std::optional<PlayerData> loadPlayerData(const std::string &path);

    private:
        std::unique_ptr<AssetDataStrategy> mStrategy;
    };
}
