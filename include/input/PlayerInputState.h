#pragma once

#include "math/Vector2.h"

using namespace Math;

constexpr float MIN_MOVE_INPUT = 8.f;
constexpr float ROTATION_INPUT = 26.f;

struct PlayerInputState
{
    Vector2 move{};
    bool fish{};
    bool attack{};
};