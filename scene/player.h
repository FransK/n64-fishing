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
    T3DModel *mModel{};
    T3DMat4FP *mModelMatFP{};
    rspq_block_t *mDplPlayer{};
    rdpq_font_t *mFontBillboard{};

    T3DVec3 mPosition{};
    float mSpeed{};
    float mFishingTimer{};
    int mFishCaught{};

    void draw_billboard(T3DViewport &viewport, const T3DVec3 &camPos) const;

public:
    [[nodiscard]] bool is_fishing() { return mFishingTimer > 0.0f; };
    [[nodiscard]] bool is_catchable() { return mFishingTimer < 1.0f && mFishingTimer > 0.0f; };
    void update_fixed(InputState input);
    void update(float deltaTime, InputState input);
    void draw(T3DViewport &viewport, const T3DVec3 &cameraPos) const;

    Player();
    ~Player();
};