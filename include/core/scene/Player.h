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
    Player(Collision::CollisionScene *scene, int8_t playerNumber);
    ~Player();
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;
    Player(Player &&) = default;
    Player &operator=(Player &&) = default;

    void drawBillboard(T3DViewport &viewport) const;
    void reset(Vector3 const &position, Vector2 const &rotation);

    PlayerState *getPlayerState() const { return mPlayerState.get(); }
    Actor *getAttackActor() const { return mAttackActor.get(); }
    int8_t getPlayerNumber() const { return mPlayerNumber; }

private:
    std::unique_ptr<Actor> mAttackActor{};
    std::unique_ptr<PlayerState> mPlayerState{};
    std::shared_ptr<Collision::CollisionScene> mScene{};
    int8_t mPlayerNumber{};
};