#include "player.h"
#include "fish.h"

Player::Player()
{
    mModel = t3d_model_load("rom:/n64-fishing/player.t3dm");

    mSpeed = 3.0f;

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

void Player::init(int playerNumber, T3DVec3 position, float rotation, color_t color)
{
    assert(playerNumber >= 0 && playerNumber < MAXPLAYERS);

    mPlayerNumber = playerNumber;
    mColor = color;
    mPosition = position;
    mRotationY = rotation;
}

void Player::update_fixed(InputState input)
{
    assert(mPlayerNumber != -1 && "Player needs to be initialized before update.");

    if (!is_fishing())
    {
        t3d_vec3_norm(input.move);
        mPosition.v[0] += input.move.v[0] * mSpeed;
        mPosition.v[2] += input.move.v[2] * mSpeed;
        mRotationY = atan2f(mPosition.v[0], mPosition.v[2]);
    }

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
        fprintf(stderr, "Fishing Timer: %f\n", mFishingTimer);
    }
    else if (input.fish)
    {
        mFishingTimer = Fish::get_new_timer();
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
    rdpq_text_printf(&param, FONT_BILLBOARD, x, y, "%d", mFishCaught);
}

void Player::get_attack_direction(float (&attack_dir)[2]) const
{
    float s, c;
    fm_sincosf(mRotationY, &s, &c);
    attack_dir[0] = mPosition.v[0] + s * ATTACK_OFFSET;
    attack_dir[1] = mPosition.v[2] + c * ATTACK_OFFSET;
}

const T3DVec3 &Player::get_position() const
{
    return mPosition;
}

void Player::shove(float (&direction)[2])
{
    mPosition.v[0] += direction[0] * mSpeed * 2.0;
    mPosition.v[2] += direction[1] * mSpeed * 2.0;
}