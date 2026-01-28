#pragma once

#include <cstdint>
#include <memory>
#include "animation/AnimationComponent.h"
#include "collision/CollisionScene.h"
#include "collision/Collider.h"
#include "Actor.h"
#include "PlayerState.h"

using namespace Math;

class Player : public Actor
{
public:
    Player(Collision::CollisionScene *scene, PlayerState *state, int8_t playerNumber);
    ~Player();
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;
    Player(Player &&) = default;
    Player &operator=(Player &&) = default;

    void drawBillboard(T3DViewport &viewport) const;

    const Collision::Collider *getCollider() const { return &mCollider; }
    Collision::Collider *getDamageTrigger() { return &mDamageTrigger; }
    PlayerState *getPlayerState() const { return mPlayerState; }
    Actor *getAttackActor() { return &mAttackActor; }
    int8_t getPlayerNumber() const { return mPlayerNumber; }

private:
    Actor mAttackActor{};
    Collision::Collider mCollider{};
    Collision::Collider mDamageTrigger{};
    Collision::CollisionScene *mScene{};
    PlayerState *mPlayerState{};
    int8_t mPlayerNumber{};
};