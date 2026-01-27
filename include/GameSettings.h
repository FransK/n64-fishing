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
    uint32_t do_get_playercount() override;
    joypad_port_t do_get_playercontroller(Core::PlyNum ply) override;
    Core::AiDiff do_get_aidifficulty() override;
    bool do_get_game_ending() override;
    double do_get_subtick() override;
    void do_set_winner(Core::PlyNum ply) override;
    void do_game_end() override;

    void do_set_playercount(bool *enabledconts) override;
    void do_set_aidifficulty(Core::AiDiff difficulty) override;
    void do_set_subtick(double subtick) override;
    void do_reset_game() override;

    std::array<Core::PlayerJoypad, JOYPAD_PORT_COUNT> player_joypads;
    std::array<bool, Core::MAX_PLAYERS> player_is_winner;
    Core::AiDiff ai_difficulty;
    uint32_t player_count;
    bool game_ending;

    // Core info
    double subtick;
};