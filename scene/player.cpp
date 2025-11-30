#include "player.h"
#include "fish.h"

Player::Player()
{
    model = t3d_model_load("rom:/n64-fishing/player.t3dm");

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

void Player::draw()
{
    rspq_block_run(dplPlayer);
}