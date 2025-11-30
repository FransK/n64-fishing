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
    float fishingTimer{};
    int fishCaught{};

public:
    [[nodiscard]] bool isFishing() { return fishingTimer > 0.0f; };
    [[nodiscard]] bool isCatchable() { return fishingTimer < 1.0f && fishingTimer > 0.0f; };
    void update_fixed(InputState input);
    void update(float deltaTime, InputState input);
    void draw();

    Player();
    ~Player();
};