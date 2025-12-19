#pragma once

#include <t3d/t3danim.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include "../main.h"

constexpr float BOX_SIZE = 100.f; // TODO: Doesn't make sense in here...

constexpr float BILLBOARD_YOFFSET = 350.0f;

constexpr float HITBOX_RADIUS = 10.f;
constexpr float ATTACK_OFFSET = 10.f;
constexpr float ATTACK_RADIUS = 10.f;

constexpr float BASE_SPEED = 3.f;
constexpr float SHOVE_DIST = 7.f;
constexpr float MIN_MOVE_INPUT = 8.f;
constexpr float ROTATION_INPUT = 26.f;

constexpr float CATCH_TIMER = 1.f;

class Player
{
private:
    T3DModel *mModel{};
    T3DMat4FP *mModelMatFP{};
    rspq_block_t *mDplPlayer{};

    T3DAnim mAnimIdle{};
    T3DSkeleton mSkeleton{};

    T3DVec3 mPosition{};
    float mRotationY{};
    float mSpeed{BASE_SPEED};
    float mFishingTimer{};
    int8_t mFishCaught{};
    int8_t mPlayerNumber{-1}; // -1 = uninitialized
    color_t mColor{};
    bool mIsHuman{};

public:
    [[nodiscard]] bool is_fishing() const { return mFishingTimer > 0.0f; };
    [[nodiscard]] bool is_catchable() const { return mFishingTimer < CATCH_TIMER && mFishingTimer > 0.0f; };
    void init(int playerNumber, T3DVec3 position, float rotation, color_t color, bool isHuman);
    void update_fixed(InputState input);
    void update(float deltaTime, InputState input);
    void draw(T3DViewport &viewport, const T3DVec3 &cameraPos) const;
    void draw_billboard(T3DViewport &viewport, const T3DVec3 &cameraPos) const;

    void get_attack_position(fm_vec2_t &attack_pos) const;
    [[nodiscard]] uint8_t get_fish_caught() const;
    const T3DVec3 &get_position() const;

    void shove(const float &direction);

    Player();
    ~Player();
};