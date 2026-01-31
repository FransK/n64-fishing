#pragma once

#include <cstdint>

enum struct ActorFlags : uint32_t
{
    NONE = 0,
    IS_PLAYER = 1 << 0,
    IS_STUNNED = 1 << 1,
};