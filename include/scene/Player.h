#pragma once

#include <cstdint>
#include "animation/AnimationComponent.h"
#include "collision/CollisionScene.h"
#include "collision/Collider.h"
#include "input/PlayerData.h"
#include "input/PlayerState.h"

using namespace Math;

class Player
{
public:
    Player() = default;
    ~Player();

    void init(Collision::CollisionScene *scene, PlayerData *data, PlayerState *state, int8_t playerNumber);
    void draw_billboard(T3DViewport &viewport) const;

    Collision::Collider *get_collider() { return &mCollider; }
    Collision::Collider *get_damage_trigger() { return &mDamageTrigger; }

private:
    Collision::Collider mCollider{};
    Collision::Collider mDamageTrigger{};

    Collision::CollisionScene *mScene{};
    PlayerData *mPlayerData{};
    PlayerState *mPlayerState{};
};