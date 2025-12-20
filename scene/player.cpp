#include "player.h"
#include "fish.h"
#include <string>

Player::Player()
{
    // Model Credits: Quaternius (CC0) https://quaternius.com/
    mModel = t3d_model_load("rom:/n64-fishing/player2.t3dm");
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
    t3d_anim_set_speed(&mAnimPunch, 3.0);

    mAnimReceiveHit = t3d_anim_create(mModel, "RecieveHit");
    t3d_anim_attach(&mAnimReceiveHit, &mSkeleton);
    t3d_anim_set_looping(&mAnimReceiveHit, false);
    t3d_anim_set_playing(&mAnimReceiveHit, false);

    mModelMatFP = (T3DMat4FP *)malloc_uncached(sizeof(T3DMat4FP));
}

Player::~Player()
{
    // Player cleanup
    rspq_block_free(mDplPlayer);

    t3d_anim_destroy(&mAnimIdle);
    t3d_anim_destroy(&mAnimRun);
    t3d_anim_destroy(&mAnimPunch);
    t3d_anim_destroy(&mAnimReceiveHit);
    t3d_skeleton_destroy(&mSkeleton);

    free_uncached(mModelMatFP);
    t3d_model_free(mModel);
}

void Player::play_animation(Anim anim)
{
    t3d_anim_set_playing(&mAnimIdle, anim == Anim::IDLE);
    t3d_anim_set_playing(&mAnimRun, anim == Anim::RUN);
    t3d_anim_set_playing(&mAnimPunch, anim == Anim::SHOVE);
    t3d_anim_set_playing(&mAnimReceiveHit, anim == Anim::RECEIVE_SHOVE);
}

void Player::init(int playerNumber, T3DVec3 position, float rotation, color_t color, bool is_human)
{
    assert(playerNumber >= 0 && playerNumber < MAXPLAYERS);

    mPlayerNumber = playerNumber;
    mColor = color;
    mPosition = position;
    mRotationY = rotation;
    mIsHuman = is_human;

    rspq_block_begin();
    t3d_matrix_push(mModelMatFP);
    rdpq_set_prim_color(mColor);
    t3d_model_draw_skinned(mModel, &mSkeleton);
    t3d_matrix_pop(1);
    mDplPlayer = rspq_block_end();

    t3d_mat4fp_from_srt_euler(mModelMatFP,
                              (float[3]){0.125f, 0.125f, 0.125f},
                              (float[3]){0.0f, -mRotationY, 0},
                              mPosition.v);
}

void Player::update_fixed(float deltaTime, InputState input)
{
    assert(mPlayerNumber != -1 && "Player needs to be initialized before update.");

    // If currently playing an animation, continue playing
    if (mAnimTimer >= 0)
    {
        mAnimTimer -= deltaTime;
        return;
    }

    play_animation(Anim::IDLE);

    // Movement
    // Resting thumb on stick doesn't do anything
    // Lightly pressing rotates character
    // Pushing on stick moves character
    if (!is_fishing() &&
        (abs(input.move.v[0]) > MIN_MOVE_INPUT || abs(input.move.v[2]) > MIN_MOVE_INPUT))
    {
        mRotationY = atan2f(input.move.v[0], input.move.v[2]);

        if (abs(input.move.v[0]) > ROTATION_INPUT || abs(input.move.v[2]) > ROTATION_INPUT)
        {
            t3d_vec3_norm(input.move);
            mPosition.v[0] += input.move.v[0] * mSpeed;
            mPosition.v[2] += input.move.v[2] * mSpeed;

            play_animation(Anim::RUN);
        }
    }

    // Restrict movement to playing surface
    constrain_position();
}

void Player::constrain_position()
{
    float squared_position = mPosition.v[0] * mPosition.v[0] + mPosition.v[2] * mPosition.v[2];

    if (squared_position > PLAYING_R2)
    {
        float angle = atan2f(mPosition.v[0], mPosition.v[2]);
        mPosition.v[0] = PLAYING_R * sinf(angle);
        mPosition.v[2] = PLAYING_R * cosf(angle);
    }
}

void Player::update(float deltaTime, InputState input, bool updateAI = true)
{
    assert(mPlayerNumber != -1 && "Player needs to be initialized before update.");

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
                    fprintf(stderr, "Caught fish: %i\n", mFishCaught);
                }
                else
                {
                    fprintf(stderr, "Missed a fish!\n");
                }
                mFishingTimer = 0.0f;
            }
        }
        else
        {
            // AI fishing
            if (is_catchable())
            {
                float catchModifier = 10 * (CATCH_TIMER - mFishingTimer) / CATCH_TIMER;
                if (rand() % 1000 < catchModifier)
                {
                    mFishCaught++;
                    mFishingTimer = 0.0f;
                }
            }
        }
    }
    else if (input.fish)
    {
        mFishingTimer = Fish::get_new_timer();
    }
    else if (!mIsHuman && updateAI)
    {
        if (rand() % 1000 < (20 * core_get_aidifficulty() + 20))
        {
            mFishingTimer = Fish::get_new_timer();
        }
    }

    // Update model and animation for drawing
    t3d_anim_update(&mAnimIdle, deltaTime);
    t3d_anim_update(&mAnimRun, deltaTime);
    t3d_anim_update(&mAnimPunch, deltaTime);
    t3d_anim_update(&mAnimReceiveHit, deltaTime);

    t3d_skeleton_update(&mSkeleton);
    t3d_mat4fp_from_srt_euler(mModelMatFP,
                              (float[3]){0.125f, 0.125f, 0.125f},
                              (float[3]){0.0f, -mRotationY, 0},
                              mPosition.v);
}

void Player::draw(T3DViewport &viewport, const T3DVec3 &camPos) const
{
    rspq_block_run(mDplPlayer);
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
        mPosition.v[0] + localPos.v[0] * 0.125f,
        mPosition.v[1] + localPos.v[1] * 0.125f,
        mPosition.v[2] + localPos.v[2] * 0.125f,
    }};

    T3DVec3 screenPos;
    t3d_viewport_calc_viewspace_pos(viewport, screenPos, worldPos);

    int x = floorf(screenPos.v[0]);
    int y = floorf(screenPos.v[1]);

    const rdpq_textparms_t param{};
    std::string mBillboardText = is_catchable() ? "HOOKED!" : "Fishing...";
    rdpq_text_printf(&param, FONT_BILLBOARD, x, y, "%s", mBillboardText.c_str());
}

void Player::get_attack_position(fm_vec2_t &attack_pos) const
{
    float s, c;
    fm_sincosf(mRotationY, &s, &c);
    attack_pos.v[0] = mPosition.v[0] + s * ATTACK_OFFSET;
    attack_pos.v[1] = mPosition.v[2] + c * ATTACK_OFFSET;
}

uint8_t Player::get_fish_caught() const
{
    return mFishCaught;
}

const T3DVec3 &Player::get_position() const
{
    return mPosition;
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
    mPosition.v[0] += s * SHOVE_DIST;
    mPosition.v[2] += c * SHOVE_DIST;

    mFishingTimer = 0.0f;

    play_animation(Anim::RECEIVE_SHOVE);
    mAnimTimer = RECEIVE_SHOVE_TIME;
}