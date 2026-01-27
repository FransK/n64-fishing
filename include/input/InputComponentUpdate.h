#pragma once

#include "input/InputComponentVariant.h"

struct InputComponentUpdate
{
    struct args
    {
        float deltaTime;
        PlayerState &playerState;
        PlayerData &playerData;
        Collision::CollisionScene &collScene;
        Collision::Collider *damageTrigger;
        bool stunned;
    } args;

    InputComponentUpdate(float deltaTime,
                         PlayerState &playerState,
                         PlayerData &playerData,
                         Collision::CollisionScene &collScene,
                         Collision::Collider *damageTrigger,
                         bool stunned) : args{deltaTime, playerState, playerData, collScene, damageTrigger, stunned} {}

    template <typename InputComponentType>
    void operator()(InputComponentType &inputComponent)
    {
        inputComponent.update(args.deltaTime,
                              args.playerState,
                              args.playerData,
                              args.collScene,
                              args.damageTrigger,
                              args.stunned);
    }
};