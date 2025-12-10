#pragma once

#include <t3d/t3dmodel.h>
#include "../main.h"

namespace
{
    constexpr float BILLBOARD_YOFFSET = 350.0f;
}

constexpr float HITBOX_RADIUS = 10.f;
constexpr float ATTACK_OFFSET = 10.f;
constexpr float ATTACK_RADIUS = 10.f;

constexpr float BASE_SPEED = 3.f;
constexpr float SHOVE_DIST = 7.f;

class Player
{
private:
    T3DModel *mModel{};
    T3DMat4FP *mModelMatFP{};
    rspq_block_t *mDplPlayer{};

    T3DVec3 mPosition{};
    float mRotationY{};
    float mSpeed{BASE_SPEED};
    float mFishingTimer{};
    int mFishCaught{};
    int mPlayerNumber{-1}; // -1 = uninitialized
    color_t mColor{};

public:
    [[nodiscard]] bool is_fishing() { return mFishingTimer > 0.0f; };
    [[nodiscard]] bool is_catchable() { return mFishingTimer < 1.0f && mFishingTimer > 0.0f; };
    void init(int playerNumber, T3DVec3 position, float rotation, color_t color);
    void update_fixed(InputState input);
    void update(float deltaTime, InputState input);
    void draw(T3DViewport &viewport, const T3DVec3 &cameraPos) const;
    void draw_billboard(T3DViewport &viewport, const T3DVec3 &cameraPos) const;

    void get_attack_position(fm_vec2_t &attack_pos) const;
    const T3DVec3 &get_position() const;

    void shove(const float &direction);

    Player();
    ~Player();
};