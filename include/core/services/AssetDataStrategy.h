#pragma once

#include "AssetData.h"

#include <istream>
#include <optional>

namespace Services
{
    class AssetDataStrategy
    {
    public:
        virtual ~AssetDataStrategy() = default;

        virtual std::optional<AssetListData> parseAssetList(std::istream &stream) = 0;
        virtual std::optional<PlayerData> parsePlayerData(std::istream &stream) = 0;
    };
}
