#include "PlayerUpdateStrategy.h"

void PlayerUpdateStrategy::update(const PlayerInputState &inputState, float deltaTime)
{
    mPlayer.getPlayerState()->update(deltaTime, mPlayer, *mPlayer.getScene());
    switch (mPlayer.getPlayerState()->getState())
    {
    case PlayerStateEnum::STATE_STUNNED:
    case PlayerStateEnum::STATE_CASTING:
    case PlayerStateEnum::STATE_ATTACKING:
        // Ignore inputs
        break;
    case PlayerStateEnum::STATE_FISHING:
        // Allow only fishing inputs
        if (inputState.fish)
        {
            // Stop fishing and check for success
            if (mPlayer.getPlayerState()->getStateTimer() <= CATCH_TIMER)
            {
                // Successful catch
                mPlayer.getPlayerState()->setActionSuccess(true);
            }
            mPlayer.getPlayerState()->changeState(PlayerStateEnum::STATE_IDLE, mPlayer, *mPlayer.getScene());
        }
        break;
    case PlayerStateEnum::STATE_WALKING:
    case PlayerStateEnum::STATE_IDLE:
        // Process all inputs
        if (inputState.attack)
        {
            // Start attack
            mPlayer.getPlayerState()->changeState(PlayerStateEnum::STATE_ATTACKING, mPlayer, *mPlayer.getScene());
            break;
        }

        if (inputState.fish)
        {
            // Start fishing
            mPlayer.getPlayerState()->changeState(PlayerStateEnum::STATE_CASTING, mPlayer, *mPlayer.getScene());
            break;
        }

        if (abs(inputState.move.x) > MIN_MOVE_INPUT || abs(inputState.move.y) > MIN_MOVE_INPUT)
        {
            // Normalize rotation vector
            Vector2 normMove = normalize(inputState.move);
            // Start walking
            mPlayer.getPlayerState()->changeState(PlayerStateEnum::STATE_WALKING, mPlayer, *mPlayer.getScene());
            mPlayer.setRotation({normMove.x, normMove.y});
            mPlayer.setVelocity({normMove.x * BASE_SPEED, 0.0f, -normMove.y * BASE_SPEED});
            break;
        }

        mPlayer.getPlayerState()->changeState(PlayerStateEnum::STATE_IDLE, mPlayer, *mPlayer.getScene());
        break;
    default:
        break;
    }
}