#include "PlayerAi.h"

#include "Config.h"
#include "Collider.h"

void PlayerAi::reset()
{
    mMovementTarget = {};
    mTarget = nullptr;
    mDelayActionTimer = 0.0f;
    mDelayCatchTimer = 0.8f;
    mInputState = {
        .move = {0.0f, 0.0f},
        .fish = false,
        .attack = false,
    };
}

void PlayerAi::update(float deltaTime, Player *allPlayers, Containers::vector<bool> &winners)
{
    // Reset input state
    mInputState = {
        .move = {0.0f, 0.0f},
        .fish = false,
        .attack = false,
    };

    if (mDelayActionTimer > 0.0f)
    {
        mDelayActionTimer -= deltaTime;
        return;
    }

    PlayerStateEnum state = mPlayer->getPlayerState()->getState();

    switch (state)
    {
    case PlayerStateEnum::STATE_IDLE:
        // Find something to do
        updateIdle(deltaTime, allPlayers, winners);
    case PlayerStateEnum::STATE_WALKING:
        // Move towards target and perform actions
        updateMovementTarget();
        moveToTarget();
        break;
    case PlayerStateEnum::STATE_FISHING:
        // Check fishing status
        if (mPlayer->getPlayerState()->getStateTimer() < CATCH_TIMER - mDelayCatchTimer)
        {
            mInputState.fish = true;
            mDelayActionTimer = 2.0f;
            mDelayCatchTimer = 0.6f;
        }
        break;
    case PlayerStateEnum::STATE_ATTACKING:
        mDelayActionTimer = 5.0f;
        break;
    case PlayerStateEnum::STATE_CASTING:
    case PlayerStateEnum::STATE_STUNNED:
    default:
        // Do nothing, wait for action to complete
        break;
    }
}

void PlayerAi::updateIdle(float deltaTime, Player *allPlayers, Containers::vector<bool> &winners)
{
    switch (mBehavior)
    {
    case AIBehavior::BEHAVE_BALANCED:
        // Alternate between fish and players
        if (mTarget != nullptr)
        {
            mTarget = nullptr;
            mMovementTarget = findClosestFish();
        }
        else
        {
            mTarget = findWinnerTarget(allPlayers, winners);
            if (mTarget == nullptr)
            {
                mMovementTarget = findClosestFish();
            }
        }
        break;
    case AIBehavior::BEHAVE_BULLY:
        mTarget = findWinnerTarget(allPlayers, winners);
        if (mTarget == nullptr)
        {
            mMovementTarget = findClosestFish();
        }
        break;
    case AIBehavior::BEHAVE_FISHERMAN:
        mMovementTarget = findClosestFish();
        break;
    }
}

void PlayerAi::updateMovementTarget()
{
    if (mTarget != nullptr)
    {
        mMovementTarget = mTarget->getPosition();
    }
}

void PlayerAi::moveToTarget()
{
    Vector3 position = mPlayer->getPosition();
    Vector3 distance = mMovementTarget - position;

    float distanceThreshold = mTarget ? HITBOX_RADIUS * HITBOX_RADIUS : 1.f;
    if (distance.magSqrd() < distanceThreshold)
    {
        mInputState.move = {0.0f, 0.0f};
        mInputState.fish = mTarget == nullptr;
        mInputState.attack = mTarget != nullptr;
        return;
    }

    Vector3 direction = normAndScale(distance, BASE_SPEED);

    mInputState = {
        .move = {direction.x, -direction.z},
        .fish = false,
        .attack = false,
    };
}

Player *PlayerAi::findWinnerTarget(Player *allPlayers, Containers::vector<bool> &winners) const
{
    for (int i = 0; i < Core::MAX_PLAYERS; i++)
    {
        if (i == mPlayer->getPlayerNumber())
        {
            continue;
        }

        if (winners[i])
        {
            return &allPlayers[i];
        }
    }
    return nullptr;
}

Vector3 PlayerAi::findClosestFish() const
{
    Vector3 position = mPlayer->getPosition();
    Vector3 closestFish = normAndScale(position, Collision::PLAYING_R);
    return closestFish;
}