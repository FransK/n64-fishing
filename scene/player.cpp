#include "player.h"
#include "fish.h"
#include <string>

Player::Player()
{
    mModel = t3d_model_load("rom:/n64-fishing/player.t3dm");

    mModelMatFP = (T3DMat4FP *)malloc_uncached(sizeof(T3DMat4FP));

    rspq_block_begin();
    t3d_matrix_push(mModelMatFP);
    // rdpq_set_prim_color(mColor); Need to check model to see why this isn't working
    t3d_model_draw(mModel);
    t3d_matrix_pop(1);
    mDplPlayer = rspq_block_end();
}

Player::~Player()
{
    // Player cleanup
    rspq_block_free(mDplPlayer);
    free_uncached(mModelMatFP);
    t3d_model_free(mModel);
}

void Player::init(int playerNumber, T3DVec3 position, float rotation, color_t color, bool is_human)
{
    assert(playerNumber >= 0 && playerNumber < MAXPLAYERS);

    mPlayerNumber = playerNumber;
    mColor = color;
    mPosition = position;
    mRotationY = rotation;
    mIsHuman = is_human;
}

void Player::update_fixed(InputState input)
{
    assert(mPlayerNumber != -1 && "Player needs to be initialized before update.");

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
        }
    }

    // Restrict movement to playing surface
    if (mPosition.v[0] < -BOX_SIZE)
        mPosition.v[0] = -BOX_SIZE;
    if (mPosition.v[0] > BOX_SIZE)
        mPosition.v[0] = BOX_SIZE;
    if (mPosition.v[2] < -BOX_SIZE)
        mPosition.v[2] = -BOX_SIZE;
    if (mPosition.v[2] > BOX_SIZE)
        mPosition.v[2] = BOX_SIZE;

    // Update player matrix for drawing
    t3d_mat4fp_from_srt_euler(mModelMatFP,
                              (float[3]){0.125f, 0.125f, 0.125f},
                              (float[3]){0.0f, -mRotationY, 0},
                              mPosition.v);
}

void Player::update(float deltaTime, InputState input)
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
    else if (!mIsHuman)
    {
        if (rand() % 1000 < (20 * core_get_aidifficulty() + 20))
        {
            mFishingTimer = Fish::get_new_timer();
        }
    }
}

void Player::draw(T3DViewport &viewport, const T3DVec3 &camPos) const
{
    rspq_block_run(mDplPlayer);
}

void Player::draw_billboard(T3DViewport &viewport, const T3DVec3 &camPos) const
{
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
    std::string mBillboardText = is_fishing() ? is_catchable() ? "HOOKED!" : "Fishing..." : std::to_string(mFishCaught);
    rdpq_text_printf(&param, FONT_BILLBOARD, x, y, "%s", mBillboardText.c_str());
}

void Player::get_attack_position(fm_vec2_t &attack_pos) const
{
    float s, c;
    fm_sincosf(mRotationY, &s, &c);
    attack_pos.v[0] = mPosition.v[0] + s * ATTACK_OFFSET;
    attack_pos.v[1] = mPosition.v[2] + c * ATTACK_OFFSET;
}

const T3DVec3 &Player::get_position() const
{
    return mPosition;
}

void Player::shove(const float &direction)
{
    float s, c;
    fm_sincosf(direction, &s, &c);
    mPosition.v[0] += s * SHOVE_DIST;
    mPosition.v[2] += c * SHOVE_DIST;

    mFishingTimer = 0.0f;
}