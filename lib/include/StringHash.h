#pragma once

#include <cstdint>
#include <string_view>

class StringHash
{
public:
    static std::int32_t hash(std::string_view str);
};
