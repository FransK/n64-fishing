#include "PlayerAnimatable.h"

#include "common/Observer.h"
#include "math/Quaternion.h"

using namespace Math;

PlayerAnimatable::PlayerAnimatable(T3DModel *model, color_t primColor)
    : Observer<Player>([this](const Player &player)
                       { this->onPlayerStateChange(player); }),
      mModel(model),
      mSkeleton(model),
      mAnimIdle(model, "Idle"),
      mAnimPunch(model, "Punch"),
      mAnimReceiveHit(model, "RecieveHit"),
      mAnimRun(model, "Run"),
      mAnimCast(model, "SwordSlash"),
      mPrimColor(primColor)
{
    // Model Credits: Quaternius (CC0) https://quaternius.com/
    mAnimIdle.attach(mSkeleton);

    mAnimRun.attach(mSkeleton);
    t3d_anim_set_playing(mAnimRun.get(), false);

    mAnimPunch.attach(mSkeleton);
    t3d_anim_set_looping(mAnimPunch.get(), false);
    t3d_anim_set_playing(mAnimPunch.get(), false);
    t3d_anim_set_speed(mAnimPunch.get(), SHOVE_TIME_SCALE);

    mAnimReceiveHit.attach(mSkeleton);
    t3d_anim_set_looping(mAnimReceiveHit.get(), false);
    t3d_anim_set_playing(mAnimReceiveHit.get(), false);

    mAnimCast.attach(mSkeleton);
    t3d_anim_set_looping(mAnimCast.get(), false);
    t3d_anim_set_playing(mAnimCast.get(), false);

    mActiveAnim = mAnimIdle.get();

    rspq_block_begin();
    t3d_matrix_push(mModelMatFP.get());
    rdpq_set_prim_color(mPrimColor);
    t3d_model_draw_skinned(mModel, mSkeleton.get());
    t3d_matrix_pop(1);
    mDplPlayer = Adapters::RspqBlockAdapter(rspq_block_end());
}

void PlayerAnimatable::update(float deltaTime)
{
    t3d_anim_update(mActiveAnim, deltaTime);
    t3d_skeleton_update(mSkeleton.get());
}

void PlayerAnimatable::draw(const Vector3 &position, const Vector2 &rotation) const
{
    // Rotate the complex by 90 degrees to align with T3D's coordinate system
    Math::Vector2 adjustedRotation = {-rotation.y, rotation.x};

    Math::Quaternion mathQuat = Math::axisComplex({0.0f, 1.0f, 0.0f}, adjustedRotation);

    T3DVec3 scale = {{PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE}};
    T3DQuat quat = {{mathQuat.x, mathQuat.y, mathQuat.z, mathQuat.w}};
    T3DVec3 pos = {{position.x, position.y, position.z}};
    t3d_mat4fp_from_srt(mModelMatFP.get(),
                        scale,
                        quat,
                        pos);

    rspq_block_run(mDplPlayer.get());
}

void PlayerAnimatable::onPlayerStateChange(const Player &player)
{
    switch (player.actionState())
    {
    case PlayerStateEnum::STATE_IDLE:
        playAnimation(Anim::IDLE);
        break;
    case PlayerStateEnum::STATE_WALKING:
        playAnimation(Anim::RUN);
        break;
    case PlayerStateEnum::STATE_ATTACKING:
        playAnimation(Anim::SHOVE);
        break;
    case PlayerStateEnum::STATE_STUNNED:
        playAnimation(Anim::RECEIVE_SHOVE);
        break;
    case PlayerStateEnum::STATE_CASTING:
        playAnimation(Anim::CAST);
        break;
    case PlayerStateEnum::STATE_FISHING:
        playAnimation(Anim::IDLE);
        break;
    }
}

void PlayerAnimatable::playAnimation(Anim anim)
{
    t3d_anim_set_playing(mAnimIdle.get(), anim == Anim::IDLE);
    t3d_anim_set_playing(mAnimRun.get(), anim == Anim::RUN);
    t3d_anim_set_playing(mAnimPunch.get(), anim == Anim::SHOVE);
    t3d_anim_set_playing(mAnimReceiveHit.get(), anim == Anim::RECEIVE_SHOVE);
    t3d_anim_set_playing(mAnimCast.get(), anim == Anim::CAST);

    switch (anim)
    {
    case Anim::IDLE:
        mActiveAnim = mAnimIdle.get();
        return;
    case Anim::RUN:
        mActiveAnim = mAnimRun.get();
        return;
    case Anim::SHOVE:
        t3d_anim_set_time(mAnimPunch.get(), 0.0f);
        mActiveAnim = mAnimPunch.get();
        return;
    case Anim::RECEIVE_SHOVE:
        t3d_anim_set_time(mAnimReceiveHit.get(), 0.0f);
        mActiveAnim = mAnimReceiveHit.get();
        return;
    case Anim::CAST:
        t3d_anim_set_time(mAnimCast.get(), 0.0f);
        mActiveAnim = mAnimCast.get();
        return;
    }
}