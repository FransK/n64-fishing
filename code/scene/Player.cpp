#include "Player.h"

#include <memory>
#include <string>

#include "Config.h"
#include "collision/CollisionScene.h"
#include "math/Quaternion.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "scene/ActorFlags.h"
#include "scene/Fish.h"
#include "scene/PlayerConsts.h"
#include "scene/PlayerColliders.h"

using namespace Math;

Player::Player(std::unique_ptr<Collision::CollisionScene> const &scene, int8_t playerNumber)
    : mPlayerNumber(playerNumber)
{
    setFlag(ActorFlags::IS_PLAYER);

    std::unique_ptr<Collision::Collider> collider = std::make_unique<Collision::Collider>();
    collider->entityId = this->getEntityId();
    collider->actor = this;
    collider->type = PlayerColliderType;
    collider->scale = 1.0f;
    collider->hasGravity = true;
    collider->isFixed = false;
    collider->collisionLayers = static_cast<uint16_t>(Collision::CollisionLayers::CollisionLayerTangible);
    collider->collisionGroup = static_cast<uint16_t>(FIRST_PLAYER_COLLIDER_GROUP + playerNumber);
    collider->center.y = PlayerColliderType.data.cylinder.halfHeight;

    collider->recalcBB();

    std::unique_ptr<Collision::Collider> damageTrigger = std::make_unique<Collision::Collider>();
    damageTrigger->entityId = this->attackActorEntityId();
    damageTrigger->actor = this->getAttackActor();
    damageTrigger->type = DamageTriggerType;
    damageTrigger->scale = 1.0f;
    damageTrigger->hasGravity = false;
    damageTrigger->isTrigger = true;
    damageTrigger->collisionLayers = static_cast<uint16_t>(Collision::CollisionLayers::CollisionLayerTangible);
    damageTrigger->collisionGroup = static_cast<uint16_t>(FIRST_PLAYER_COLLIDER_GROUP + playerNumber);
    damageTrigger->center.y = DamageTriggerType.data.sphere.radius;

    damageTrigger->setFlag(Collision::ColliderFlags::IS_ATTACK_TRIGGER);
    damageTrigger->recalcBB();

    scene->add(std::move(collider));
    scene->add(std::move(damageTrigger), false);
}

void Player::drawBillboard(T3DViewport &viewport) const
{
    if (mActionState != PlayerStateEnum::STATE_FISHING)
    {
        return;
    }

    // World position
    T3DVec3 localPos = (T3DVec3){{0.0f, BILLBOARD_YOFFSET, 0.0f}};

    // Screen position
    T3DVec3 worldPos = (T3DVec3){{
        this->getPosition().x + localPos.v[0] * 0.125f,
        this->getPosition().y + localPos.v[1] * 0.125f,
        this->getPosition().z + localPos.v[2] * 0.125f,
    }};

    T3DVec3 screenPos;
    t3d_viewport_calc_viewspace_pos(viewport, screenPos, worldPos);

    int x = floorf(screenPos.v[0]);
    int y = floorf(screenPos.v[1]);

    const rdpq_textparms_t param{};
    std::string mBillboardText = canCatch() ? "HOOKED!" : "Fishing...";
    rdpq_text_printf(&param, Core::FONT_BILLBOARD, x, y, "%s", mBillboardText.c_str());
}

void Player::update(const PlayerInputState &inputState, float deltaTime)
{
    actionStateUpdate(deltaTime);

    switch (mActionState)
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
            if (mStateTimer <= CATCH_TIMER)
            {
                // Successful catch
                setActionSuccess(true);
            }
            changeState(PlayerStateEnum::STATE_IDLE);
        }
        break;
    case PlayerStateEnum::STATE_WALKING:
    case PlayerStateEnum::STATE_IDLE:
        // Process all inputs
        if (inputState.attack)
        {
            // Start attack
            changeState(PlayerStateEnum::STATE_ATTACKING);
            break;
        }

        if (inputState.fish)
        {
            // Start fishing
            changeState(PlayerStateEnum::STATE_CASTING);
            break;
        }

        if (abs(inputState.move.x) > MIN_MOVE_INPUT || abs(inputState.move.y) > MIN_MOVE_INPUT)
        {
            // Normalize rotation vector
            Vector2 normMove = normalize(inputState.move);
            // Start walking
            changeState(PlayerStateEnum::STATE_WALKING);
            setRotation({normMove.x, normMove.y});
            setVelocity({normMove.x * BASE_SPEED, 0.0f, -normMove.y * BASE_SPEED});
            break;
        }

        changeState(PlayerStateEnum::STATE_IDLE);
        break;
    }
}

