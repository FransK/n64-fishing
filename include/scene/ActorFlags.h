#pragma once

#include <cstdint>

enum struct ActorFlags : uint32_t
{
    FLAG_NONE = 0,
    FLAG_IS_PLAYER = 1 << 0,
    FLAG_IS_STUNNED = 1 << 1,
};