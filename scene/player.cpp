#include "player.h"
#include "fish.h"

Player::Player()
{
    model = t3d_model_load("rom:/n64-fishing/player.t3dm");

    fontBillboard = rdpq_font_load("rom:/squarewave.font64");
    rdpq_text_register_font(FONT_BILLBOARD, fontBillboard);

    playerPos = (T3DVec3){{-50, 0.5f, 0}};
    speed = 3.0f;

    modelMatFP = (T3DMat4FP *)malloc_uncached(sizeof(T3DMat4FP));

    rspq_block_begin();
    t3d_matrix_push(modelMatFP);
    t3d_model_draw(model);
    t3d_matrix_pop(1);
    dplPlayer = rspq_block_end();
}

Player::~Player()
{
    // Player cleanup
    rspq_block_free(dplPlayer);
    free_uncached(modelMatFP);
    t3d_model_free(model);
}

void Player::update_fixed(InputState input)
{
    if (!isFishing())
    {
        t3d_vec3_norm(input.move);
        playerPos.v[0] += input.move.v[0] * speed;
        playerPos.v[2] += input.move.v[2] * speed;
    }

    // Update player matrix for drawing
    t3d_mat4fp_from_srt_euler(modelMatFP,
                              (float[3]){0.125f, 0.125f, 0.125f},
                              (float[3]){0.0f, 0.0f, 0},
                              playerPos.v);
}

void Player::update(float deltaTime, InputState input)
{
    if (isFishing())
    {
        fishingTimer -= deltaTime;

        if (input.fish)
        {
            if (isCatchable())
            {
                fishCaught++;
                fprintf(stderr, "Caught fish: %i\n", fishCaught);
            }
            else
            {
                fprintf(stderr, "Missed a fish!\n");
            }
            fishingTimer = 0.0f;
        }
        fprintf(stderr, "Fishing Timer: %f\n", fishingTimer);
    }
    else if (input.fish)
    {
        fishingTimer = Fish::get_new_timer();
    }
}

void Player::draw(T3DViewport &viewport, const T3DVec3 &camPos) const
{
    rspq_block_run(dplPlayer);
    draw_billboard(viewport, camPos);
}

void Player::draw_billboard(T3DViewport &viewport, const T3DVec3 &camPos) const
{
    // World position
    T3DVec3 localPos = (T3DVec3){{0.0f, BILLBOARD_YOFFSET, 0.0f}};

    // Screen position
    T3DVec3 worldPos = (T3DVec3){{
        playerPos.v[0] + localPos.v[0] * 0.125f,
        playerPos.v[1] + localPos.v[1] * 0.125f,
        playerPos.v[2] + localPos.v[2] * 0.125f,
    }};

    T3DVec3 screenPos;
    t3d_viewport_calc_viewspace_pos(viewport, screenPos, worldPos);

    int x = floorf(screenPos.v[0]);
    int y = floorf(screenPos.v[1]);

    const rdpq_textparms_t param{};
    rdpq_text_printf(&param, FONT_BILLBOARD, x, y, "%d", fishCaught);
}