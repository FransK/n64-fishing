#pragma once

#include <t3d/t3dmodel.h>
#include "../main.h"

class Player
{
private:
    T3DModel *model{};
    T3DMat4FP *modelMatFP{};
    rspq_block_t *dplPlayer{};
    T3DVec3 playerPos{};

    float speed{};

public:
    void update_fixed(float deltaTime, InputState input);
    void update(float deltaTime);
    void draw();

    Player();
    ~Player();
};