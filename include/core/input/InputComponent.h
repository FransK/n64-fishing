#pragma once

#include <libdragon.h>

#include "collision/CollisionScene.h"

#include "InputState.h"
#include "Player.h"

template <typename InputStrategy>
class InputComponent
{
public:
    InputComponent(InputStrategy inputStrategy) : mInputStrategy(inputStrategy) {}

    void update(float deltaTime,
                Player &player,
                Collision::CollisionScene &collScene)
    {
        InputState inputState = mInputStrategy.update();

        player.getPlayerState()->update(deltaTime, player, collScene);
        switch (player.getPlayerState()->getState())
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
                if (player.getPlayerState()->getStateTimer() <= CATCH_TIMER)
                {
                    // Successful catch
                    player.getPlayerState()->setActionSuccess(true);
                }
                player.getPlayerState()->changeState(PlayerStateEnum::STATE_IDLE, player, collScene);
            }
            break;
        case PlayerStateEnum::STATE_WALKING:
        case PlayerStateEnum::STATE_IDLE:
            // Process all inputs
            if (inputState.attack)
            {
                // Start attack
                player.getPlayerState()->changeState(PlayerStateEnum::STATE_ATTACKING, player, collScene);
                break;
            }

            if (inputState.fish)
            {
                // Start fishing
                player.getPlayerState()->changeState(PlayerStateEnum::STATE_CASTING, player, collScene);
                break;
            }

            if (abs(inputState.move.x) > MIN_MOVE_INPUT || abs(inputState.move.y) > MIN_MOVE_INPUT)
            {
                // Normalize rotation vector
                Vector2 normMove{};
                Vector2::norm(&inputState.move, &normMove);
                // Start walking
                player.getPlayerState()->changeState(PlayerStateEnum::STATE_WALKING, player, collScene);
                player.setRotation({normMove.x, normMove.y});
                player.setVelocity({normMove.x * BASE_SPEED, 0.0f, -normMove.y * BASE_SPEED});
                break;
            }

            player.getPlayerState()->changeState(PlayerStateEnum::STATE_IDLE, player, collScene);
            break;
        default:
            break;
        }
    }

private:
    InputStrategy mInputStrategy{};
};