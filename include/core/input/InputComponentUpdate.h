#pragma once

#include "input/InputComponentVariant.h"

struct InputComponentUpdate
{
    float deltaTime;

    InputComponentUpdate(float deltaTime) : deltaTime{deltaTime} {}

    template <typename InputComponentType>
    void operator()(InputComponentType &inputComponent)
    {
        inputComponent.update(deltaTime);
    }
};