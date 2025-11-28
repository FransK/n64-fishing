#include <libdragon.h>
#include "../../core.h"
#include "../../minigame.h"
#include <stdio.h>
#include <t3d/t3dmodel.h>


const MinigameDef minigame_def = {
    .gamename = "Fishing",
    .developername = "Frans Kouwenhoven",
    .description = "Relax and catch some fish.",
    .instructions = "Press A to cast, then A to catch."
};

T3DViewport viewport;
T3DModel *model;
T3DMat4FP* modelMatFP;
rspq_block_t *dplPlayer;
T3DVec3 playerPos;
T3DVec3 camPos;
T3DVec3 camTarget;

float timer;

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
void minigame_init()
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);

    t3d_init((T3DInitParams){});

    viewport = t3d_viewport_create();

    camPos = (T3DVec3){{0, 125.0f, 100.0f}};
    camTarget = (T3DVec3){{0, 0, 0}};

    model = t3d_model_load("rom:/n64-fishing/player.t3dm");

    // Player Init
    playerPos = (T3DVec3){{-50, 0.5f, 0}};

    modelMatFP = malloc_uncached(sizeof(T3DMat4FP));
    rspq_block_begin();
        t3d_matrix_push(modelMatFP);
        t3d_model_draw(model);
        t3d_matrix_pop(1);
    dplPlayer = rspq_block_end();

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
    t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(90.0f), 20.0f, 160.0f);
    t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(T3DVec3){{0,1,0}});

    // ===== Player Loop ===== //
    // Update player matrix
    t3d_mat4fp_from_srt_euler(modelMatFP,
        (float[3]){0.375f, 0.375f, 0.375f},
        (float[3]){0.0f, 0.0f, 0},
        playerPos.v
    );

    // ======== Draw (3D) ======== //
    rdpq_attach(display_get(), display_get_zbuf());
    t3d_frame_start();
    t3d_viewport_attach(&viewport);

    t3d_screen_clear_color(RGBA32(224, 180, 96, 0xFF));
    t3d_screen_clear_depth();

    rspq_block_run(dplPlayer);

	rdpq_detach_show();
}

/*==============================
    minigame_cleanup
    Clean up any memory used by your game just before it ends.
==============================*/
void minigame_cleanup()
{
    // Player cleanup
    rspq_block_free(dplPlayer);
    free_uncached(modelMatFP);


    t3d_model_free(model);
    t3d_destroy();
	display_close();
}