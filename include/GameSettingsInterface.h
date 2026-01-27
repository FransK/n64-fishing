#pragma once

#include <libdragon.h>
#include "Config.h"

class GameSettingsInterface
{
public:
    virtual ~GameSettingsInterface() = default;

    uint32_t coreGetPlayercount() { return doGetPlayercount(); }
    joypad_port_t coreGetPlayercontroller(Core::PlyNum ply) { return doGetPlayercontroller(ply); }
    Core::AiDiff coreGetAidifficulty() { return doGetAidifficulty(); }
    bool coreGetGameEnding() { return doGetGameEnding(); }
    double coreGetSubtick() { return doGetSubtick(); }
    void coreSetWinner(Core::PlyNum ply) { doSetWinner(ply); }
    void coreGameEnd() { doGameEnd(); }
    void coreSetPlayercount(bool *enabledConts) { doSetPlayercount(enabledConts); }
    void coreSetAidifficulty(Core::AiDiff difficulty) { doSetAidifficulty(difficulty); }
    void coreSetSubtick(double subtick) { doSetSubtick(subtick); }
    void coreResetGame() { doResetGame(); }

private:
    virtual uint32_t doGetPlayercount() = 0;
    virtual joypad_port_t doGetPlayercontroller(Core::PlyNum ply) = 0;
    virtual Core::AiDiff doGetAidifficulty() = 0;
    virtual bool doGetGameEnding() = 0;
    virtual double doGetSubtick() = 0;
    virtual void doSetWinner(Core::PlyNum ply) = 0;
    virtual void doGameEnd() = 0;

    virtual void doSetPlayercount(bool *enabledConts) = 0;
    virtual void doSetAidifficulty(Core::AiDiff difficulty) = 0;
    virtual void doSetSubtick(double subtick) = 0;
    virtual void doResetGame() = 0;
};

GameSettingsInterface *getGameSettingsInterface();
void setGameSettingsInterface(GameSettingsInterface *gameSettings);

extern GameSettingsInterface *instance;
