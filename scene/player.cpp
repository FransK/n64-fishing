#include "player.h"

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

void Player::update_fixed(float deltaTime, InputState input)
{
    playerPos.v[0] += input.move.v[0] * speed;
    playerPos.v[2] += input.move.v[2] * speed;
}

void Player::update(float deltaTime)
{
    // Update player matrix
    t3d_mat4fp_from_srt_euler(modelMatFP,
                              (float[3]){0.125f, 0.125f, 0.125f},
                              (float[3]){0.0f, 0.0f, 0},
                              playerPos.v);
}

void Player::draw()
{
    rspq_block_run(dplPlayer);
}