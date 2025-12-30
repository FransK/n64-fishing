#include "main.h"
#include "scene/scene.h"

namespace
{
    Fishing::Scene *scene{};
}

extern "C"
{
#include "../../core.h"
#include "../../minigame.h"

    MinigameDef minigame_def = {
        .gamename = "Fishing",
        .developername = "Frans Kouwenhoven",
        .description = "Relax and catch some fish.",
        .instructions = "Press A to cast, then A to catch."};

    /*==============================
        minigame_init
        The minigame initialization function
      ==============================*/
    void minigame_init()
    {
        display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
        t3d_init((T3DInitParams){});

        scene = new Fishing::Scene();
    }

    /*==============================
        minigame_fixedloop
        Code that is called every loop, at a fixed delta time.
        Use this function for stuff where a fixed delta time is
        important, like physics.
        @param  The fixed delta time for this tick
    ==============================*/
    void minigame_fixedloop(float deltatime)
    {
        scene->update_fixed(deltatime);
    }

    /*==============================
        minigame_loop
        Code that is called every loop.
        @param  The delta time for this tick
    ==============================*/
    void minigame_loop(float deltatime)
    {
        scene->update(deltatime);
    }

    /*==============================
        minigame_cleanup
        Clean up any memory used by your game just before it ends.
    ==============================*/
    void minigame_cleanup()
    {
        delete scene;

        // Display cleanup
        t3d_destroy();
        display_close();
    }
}