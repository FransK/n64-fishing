#pragma once

#include "AssetDataStrategy.h"

namespace Services
{
    class JsonAssetDataStrategy : public AssetDataStrategy
    {
    public:
        std::optional<AssetListData> parseAssetList(std::istream &stream) override;
        std::optional<PlayerData> parsePlayerData(std::istream &stream) override;

    private:
        static Math::Vector3 parseVector3(std::string_view str);
        static Math::Vector2 parseVector2(std::string_view str);
    };
}
