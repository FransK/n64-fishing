#pragma once

#include <t3d/t3dmodel.h>
#include "../main.h"

namespace
{
    constexpr int FONT_BILLBOARD = 1;
    constexpr float BILLBOARD_YOFFSET = 350.0f;
}

class Player
{
private:
    T3DModel *model{};
    T3DMat4FP *modelMatFP{};
    rspq_block_t *dplPlayer{};
    rdpq_font_t *fontBillboard;

    T3DVec3 playerPos{};

    float speed{};
    float fishingTimer{};
    int fishCaught{};

    void draw_billboard(T3DViewport &viewport, const T3DVec3 &camPos) const;

public:
    [[nodiscard]] bool isFishing() { return fishingTimer > 0.0f; };
    [[nodiscard]] bool isCatchable() { return fishingTimer < 1.0f && fishingTimer > 0.0f; };
    void update_fixed(InputState input);
    void update(float deltaTime, InputState input);
    void draw(T3DViewport &viewport, const T3DVec3 &cameraPos) const;

    Player();
    ~Player();
};