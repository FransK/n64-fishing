#include "scene.h"

Scene::Scene()
{
    viewport = t3d_viewport_create();
    cam.pos = (T3DVec3){{0, 125.0f, 100.0f}};
    cam.target = (T3DVec3){{0, 0, 0}};

    lightDirVec = (T3DVec3){{1.0f, 1.0f, 1.0f}};
    t3d_vec3_norm(&lightDirVec);
}

void Scene::read_inputs(PlyNum plyNum)
{
    joypad_port_t port = core_get_playercontroller(plyNum);
    auto btn = joypad_get_buttons_pressed(port);
    auto stick = joypad_get_inputs(port);

    inputState = {
        .move = {{(float)stick.stick_x, 0, -(float)stick.stick_y}},
        .fish = btn.a != 0,
        .attack = btn.b != 0};

    inputState.move.v[0] = fminf(fmaxf(inputState.move.v[0], -1.0f), 1.0f);
    inputState.move.v[2] = fminf(fmaxf(inputState.move.v[2], -1.0f), 1.0f);
}

void Scene::update_fixed(float deltaTime)
{
    player.update_fixed(deltaTime, inputState);
}

void Scene::update(float deltaTime)
{
    cam.update(viewport);

    read_inputs(PlyNum::PLAYER_1);

    player.update(deltaTime);

    // ======== Draw (3D) ======== //
    rdpq_attach(display_get(), display_get_zbuf());
    t3d_frame_start();
    t3d_viewport_attach(&viewport);

    t3d_screen_clear_color(RGBA32(224, 180, 96, 0xFF));
    t3d_screen_clear_depth();

    t3d_light_set_ambient(FranSoft::colorAmbient);
    t3d_light_set_directional(0, FranSoft::colorDir, &lightDirVec);
    t3d_light_set_count(1);

    player.draw();

    rdpq_detach_show();
}