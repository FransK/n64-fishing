#include "services/JsonAssetDataStrategy.h"

#include "JsonParser.h"

#include <cstdlib>

namespace Services
{
    std::optional<AssetListData> JsonAssetDataStrategy::parseAssetList(std::istream &stream)
    {
        auto parser = JsonParser{};
        if (!parser.parse(stream))
        {
            return std::nullopt;
        }

        auto data = AssetListData{};
        data.scene = parser.valueOf("scene");
        data.assets = parser.arrayOf("assets");

        return data;
    }

    std::optional<PlayerData> JsonAssetDataStrategy::parsePlayerData(std::istream &stream)
    {
        auto parser = JsonParser{};
        if (!parser.parse(stream))
        {
            return std::nullopt;
        }

        auto data = PlayerData{};
        data.position = parseVector3(parser.valueOf("position"));
        data.rotation = parseVector2(parser.valueOf("rotation"));

        return data;
    }

    Math::Vector3 JsonAssetDataStrategy::parseVector3(std::string_view str)
    {
        auto result = Math::Vector3{0.0f, 0.0f, 0.0f};

        auto ptr = str.data();
        char *end = nullptr;

        for (auto i = 0; i < 3; ++i)
        {
            result[i] = std::strtof(ptr, &end);
            if (!end || *end != ',')
            {
                break;
            }
            ptr = end + 1;
        }

        return result;
    }

    Math::Vector2 JsonAssetDataStrategy::parseVector2(std::string_view str)
    {
        auto result = Math::Vector2{0.0f, 0.0f};

        auto ptr = str.data();
        char *end = nullptr;

        for (auto i = 0; i < 2; ++i)
        {
            result[i] = std::strtof(ptr, &end);
            if (!end || *end != ',')
            {
                break;
            }
            ptr = end + 1;
        }

        return result;
    }
}
