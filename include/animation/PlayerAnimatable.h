#pragma once

#include <t3d/t3danim.h>

#include "adapters/T3d.h"
#include "common/Observer.h"
#include "scene/Player.h"

constexpr float PLAYER_SCALE = 0.125f;

class PlayerAnimatable : public Observer<Player>
{
public:
    PlayerAnimatable(T3DModel *model, color_t primColor);

    void update(float deltaTime);
    void draw(const Math::Vector3 &position, const Math::Vector2 &rotation) const;

private:
    enum struct Anim : uint8_t
    {
        IDLE = 0,
        RUN,
        SHOVE,
        RECEIVE_SHOVE,
        CAST
    };

    T3DModel *mModel{};
    Adapters::Mat4FPAdapter mModelMatFP;
    Adapters::RspqBlockAdapter mDplPlayer;

    Adapters::SkeletonAdapter mSkeleton;
    Adapters::AnimAdapter mAnimIdle;
    Adapters::AnimAdapter mAnimPunch;
    Adapters::AnimAdapter mAnimReceiveHit;
    Adapters::AnimAdapter mAnimRun;
    Adapters::AnimAdapter mAnimCast;
    T3DAnim *mActiveAnim{};

    color_t mPrimColor{};

    void onPlayerStateChange(const Player &state);
    void playAnimation(Anim anim);
};