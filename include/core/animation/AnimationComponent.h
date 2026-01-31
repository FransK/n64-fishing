#pragma once

#include "math/Vector2.h"
#include "math/Vector3.h"

using namespace Math;

template <typename AnimatableT>
class AnimationComponent
{
public:
    template <typename... Args>
    explicit AnimationComponent(Args &&...args) : mAnimatable(std::forward<Args>(args)...){};

    AnimatableT &getAnimatable() { return mAnimatable; }
    uint32_t getVertCount() const { return mAnimatable.getVertCount(); }
    void update(float deltaTime) { mAnimatable.update(deltaTime); }
    void draw(const Vector3 &position, const Vector2 &rotation) const
    {
        mAnimatable.draw(position, rotation);
    }

private:
    AnimatableT mAnimatable;
};