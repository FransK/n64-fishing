#pragma once

#include <cstdint>
#include "Entity.h"
#include "Vector3.h"

class Actor : public Entity
{
public:
    [[nodiscard]] Math::Vector3 getVelocity() const { return mVelocity; }

    void setVelocity(const Math::Vector3 &vel) { mVelocity = vel; }
    void setFlag(uint32_t flag) { mFlags |= flag; }
    void clearFlag(uint32_t flag) { mFlags &= ~flag; }
    void clearAllFlags() { mFlags = 0; }
    bool hasFlag(uint32_t flag) const { return (mFlags & flag) != 0; }

private:
    Math::Vector3 mVelocity{};
    uint32_t mFlags{0};
};