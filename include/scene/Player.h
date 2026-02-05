#pragma once

#include <cstdint>
#include <memory>

#include <t3d/t3d.h>

#include "collision/CollisionScene.h"
#include "collision/Collider.h"
#include "input/PlayerInputState.h"
#include "scene/Actor.h"
#include "scene/PlayerState.h"

using namespace Math;

class Player : public Actor
{
public:
    Player(std::shared_ptr<Collision::CollisionScene> scene, int8_t playerNumber); // TODO remove shared (player shouldn't own collision scene)
    ~Player();
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;
    Player(Player &&) = default;
    Player &operator=(Player &&) = default;

    void drawBillboard(T3DViewport &viewport) const;
    void reset(Vector3 const &position, Vector2 const &rotation);
    void update(const PlayerInputState &inputState, float deltaTime);

    Actor *getAttackActor() const { return mAttackActor.get(); }
    PlayerState *getPlayerState() const { return mPlayerState.get(); }
    Collision::CollisionScene *getScene() const { return mScene.get(); }
    int8_t getPlayerNumber() const { return mPlayerNumber; }

private:
    std::unique_ptr<Actor> mAttackActor{};
    std::unique_ptr<PlayerState> mPlayerState{};
    std::shared_ptr<Collision::CollisionScene> mScene{};
    int8_t mPlayerNumber{};
};