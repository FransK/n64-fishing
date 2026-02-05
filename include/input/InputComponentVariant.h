#pragma once

#include <variant>
#include "InputComponent.h"
#include "AIInputStrategy.h"
#include "PlayerInputStrategy.h"
#include "Player.h"

using InputComponentVariant = std::variant<
    InputComponent<PlayerInputStrategy>,
    InputComponent<AIInputStrategy>>;