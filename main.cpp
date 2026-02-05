/***************************************************************
                             main.cpp

The ROM entrypoint, which initializes the game core
and provides a basic game loop.
***************************************************************/

#include <libdragon.h>
#include <rspq_profile.h>
#include <memory>
#include "include/Config.h"
#include "GameSettings.h"
#include "GlobalSettingsInterface.h"
#include "World.h"

/*==============================
    main
    The program main
==============================*/

int main()
{
    if constexpr (Core::DEBUG_LOG)
    {
        debug_init_isviewer();
        debug_init_usblog();
    }

    // Initialize most subsystems
    asset_init_compression(2);
    asset_init_compression(3);
    wav64_init_compression(3);
    dfs_init(DFS_DEFAULT_LOCATION);

    joypad_init();
    timer_init();
    rdpq_init();
    audio_init(32000, 3);
    mixer_init(32);

    // Enable RDP debugging
    if constexpr (Core::DEBUG_RDP)
    {
        rdpq_debug_start();
        rdpq_debug_log(true);
        rspq_profile_start();
    }

    // Initialize the random number generator, then call rand() every
    // frame so to get random behavior also in emulators.
    uint32_t seed;
    getentropy(&seed, sizeof(seed));
    srand(seed);
    register_VI_handler((void (*)(void))rand);

    // Load game
    GlobalSettingsInterface *globalSettings = getGlobalSettingsInterface();

    bool joinedPlayers[Core::MAX_PLAYERS] = {true, false, false, false};
    std::array<Core::PlayerJoypad, JOYPAD_PORT_COUNT> playerJoypads{};
    size_t playerCount = setPlayers(joinedPlayers, playerJoypads);

    globalSettings->setGlobalSettingValue(static_cast<size_t>(GameSettingsKeys::PLAYER_COUNT), playerCount);
    globalSettings->setGlobalSettingValue(static_cast<size_t>(GameSettingsKeys::PLAYER_JOYPADS), playerJoypads);
    globalSettings->setGlobalSettingValue(static_cast<size_t>(GameSettingsKeys::AI_DIFFICULTY), Core::AiDiff::DIFF_EASY);
    globalSettings->setGlobalSettingValue(static_cast<size_t>(GameSettingsKeys::SUBTICK), 0.0);

    std::unique_ptr<World> world = std::make_unique<World>();

    while (1)
    {
        world->loadNextScene();

        resetGameSettings(globalSettings);

        float accumulator = 0;
        const float dt = Core::DELTA_TIME;

        // Program loop
        while (!globalSettings->getGlobalSettingValue(static_cast<size_t>(GameSettingsKeys::EXIT_GAME)).has_value())
        {
            float frametime = display_get_delta_time();

            // In order to prevent problems if the game slows down significantly, we will clamp the maximum timestep the simulation can take
            if (frametime > 0.25f)
                frametime = 0.25f;

            // Perform the update in discrete steps (ticks)
            accumulator += frametime;
            while (accumulator >= dt)
            {
                world->fixedloop(dt);
                accumulator -= dt;
            }

            // Read controler data
            joypad_poll();
            mixer_try_play();

            // Unfixed loop
            globalSettings->setGlobalSettingValue(static_cast<size_t>(GameSettingsKeys::SUBTICK), accumulator / dt);
            world->loop(frametime);
        }

        world->reset();
        world->unloadCurrentScene();
    }
}