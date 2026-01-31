#pragma once

#include "Player.h"
#include "PlayerInputState.h"

class PlayerUpdateStrategy
{
public:
    explicit PlayerUpdateStrategy(Player &player) : mPlayer(player) {}

    void update(const PlayerInputState &inputState, float deltaTime);

private:
    Player &mPlayer;
};
