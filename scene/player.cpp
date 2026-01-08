#include <string>
#include "../main.h"
#include "player.h"
#include "fish.h"
#include "../math/quaternion.h"

namespace Fishing
{
    Player::Player(Collision::Scene *scene, T3DModel *model) : mScene(scene),
                                                               mModel(model)
    {
        // Model Credits: Quaternius (CC0) https://quaternius.com/
        mSkeleton = t3d_skeleton_create(mModel);

        mAnimIdle = t3d_anim_create(mModel, "Idle");
        t3d_anim_attach(&mAnimIdle, &mSkeleton);

        mAnimRun = t3d_anim_create(mModel, "Run");
        t3d_anim_attach(&mAnimRun, &mSkeleton);
        t3d_anim_set_playing(&mAnimRun, false);

        mAnimPunch = t3d_anim_create(mModel, "Punch");
        t3d_anim_attach(&mAnimPunch, &mSkeleton);
        t3d_anim_set_looping(&mAnimPunch, false);
        t3d_anim_set_playing(&mAnimPunch, false);
        t3d_anim_set_speed(&mAnimPunch, SHOVE_TIME_SCALE);

        mAnimReceiveHit = t3d_anim_create(mModel, "RecieveHit");
        t3d_anim_attach(&mAnimReceiveHit, &mSkeleton);
        t3d_anim_set_looping(&mAnimReceiveHit, false);
        t3d_anim_set_playing(&mAnimReceiveHit, false);

        mAnimCast = t3d_anim_create(mModel, "SwordSlash");
        t3d_anim_attach(&mAnimCast, &mSkeleton);
        t3d_anim_set_looping(&mAnimCast, false);
        t3d_anim_set_playing(&mAnimCast, false);

        mActiveAnim = &mAnimIdle;
        mModelMatFP = (T3DMat4FP *)malloc_uncached(sizeof(T3DMat4FP));
    }

    Player::~Player()
    {
        // Player cleanup
        rspq_block_free(mDplPlayer);

        mActiveAnim = nullptr;

        t3d_anim_destroy(&mAnimIdle);
        t3d_anim_destroy(&mAnimRun);
        t3d_anim_destroy(&mAnimPunch);
        t3d_anim_destroy(&mAnimReceiveHit);
        t3d_anim_destroy(&mAnimCast);
        t3d_skeleton_destroy(&mSkeleton);

        free_uncached(mModelMatFP);
    }

    void Player::play_animation(Anim anim)
    {
        t3d_anim_set_playing(&mAnimIdle, anim == Anim::IDLE);
        t3d_anim_set_playing(&mAnimRun, anim == Anim::RUN);
        t3d_anim_set_playing(&mAnimPunch, anim == Anim::SHOVE);
        t3d_anim_set_playing(&mAnimReceiveHit, anim == Anim::RECEIVE_SHOVE);
        t3d_anim_set_playing(&mAnimCast, anim == Anim::CAST);

        switch (anim)
        {
        case Anim::IDLE:
            mActiveAnim = &mAnimIdle;
            return;
        case Anim::RUN:
            mActiveAnim = &mAnimRun;
            return;
        case Anim::SHOVE:
            mActiveAnim = &mAnimPunch;
            return;
        case Anim::RECEIVE_SHOVE:
            mActiveAnim = &mAnimReceiveHit;
            return;
        case Anim::CAST:
            mActiveAnim = &mAnimCast;
            return;
        }
    }

    void Player::init(int8_t playerNumber, T3DVec3 position, Vector2 rotation, color_t color, bool is_human)
    {
        assert(playerNumber >= 0 && playerNumber < MAXPLAYERS);

        mCollider = Collision::Collider{
            .entityId = GetNextEntityId(),
            .position = {position.x, position.y, position.z},
            .rotation = rotation,
            .type = PlayerColliderType,
            .scale = 1.0f,
            .hasGravity = true,
            .isFixed = false,
            .collisionLayers = CollisionLayerTangible,
            .collisionGroup = uint16_t(FIRST_PLAYER_COLLIDER_GROUP + playerNumber),
        };

        mCollider.center.y = PlayerColliderType.data.cylinder.halfHeight;
        mCollider.recalcBB();

        mScene->add(&mCollider);

        mPlayerNumber = playerNumber;
        mColor = color;
        mIsHuman = is_human;

        rspq_block_begin();
        t3d_matrix_push(mModelMatFP);
        rdpq_set_prim_color(mColor);
        t3d_model_draw_skinned(mModel, &mSkeleton);
        t3d_matrix_pop(1);
        mDplPlayer = rspq_block_end();

        Quaternion mathQuat;
        Math::Quaternion::axisComplex({0.0f, 1.0f, 0.0f}, &mCollider.rotation, &mathQuat);

        T3DVec3 scale = {{PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE}};
        T3DQuat quat = {{mathQuat.x, mathQuat.y, mathQuat.z, mathQuat.w}};
        T3DVec3 pos = {{mCollider.position.x, mCollider.position.y, mCollider.position.z}};
        t3d_mat4fp_from_srt(mModelMatFP,
                            scale,
                            quat,
                            pos);
    }

    void Player::update_fixed(float deltaTime, InputState input)
    {
        // assert(mPlayerNumber != -1 && "Player needs to be initialized before update.");
    }

