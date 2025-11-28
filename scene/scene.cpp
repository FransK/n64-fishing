#include "scene.h"

Scene::Scene()
{
    viewport = t3d_viewport_create();
    cam.pos = (T3DVec3){{0, 125.0f, 100.0f}};
    cam.target = (T3DVec3){{0, 0, 0}};

    inputState = {
        .newDir = (T3DVec3){{0, 0, 0}},
        .speed = 0.0f};
}

void Scene::read_inputs(float deltaTime, joypad_port_t port)
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

    T3DVec3 dir{};
    dir.v[0] = (float)joypad.stick_x * 0.05f;
    dir.v[2] = -(float)joypad.stick_y * 0.05f;

    inputState = {
        .newDir = dir,
        .speed = sqrtf(t3d_vec3_len2(&dir))};
}

void Scene::update_fixed(float deltaTime)
{
    player.update_fixed(deltaTime, inputState);
}

void Scene::update(float deltaTime)
{
    cam.update(viewport);

    read_inputs(deltaTime, core_get_playercontroller((PlyNum)0));

    player.update(deltaTime);

    // ======== Draw (3D) ======== //
    rdpq_attach(display_get(), display_get_zbuf());
    t3d_frame_start();
    t3d_viewport_attach(&viewport);

    t3d_screen_clear_color(RGBA32(224, 180, 96, 0xFF));
    t3d_screen_clear_depth();

    player.draw();

    rdpq_detach_show();
}