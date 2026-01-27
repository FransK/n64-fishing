#pragma once

#include <t3d/t3danim.h>

#include "adapters/T3d.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "PlayerState.h"

#include "Anim.h"

using namespace Math;

/*
Animation Component
-
Encapsulates both animation and rendering
*/
class AnimationComponent : public Observer<PlayerState>
{
public:
    AnimationComponent(T3DModel *model, PlayerState *playerState, color_t primColor);
    ~AnimationComponent() = default;

    AnimationComponent(const AnimationComponent &) = delete;
    AnimationComponent &operator=(const AnimationComponent &) = delete;
    AnimationComponent(AnimationComponent &&) = default;
    AnimationComponent &operator=(AnimationComponent &&) = default;

    uint32_t get_vert_count() const { return mModel->totalVertCount; }
    void update(float deltaTime);
    void draw(const Vector3 &position, const Vector2 &rotation) const;

private:
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

    void on_player_state_change(const PlayerState &state);
    void play_animation(Anim anim);
};

void update(AnimationComponent &comp, float deltaTime);
void draw(const AnimationComponent &comp, const Vector3 &position, const Vector2 &rotation);