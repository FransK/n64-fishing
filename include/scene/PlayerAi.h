#pragma once

#include "containers/Vector.h"
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
    PlayerAi(Player *player) : mPlayer(player) {} // TODO: Should not be holding raw pointer here

    void reset();
    void setBehavior(AIBehavior behavior) { mBehavior = behavior; }
    void update(float deltaTime, Player *allPlayers, Containers::vector<bool> &winners);
    PlayerInputState inputState() const { return mInputState; }

private:
    AIBehavior mBehavior{AIBehavior::BEHAVE_FISHERMAN};
    Vector3 mMovementTarget{};
    const Player *mPlayer{}; // TODO
    const Player *mTarget{}; // TODO
    float mDelayActionTimer{0.0f};
    float mDelayCatchTimer{0.8f};
    PlayerInputState mInputState{};

    void updateIdle(float deltaTime, Player *allPlayers, Containers::vector<bool> &winners);
    void updateMovementTarget();
    void moveToTarget();
    Player *findWinnerTarget(Player *allPlayers, Containers::vector<bool> &winners) const;
    Vector3 findClosestFish() const;
};