#include "GameSettings.h"

uint32_t GameSettings::do_get_playercount()
{
    return player_count;
}

joypad_port_t GameSettings::do_get_playercontroller(Core::PlyNum ply)
{
    return player_joypads[static_cast<size_t>(ply)].port;
}

Core::AiDiff GameSettings::do_get_aidifficulty()
{
    return ai_difficulty;
}

double GameSettings::do_get_subtick()
{
    return subtick;
}

bool GameSettings::do_get_game_ending()
{
    return game_ending;
}

void GameSettings::do_set_winner(Core::PlyNum ply)
{
    player_is_winner[static_cast<size_t>(ply)] = true;
}
void GameSettings::do_game_end()
{
    game_ending = true;
}

void GameSettings::do_set_playercount(bool *enabledconts)
{
    int plynum = 0;

    // Attempt to find the first N left-most available controllers
    for (int i = 0; i < Core::MAX_PLAYERS; i++)
    {
        if (enabledconts[i])
        {
            player_joypads[plynum].port = static_cast<joypad_port_t>(i);
            plynum++;
        }
    }
    player_count = plynum;
}

void GameSettings::do_set_aidifficulty(Core::AiDiff difficulty)
{
    ai_difficulty = difficulty;
}

void GameSettings::do_set_subtick(double subtick)
{
    this->subtick = subtick;
}

void GameSettings::do_reset_game()
{
    game_ending = false;
    player_is_winner.fill(false);
}