    void Player::update_animation(float deltaTime)
    {
        t3d_anim_update(mActiveAnim, deltaTime);
        t3d_skeleton_update(&mSkeleton);

        Quaternion mathQuat;
        Math::Quaternion::axisComplex({0.0f, 1.0f, 0.0f}, &mCollider.rotation, &mathQuat);

        T3DVec3 scale = {{PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE}};
        T3DQuat quat = {{mathQuat.x, mathQuat.y, mathQuat.z, mathQuat.w}};
        T3DVec3 pos = {{mCollider.position.x, mCollider.position.y, mCollider.position.z}};
        t3d_mat4fp_from_srt(mModelMatFP,
                            scale,
                            quat,
                            pos);
    }

    void Player::update(float deltaTime, InputState input, bool updateAI = true)
    {
        assert(mPlayerNumber != -1 && "Player needs to be initialized before update.");

        // If currently playing an animation, continue playing
        if (mAnimTimer >= 0)
        {
            mAnimTimer -= deltaTime;
            update_animation(deltaTime);

            if (is_casting())
            {
                mCastTimer -= deltaTime;
                if (mCastTimer <= 0.0f)
                {
                    mFishingTimer = Fish::get_new_timer();
                    t3d_anim_set_time(&mAnimCast, 0.0f);
                }
            }
            return;
        }

        play_animation(Anim::IDLE);
        mCollider.velocity = {0.0f, 0.0f, 0.0f};

        // Movement
        // Resting thumb on stick doesn't do anything
        // Lightly pressing rotates character
        // Pushing on stick moves character
        if (can_move() &&
            (abs(input.move.x) > MIN_MOVE_INPUT || abs(input.move.y) > MIN_MOVE_INPUT))
        {
            Vector2 normMove{};
            Vector2::norm(&input.move, &normMove);
            mCollider.rotation = {-normMove.y, normMove.x};

            if (abs(input.move.x) > ROTATION_INPUT || abs(input.move.y) > ROTATION_INPUT)
            {
                mCollider.velocity = {normMove.x * mSpeed, 0, -normMove.y * mSpeed};

                play_animation(Anim::RUN);
            }

            update_animation(deltaTime);
            return;
        }

        if (is_fishing())
        {
            mFishingTimer -= deltaTime;

            if (mIsHuman)
            {
                if (input.fish)
                {
                    if (is_catchable())
                    {
                        mFishCaught++;
                        debugf("Caught fish: %i\n", mFishCaught);
                    }
                    else
                    {
                        debugf("Missed a fish!\n");
                    }
                    mFishingTimer = 0.0f;
                }
            }
            else
            {
                // AI fishing
                if (is_catchable())
                {
                    if (mFishingTimer <= 0.1f)
                    {
                        mFishCaught++;
                        mFishingTimer = 0.0f;
                        mAiDelayTimer = (2 - core_get_aidifficulty()) * 5 + rand() % ((3 - core_get_aidifficulty()) * 3);
                    }
                }
            }
        }
        else if (input.fish)
        {
            cast();
        }
        else if (!mIsHuman && updateAI)
        {
            if (mAiDelayTimer > 0.0f)
            {
                mAiDelayTimer -= deltaTime;
            }
            else
            {
                cast();
            }
        }

        update_animation(deltaTime);
    }

    uint32_t Player::draw(T3DViewport &viewport, const T3DVec3 &camPos) const
    {
        rspq_block_run(mDplPlayer);
        return mModel->totalVertCount;
    }

    void Player::draw_billboard(T3DViewport &viewport, const T3DVec3 &camPos) const
    {
        if (!is_fishing())
        {
            return;
        }

        // World position
        T3DVec3 localPos = (T3DVec3){{0.0f, BILLBOARD_YOFFSET, 0.0f}};

        // Screen position
        T3DVec3 worldPos = (T3DVec3){{
            mCollider.position.x + localPos.v[0] * 0.125f,
            mCollider.position.y + localPos.v[1] * 0.125f,
            mCollider.position.z + localPos.v[2] * 0.125f,
        }};

        T3DVec3 screenPos;
        t3d_viewport_calc_viewspace_pos(viewport, screenPos, worldPos);

        int x = floorf(screenPos.v[0]);
        int y = floorf(screenPos.v[1]);

        const rdpq_textparms_t param{};
        std::string mBillboardText = is_catchable() ? "HOOKED!" : "Fishing...";
        rdpq_text_printf(&param, FONT_BILLBOARD, x, y, "%s", mBillboardText.c_str());
    }

    void Player::get_attack_position(Vector2 &attack_pos) const
    {
        attack_pos.x = mCollider.position.x + mCollider.rotation.x * ATTACK_OFFSET;
        attack_pos.y = mCollider.position.z + mCollider.rotation.y * ATTACK_OFFSET;
    }

    uint8_t Player::get_fish_caught() const
    {
        return mFishCaught;
    }

    const Vector3 &Player::get_position() const
    {
        return mCollider.position;
    }

    void Player::shove()
    {
        play_animation(Anim::SHOVE);
        mAnimTimer = SHOVE_TIME;
    }

    void Player::receive_shove(const float &direction)
    {
        float s, c;
        fm_sincosf(direction, &s, &c);
        mCollider.velocity.x += s * SHOVE_DIST;
        mCollider.velocity.y += c * SHOVE_DIST;

        mFishingTimer = 0.0f;

        play_animation(Anim::RECEIVE_SHOVE);
        mAnimTimer = RECEIVE_SHOVE_TIME;
    }

    void Player::cast()
    {
        mCastTimer = CAST_TIME;

        play_animation(Anim::CAST);
        mAnimTimer = CAST_TIME;
    }
}