#pragma once

#include "CollisionScene.h"
#include "Observable.h"
#include "PlayerConsts.h"
#include "Vector3.h"

class Player;

enum struct PlayerStateEnum
{
    STATE_IDLE,
    STATE_WALKING,
    STATE_ATTACKING,
    STATE_STUNNED,
    STATE_CASTING,
    STATE_FISHING,
};

class PlayerState : public Observable<PlayerState>
{
public:
    bool canCatch() const { return mState == PlayerStateEnum::STATE_FISHING && mStateTimer < CATCH_TIMER; }
    PlayerStateEnum getState() const { return mState; }
    int getFishCaught() const { return mFishCaught; }
    float getStateTimer() const { return mStateTimer; }
    void setActionSuccess(bool success) { mActionSuccess = success; }

    void changeState(const PlayerStateEnum &newState, Player &player, Collision::CollisionScene &collScene);
    void reset(Player &player, Collision::CollisionScene &collScene);
    void update(float deltaTime, Player &player, Collision::CollisionScene &collScene);

private:
    PlayerStateEnum mState{PlayerStateEnum::STATE_IDLE};
    float mStateTimer{};
    int mFishCaught{};
    bool mActionSuccess{false};
};