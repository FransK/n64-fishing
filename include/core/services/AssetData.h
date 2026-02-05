#pragma once

#include "math/Vector2.h"
#include "math/Vector3.h"

#include <string>
#include <vector>

namespace Services
{
    struct PlayerData
    {
        Math::Vector3 position;
        Math::Vector2 rotation;
    };

    struct AssetListData
    {
        std::string scene;
        std::vector<std::string> assets;
    };
}
