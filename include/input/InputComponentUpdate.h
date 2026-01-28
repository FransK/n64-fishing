#pragma once

#include "input/InputComponentVariant.h"

struct InputComponentUpdate
{
    struct args
    {
        float deltaTime;
        Player &player;
        Collision::CollisionScene &collScene;
    } args;

    InputComponentUpdate(float deltaTime,
                         Player &player,
                         Collision::CollisionScene &collScene) : args{deltaTime, player, collScene} {}

    template <typename InputComponentType>
    void operator()(InputComponentType &inputComponent)
    {
        inputComponent.update(args.deltaTime,
                              args.player,
                              args.collScene);
    }
};