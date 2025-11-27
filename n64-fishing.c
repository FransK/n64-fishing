#include <libdragon.h>
#include "../../core.h"
#include "../../minigame.h"
#include <stdio.h>

const MinigameDef minigame_def = {
    .gamename = "Fishing",
    .developername = "Frans Kouwenhoven",
    .description = "Relax and catch some fish.",
    .instructions = "Press A to cast, then A to catch."
};

float timer;

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
void minigame_init()
{
    timer = 4.0f;
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
    timer -= deltatime;

    fprintf(stderr, "Time left %f\n", timer);

    if (timer <= 0.0f) {
        minigame_end();
    }
}

/*==============================
    minigame_loop
    Code that is called every loop.
    @param  The delta time for this tick
==============================*/
void minigame_loop(float deltatime)
{

}

/*==============================
    minigame_cleanup
    Clean up any memory used by your game just before it ends.
==============================*/
void minigame_cleanup()
{

}