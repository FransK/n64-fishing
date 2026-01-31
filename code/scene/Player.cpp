#include "Player.h"

#include <string>

#include "Config.h"
#include "math/Quaternion.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "scene/ActorFlags.h"
#include "scene/Fish.h"
#include "scene/PlayerConsts.h"
#include "scene/PlayerColliders.h"

using namespace Math;

Player::Player(std::shared_ptr<Collision::CollisionScene> scene, int8_t playerNumber)
    : mAttackActor(std::make_unique<Actor>()),
      mPlayerState(std::make_unique<PlayerState>()),
      mScene(std::move(scene)),
      mPlayerNumber(playerNumber)
{
    setFlag(ActorFlags::IS_PLAYER);

    Collision::Collider collider = Collision::Collider{
        .entityId = this->getEntityId(),
        .actor = this,
        .type = PlayerColliderType,
        .scale = 1.0f,
        .hasGravity = true,
        .isFixed = false,
        .collisionLayers = static_cast<uint16_t>(Collision::CollisionLayers::CollisionLayerTangible),
        .collisionGroup = static_cast<uint16_t>(FIRST_PLAYER_COLLIDER_GROUP + playerNumber),
    };

    collider.center.y = PlayerColliderType.data.cylinder.halfHeight;
    collider.recalcBB();

    mScene->add(std::move(collider));

    Collision::Collider damageTrigger = Collision::Collider{
        .entityId = this->getAttackActor()->getEntityId(),
        .actor = this->getAttackActor(),
        .type = DamageTriggerType,
        .scale = 1.0f,
        .hasGravity = false,
        .isTrigger = true,
        .collisionLayers = static_cast<uint16_t>(Collision::CollisionLayers::CollisionLayerTangible),
        .collisionGroup = static_cast<uint16_t>(FIRST_PLAYER_COLLIDER_GROUP + playerNumber),
    };

    damageTrigger.setFlag(Collision::ColliderFlags::IS_ATTACK_TRIGGER);
    damageTrigger.center.y = DamageTriggerType.data.sphere.radius;
    damageTrigger.recalcBB();

    mScene->add(std::move(damageTrigger), false);
}

Player::~Player()
{
    mScene->remove(this->getEntityId());
    mScene->remove(this->getAttackActor()->getEntityId());
}

void Player::drawBillboard(T3DViewport &viewport) const
{
    if (mPlayerState->getState() != PlayerStateEnum::STATE_FISHING)
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
    std::string mBillboardText = mPlayerState->canCatch() ? "HOOKED!" : "Fishing...";
    rdpq_text_printf(&param, Core::FONT_BILLBOARD, x, y, "%s", mBillboardText.c_str());
}

void Player::reset(Vector3 const &position, Vector2 const &rotation)
{
    setPosition(position);
    setRotation(rotation);
    setVelocity({0.0f, 0.0f, 0.0f});
    clearFlag(ActorFlags::IS_STUNNED);
    mPlayerState->reset(*this, *mScene);
}