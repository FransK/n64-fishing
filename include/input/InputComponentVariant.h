#pragma once

#include <variant>
#include "InputComponent.h"
#include "AIInputStrategy.h"
#include "PlayerInputStrategy.h"
#include "PlayerUpdateStrategy.h"

using InputComponentVariant = std::variant<
    InputComponent<PlayerInputStrategy, PlayerUpdateStrategy>,
    InputComponent<AIInputStrategy, PlayerUpdateStrategy>>;