void Player::reset(Vector3 const &position, Vector2 const &rotation)
{
    setPosition(position);
    setRotation(rotation);
    setVelocity({0.0f, 0.0f, 0.0f});
    clearFlag(ActorFlags::IS_STUNNED);

    changeState(PlayerStateEnum::STATE_IDLE);
    mStateTimer = 0.0f;
    mActionSuccess = false;
    mFishCaught = 0;
}

void Player::changeState(const PlayerStateEnum &newState)
{
    if (mActionState == newState)
    {
        return;
    }

    switch (mActionState)
    {
    case PlayerStateEnum::STATE_FISHING:
        if (mActionSuccess)
        {
            mFishCaught += 1;
            mActionSuccess = false;
        }
        break;
    case PlayerStateEnum::STATE_STUNNED:
        clearFlag(ActorFlags::IS_STUNNED);
        break;
    case PlayerStateEnum::STATE_IDLE:
    case PlayerStateEnum::STATE_WALKING:
    case PlayerStateEnum::STATE_CASTING:
    case PlayerStateEnum::STATE_ATTACKING:
        break;
    }

    mLastActionState = mActionState;
    mActionState = newState;

    switch (mActionState)
    {
    case PlayerStateEnum::STATE_ATTACKING:
        mStateTimer = SHOVE_TIME;
        setVelocity({0.0f, 0.0f, 0.0f});
        mAttackActor->setPosition({getPosition().x + getRotation().x * ATTACK_OFFSET,
                                   getPosition().y + PlayerColliderType.data.cylinder.halfHeight,
                                   getPosition().z + -getRotation().y * ATTACK_OFFSET});
        break;
    case PlayerStateEnum::STATE_CASTING:
        mStateTimer = CAST_TIME;
        setVelocity({0.0f, 0.0f, 0.0f});
        break;
    case PlayerStateEnum::STATE_FISHING:
        mStateTimer = Fish::GetNewTimer();
        setVelocity({0.0f, 0.0f, 0.0f});
        break;
    case PlayerStateEnum::STATE_STUNNED:
        mStateTimer = RECEIVE_SHOVE_TIME;
        setVelocity({0.0f, 0.0f, 0.0f});
        break;
    case PlayerStateEnum::STATE_IDLE:
        mStateTimer = 0.0f;
        setVelocity({0.0f, 0.0f, 0.0f});
        break;
    case PlayerStateEnum::STATE_WALKING:
        mStateTimer = 0.0f;
        break;
    }

    notify();
}

void Player::actionStateUpdate(float deltaTime)
{
    if (mActionState != PlayerStateEnum::STATE_STUNNED &&
        hasFlag(ActorFlags::IS_STUNNED))
    {
        changeState(PlayerStateEnum::STATE_STUNNED);
        return;
    }

    if (mActionState == PlayerStateEnum::STATE_IDLE || mActionState == PlayerStateEnum::STATE_WALKING)
    {
        return;
    }

    mStateTimer -= deltaTime;
    if (mStateTimer <= 0.0f)
    {
        if (mActionState == PlayerStateEnum::STATE_CASTING)
        {
            // After casting, start fishing
            changeState(PlayerStateEnum::STATE_FISHING);
            return;
        }

        // After other timed states, go back to idle
        changeState(PlayerStateEnum::STATE_IDLE);
    }
}