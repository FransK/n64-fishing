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

float speed;
T3DVec3 newDir;

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
    speed = 0.0f;
    newDir = (T3DVec3){{0, 0, 0}};

    modelMatFP = malloc_uncached(sizeof(T3DMat4FP));
    rspq_block_begin();
        t3d_matrix_push(modelMatFP);
        t3d_model_draw(model);
        t3d_matrix_pop(1);
    dplPlayer = rspq_block_end();
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
    playerPos.v[0] += newDir.v[0] * speed;
    playerPos.v[2] += newDir.v[2] * speed;
}

void read_inputs(float deltaTime, joypad_port_t port)
{
    joypad_inputs_t joypad = joypad_get_inputs(port);
    joypad_buttons_t btn = joypad_get_buttons_pressed(port);

    if (btn.start)
    {
        fprintf(stderr, "Start pressed\n");
    }

    if (btn.a)
    {
        fprintf(stderr, "A pressed\n");
    }

    if (btn.b)
    {
        fprintf(stderr, "B pressed\n");
    }

    if (joypad.stick_x > 0.0f || joypad.stick_x < 0.0f)
    {
        fprintf(stderr, "Joypad X: %f\n", (float)joypad.stick_x);
    }

    if (joypad.stick_y > 0.0f || joypad.stick_y < 0.0f)
    {
        fprintf(stderr, "Joypad Y: %f\n", (float)joypad.stick_y);
    }

    newDir.v[0] = (float)joypad.stick_x * 0.05f;
    newDir.v[2] = -(float)joypad.stick_y * 0.05f;
    speed = sqrtf(t3d_vec3_len2(&newDir));
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
    read_inputs(deltatime, core_get_playercontroller(0));

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

    // Display cleanup
    t3d_destroy();
	display_close();
}