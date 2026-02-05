#pragma once

#include <variant>
#include "InputComponent.h"
#include "PlayerInputStrategy.h"
#include "scene/PlayerAi.h"

using InputComponentVariant = std::variant<
    InputComponent<PlayerInputStrategy>,
    InputComponent<PlayerAi *>>;