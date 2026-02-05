#include "services/AssetReader.h"

#include <fstream>
#include <sstream>

namespace Services
{
    AssetReader::AssetReader(std::unique_ptr<AssetDataStrategy> strategy)
        : mStrategy(std::move(strategy))
    {
    }

    std::optional<AssetListData> AssetReader::loadAssetList(const std::string &path)
    {
        std::ifstream file(path);
        if (!file)
        {
            return std::nullopt;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return mStrategy->parseAssetList(buffer);
    }

    std::optional<PlayerData> AssetReader::loadPlayerData(const std::string &path)
    {
        std::ifstream file(path);
        if (!file)
        {
            return std::nullopt;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return mStrategy->parsePlayerData(buffer);
    }
}
