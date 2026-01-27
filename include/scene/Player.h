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
    Player(Collision::CollisionScene *scene, PlayerData *data, PlayerState *state, int8_t playerNumber);
    ~Player();
    void drawBillboard(T3DViewport &viewport) const;

    Collision::Collider *getCollider() { return &mCollider; }
    Collision::Collider *getDamageTrigger() { return &mDamageTrigger; }

private:
    Collision::Collider mCollider{};
    Collision::Collider mDamageTrigger{};

    Collision::CollisionScene *mScene{};
    PlayerData *mPlayerData{};
    PlayerState *mPlayerState{};
};