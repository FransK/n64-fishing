#pragma once

#include <cstdint>
#include <istream>
#include <string>
#include <unordered_map>
#include <vector>

class JsonParser
{
public:
    bool parse(std::istream &jsonStream);
    std::string valueOf(std::string_view attribute) const;
    std::vector<std::string> arrayOf(std::string_view attribute) const;

private:
    using AttributeMap = std::unordered_map<std::string, std::string>;
    using ArrayMap = std::unordered_map<std::string, std::vector<std::string>>;

    std::int32_t _typeId{};
    AttributeMap _parsedAttributes{};
    ArrayMap _parsedArrays{};
};