#pragma once

#include <cstdint>
#include "ActorFlags.h"
#include "Entity.h"
#include "Vector3.h"

class Actor : public Entity
{
public:
    [[nodiscard]] Math::Vector3 getVelocity() const { return mVelocity; }

    void setVelocity(const Math::Vector3 &vel) { mVelocity = vel; }
    void setFlag(ActorFlags flag) { mFlags |= static_cast<uint32_t>(flag); }
    void clearFlag(ActorFlags flag) { mFlags &= ~static_cast<uint32_t>(flag); }
    void clearAllFlags() { mFlags = 0; }
    bool hasFlag(ActorFlags flag) const { return (mFlags & static_cast<uint32_t>(flag)) != 0; }

private:
    Math::Vector3 mVelocity{};
    uint32_t mFlags{0};
};