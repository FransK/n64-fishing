#pragma once

#include <any>
#include <unordered_map>

namespace Core
{
    using GlobalSettingTable = std::unordered_map<size_t, std::any>;
}