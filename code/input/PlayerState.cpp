#include "PlayerState.h"
#include "Player.h"
#include "ActorFlags.h"

#include "animation/AnimationComponent.h"
#include "math/Vector3.h"
#include "scene/PlayerColliders.h"

void PlayerState::changeState(const PlayerStateEnum &newState, Player &player, Collision::CollisionScene &collScene)
{
    if (mState == newState)
    {
        return;
    }

    switch (mState)
    {
    case PlayerStateEnum::STATE_IDLE:
    case PlayerStateEnum::STATE_WALKING:
    case PlayerStateEnum::STATE_CASTING:
        break;
    case PlayerStateEnum::STATE_FISHING:
        if (mActionSuccess)
        {
            mFishCaught += 1;
            mActionSuccess = false;
        }
        break;
    case PlayerStateEnum::STATE_STUNNED:
        player.clearFlag(ActorFlags::IS_STUNNED);
        break;
    case PlayerStateEnum::STATE_ATTACKING:
        collScene.deactivate(player.getAttackActor()->getEntityId());
        break;
    }

    mState = newState;

    switch (mState)
    {
    case PlayerStateEnum::STATE_ATTACKING:
        mStateTimer = SHOVE_TIME;
        player.setVelocity({0.0f, 0.0f, 0.0f});
        player.getAttackActor()->setPosition({player.getPosition().x + player.getRotation().x * ATTACK_OFFSET,
                                              player.getPosition().y + PlayerColliderType.data.cylinder.halfHeight,
                                              player.getPosition().z + -player.getRotation().y * ATTACK_OFFSET});
        collScene.activate(player.getAttackActor()->getEntityId());
        notify();
        break;
    case PlayerStateEnum::STATE_CASTING:
        mStateTimer = CAST_TIME;
        player.setVelocity({0.0f, 0.0f, 0.0f});
        notify();
        break;
    case PlayerStateEnum::STATE_FISHING:
        // mStateTimer = Fish::GetNewTimer();
        mStateTimer = 4.0f; // Temporary fixed timer
        player.setVelocity({0.0f, 0.0f, 0.0f});
        notify();
        break;
    case PlayerStateEnum::STATE_STUNNED:
        mStateTimer = RECEIVE_SHOVE_TIME;
        player.setVelocity({0.0f, 0.0f, 0.0f});
        notify();
        break;
    case PlayerStateEnum::STATE_IDLE:
        mStateTimer = 0.0f;
        player.setVelocity({0.0f, 0.0f, 0.0f});
        notify();
        break;
    case PlayerStateEnum::STATE_WALKING:
        mStateTimer = 0.0f;
        notify();
        break;
    }
}

void PlayerState::reset(Player &player, Collision::CollisionScene &collScene)
{
    changeState(PlayerStateEnum::STATE_IDLE, player, collScene);
    mStateTimer = 0.0f;
    mActionSuccess = false;
    mFishCaught = 0;
}

void PlayerState::update(float deltaTime, Player &player, Collision::CollisionScene &collScene)
{
    if (mState != PlayerStateEnum::STATE_STUNNED &&
        player.hasFlag(ActorFlags::IS_STUNNED))
    {
        changeState(PlayerStateEnum::STATE_STUNNED, player, collScene);
        return;
    }

    if (mState == PlayerStateEnum::STATE_IDLE || mState == PlayerStateEnum::STATE_WALKING)
    {
        return;
    }

    mStateTimer -= deltaTime;
    if (mStateTimer <= 0.0f)
    {
        if (mState == PlayerStateEnum::STATE_CASTING)
        {
            // After casting, start fishing
            changeState(PlayerStateEnum::STATE_FISHING, player, collScene);
            return;
        }

        // After other timed states, go back to idle
        changeState(PlayerStateEnum::STATE_IDLE, player, collScene);
    }
}