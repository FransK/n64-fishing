#pragma once

#include <cstdint>
#include <memory>

#include <t3d/t3d.h>

#include "collision/CollisionScene.h"
#include "common/Observable.h"
#include "input/PlayerInputState.h"
#include "scene/Actor.h"
#include "scene/PlayerConsts.h"

enum struct PlayerStateEnum
{
    STATE_IDLE,
    STATE_WALKING,
    STATE_ATTACKING,
    STATE_STUNNED,
    STATE_CASTING,
    STATE_FISHING,
};

class Player : public Actor, public Observable<Player>
{
public:
    Player(std::unique_ptr<Collision::CollisionScene> const &scene, int8_t playerNumber);
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;
    Player(Player &&) = default;
    Player &operator=(Player &&) = default;

    void drawBillboard(T3DViewport &viewport) const;
    void changeState(const PlayerStateEnum &newState);
    void reset(Math::Vector3 const &position, Math::Vector2 const &rotation);
    void update(const PlayerInputState &inputState, float deltaTime);

    bool canCatch() const { return mActionState == PlayerStateEnum::STATE_FISHING && mStateTimer < CATCH_TIMER; }
    int getFishCaught() const { return mFishCaught; }
    float getStateTimer() const { return mStateTimer; }
    void setActionSuccess(bool success) { mActionSuccess = success; }

    Actor *getAttackActor() const { return mAttackActor.get(); }
    int attackActorEntityId() const { return mAttackActor->getEntityId(); }
    int8_t getPlayerNumber() const { return mPlayerNumber; }
    PlayerStateEnum actionState() const { return mActionState; }
    PlayerStateEnum lastActionState() const { return mLastActionState; }

private:
    std::unique_ptr<Actor> mAttackActor{std::make_unique<Actor>()};
    PlayerStateEnum mActionState{PlayerStateEnum::STATE_IDLE};
    PlayerStateEnum mLastActionState{PlayerStateEnum::STATE_IDLE};
    float mStateTimer{0.0f};
    int mFishCaught{0};
    bool mActionSuccess{false};
    int8_t mPlayerNumber;

    void actionStateUpdate(float deltaTime);
};