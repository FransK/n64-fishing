#ifndef PLAYER_POSITION_H
#define PLAYER_POSITION_H

#include "math/Vector2.h"
#include "math/Vector3.h"
#include "scene/Actor.h"

using namespace Math;

class PlayerData : public Actor
{
public:
    PlayerData() : Actor() {}

    [[nodiscard]] Actor *getAttackActor() { return &mAttackActor; }

private:
    Actor mAttackActor{};
};

#endif