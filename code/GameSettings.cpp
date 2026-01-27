#include "GameSettings.h"

uint32_t GameSettings::doGetPlayercount()
{
    return mPlayerCount;
}

joypad_port_t GameSettings::doGetPlayercontroller(Core::PlyNum ply)
{
    return mPlayerJoypads[static_cast<size_t>(ply)].port;
}

Core::AiDiff GameSettings::doGetAidifficulty()
{
    return mAiDifficulty;
}

double GameSettings::doGetSubtick()
{
    return mSubtick;
}

bool GameSettings::doGetGameEnding()
{
    return mGameEnding;
}

void GameSettings::doSetWinner(Core::PlyNum ply)
{
    mPlayerIsWinner[static_cast<size_t>(ply)] = true;
}
void GameSettings::doGameEnd()
{
    mGameEnding = true;
}

void GameSettings::doSetPlayercount(bool *enabledconts)
{
    int plynum = 0;

    // Attempt to find the first N left-most available controllers
    for (int i = 0; i < Core::MAX_PLAYERS; i++)
    {
        if (enabledconts[i])
        {
            mPlayerJoypads[plynum].port = static_cast<joypad_port_t>(i);
            plynum++;
        }
    }
    mPlayerCount = plynum;
}

void GameSettings::doSetAidifficulty(Core::AiDiff difficulty)
{
    mAiDifficulty = difficulty;
}

void GameSettings::doSetSubtick(double subtick)
{
    mSubtick = subtick;
}

void GameSettings::doResetGame()
{
    mGameEnding = false;
    mPlayerIsWinner.fill(false);
}
