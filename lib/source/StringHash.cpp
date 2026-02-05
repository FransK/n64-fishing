#include "StringHash.h"

std::int32_t StringHash::hash(std::string_view str)
{
    std::int32_t hash = 5381;
    for (char c : str)
    {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }
    return hash;
}
