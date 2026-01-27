#pragma once

#include "math/Vector2.h"
#include "math/Vector3.h"
#include "input/InputState.h"
#include "input/PlayerData.h"
#include "input/PlayerState.h"
#include "Player.h"

enum struct AIBehavior : uint8_t
{
    BEHAVE_FISHERMAN,
    BEHAVE_BULLY,
    BEHAVE_BALANCED,
};

class PlayerAi
{
public:
    PlayerAi(PlayerData *player) : mPlayer(player) {}

    void setBehavior(AIBehavior behavior) { mBehavior = behavior; }
    void update(float deltaTime, const PlayerState &state, int playerNumber, PlayerData *players, uint8_t *winners);
    InputState getInputState() const { return mInputState; }

private:
    AIBehavior mBehavior{AIBehavior::BEHAVE_FISHERMAN};
    Vector3 mMovementTarget{};
    const PlayerData *mPlayer{};
    const PlayerData *mTarget{};
    float mDelayActionTimer{0.0f};
    float mDelayCatchTimer{0.8f};
    InputState mInputState{};

    void updateIdle(float deltaTime, int playerNumber, PlayerData *players, uint8_t *winners);
    void updateMovementTarget();
    void moveToTarget();
    PlayerData *findWinnerTarget(int playerNumber, PlayerData *players, uint8_t *winners) const;
    Vector3 findClosestFish() const;
};