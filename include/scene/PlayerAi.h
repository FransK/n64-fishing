#pragma once

#include <vector>
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "input/PlayerInputState.h"
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
    PlayerAi(Player *player) : mPlayer(player) {}

    void reset();
    void setBehavior(AIBehavior behavior) { mBehavior = behavior; }
    void update(float deltaTime, Player *allPlayers, std::vector<bool> &winners);
    PlayerInputState getInputState() const { return mInputState; }

private:
    AIBehavior mBehavior{AIBehavior::BEHAVE_FISHERMAN};
    Vector3 mMovementTarget{};
    const Player *mPlayer{};
    const Player *mTarget{};
    float mDelayActionTimer{0.0f};
    float mDelayCatchTimer{0.8f};
    PlayerInputState mInputState{};

    void updateIdle(float deltaTime, Player *allPlayers, std::vector<bool> &winners);
    void updateMovementTarget();
    void moveToTarget();
    Player *findWinnerTarget(Player *allPlayers, std::vector<bool> &winners) const;
    Vector3 findClosestFish() const;
};