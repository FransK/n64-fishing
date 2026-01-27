#ifndef GAMESETTINGSINTERFACE_H
#define GAMESETTINGSINTERFACE_H

#include <libdragon.h>
#include "Config.h"

class GameSettingsInterface
{
public:
    virtual ~GameSettingsInterface() = default;

    uint32_t core_get_playercount() { return do_get_playercount(); }
    joypad_port_t core_get_playercontroller(Core::PlyNum ply) { return do_get_playercontroller(ply); }
    Core::AiDiff core_get_aidifficulty() { return do_get_aidifficulty(); }
    bool core_get_game_ending() { return do_get_game_ending(); }
    double core_get_subtick() { return do_get_subtick(); }
    void core_set_winner(Core::PlyNum ply) { do_set_winner(ply); }
    void core_game_end() { do_game_end(); }
    void core_set_playercount(bool *enabledconts) { do_set_playercount(enabledconts); }
    void core_set_aidifficulty(Core::AiDiff difficulty) { do_set_aidifficulty(difficulty); }
    void core_set_subtick(double subtick) { do_set_subtick(subtick); }
    void core_reset_game() { do_reset_game(); }

private:
    virtual uint32_t do_get_playercount() = 0;
    virtual joypad_port_t do_get_playercontroller(Core::PlyNum ply) = 0;
    virtual Core::AiDiff do_get_aidifficulty() = 0;
    virtual bool do_get_game_ending() = 0;
    virtual double do_get_subtick() = 0;
    virtual void do_set_winner(Core::PlyNum ply) = 0;
    virtual void do_game_end() = 0;

    virtual void do_set_playercount(bool *enabledconts) = 0;
    virtual void do_set_aidifficulty(Core::AiDiff difficulty) = 0;
    virtual void do_set_subtick(double subtick) = 0;
    virtual void do_reset_game() = 0;
};

GameSettingsInterface *get_game_settings_interface();
void set_game_settings_interface(GameSettingsInterface *game_settings);

extern GameSettingsInterface *instance;

#endif