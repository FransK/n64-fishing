#pragma once

#include <array>

#include "GameSettingsInterface.h"

class GameSettings : public GameSettingsInterface
{
public:
    GameSettings() = default;

    GameSettings(const GameSettings &) = delete;
    GameSettings(GameSettings &&) = delete;
    GameSettings &operator=(const GameSettings &) = delete;
    GameSettings &operator=(GameSettings &&) = delete;

private:
    uint32_t doGetPlayercount() override;
    joypad_port_t doGetPlayercontroller(Core::PlyNum ply) override;
    Core::AiDiff doGetAidifficulty() override;
    bool doGetGameEnding() override;
    double doGetSubtick() override;
    void doSetWinner(Core::PlyNum ply) override;
    void doGameEnd() override;

    void doSetPlayercount(bool *enabledconts) override;
    void doSetAidifficulty(Core::AiDiff difficulty) override;
    void doSetSubtick(double subtick) override;
    void doResetGame() override;

    std::array<Core::PlayerJoypad, JOYPAD_PORT_COUNT> mPlayerJoypads;
    std::array<bool, Core::MAX_PLAYERS> mPlayerIsWinner;
    Core::AiDiff mAiDifficulty;
    uint32_t mPlayerCount;
    bool mGameEnding;

    // Core info
    double mSubtick;
};
