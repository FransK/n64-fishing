#include "World.h"

World::World()
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
    t3d_init((T3DInitParams){});

    // Needs to be called after display_init
    // because it uses display_get_width/height
    mScene = std::make_unique<Scene>();
}

World::~World()
{
    display_close();
    t3d_destroy();
}

/*==============================
    reset
    Resets the game state for a new round.
==============================*/
void World::reset()
{
    mScene->reset();
}

/*==============================
    fixedloop
    Code that is called every loop, at a fixed delta time.
    Use this function for stuff where a fixed delta time is
    important, like physics.
    @param  The fixed delta time for this tick
==============================*/
void World::fixedloop(float deltatime)
{
    mScene->updateFixed(deltatime);
}

/*==============================
    loop
    Code that is called every loop.
    @param  The delta time for this tick
==============================*/
void World::loop(float deltatime)
{
    mScene->update(deltatime);
}