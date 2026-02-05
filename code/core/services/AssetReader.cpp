#include "services/AssetReader.h"

#include <cstdio>
#include <sstream>
#include <vector>

namespace Services
{
    AssetReader::AssetReader(std::unique_ptr<AssetDataStrategy> strategy)
        : mStrategy(std::move(strategy))
    {
    }

    std::optional<AssetListData> AssetReader::loadAssetList(const std::string &path)
    {
        FILE *file = fopen(path.c_str(), "r");
        if (!file)
        {
            return std::nullopt;
        }

        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);

        std::vector<char> buffer(size + 1);
        fread(buffer.data(), 1, size, file);
        buffer[size] = '\0';
        fclose(file);

        std::istringstream stream(buffer.data());
        return mStrategy->parseAssetList(stream);
    }

    std::optional<PlayerData> AssetReader::loadPlayerData(const std::string &path)
    {
        FILE *file = fopen(path.c_str(), "r");
        if (!file)
        {
            return std::nullopt;
        }

        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);

        std::vector<char> buffer(size + 1);
        fread(buffer.data(), 1, size, file);
        buffer[size] = '\0';
        fclose(file);

        std::istringstream stream(buffer.data());
        return mStrategy->parsePlayerData(stream);
    }
}
