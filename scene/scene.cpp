#include "scene.h"

Scene::Scene()
{
    mViewport = t3d_viewport_create();
    mCamera.position = (T3DVec3){{0, 125.0f, 100.0f}};
    mCamera.target = (T3DVec3){{0, 0, 0}};

    mLightDirVec = (T3DVec3){{1.0f, 1.0f, 1.0f}};
    t3d_vec3_norm(&mLightDirVec);
}

void Scene::read_inputs(PlyNum plyNum)
{
    joypad_port_t port = core_get_playercontroller(plyNum);
    auto btn = joypad_get_buttons_pressed(port);
    auto inputs = joypad_get_inputs(port);

    mInputState = {
        .move = {{(float)inputs.stick_x, 0, -(float)inputs.stick_y}},
        .fish = btn.a != 0,
        .attack = btn.b != 0};

    mInputState.move.v[0] = fminf(fmaxf(mInputState.move.v[0], -1.0f), 1.0f);
    mInputState.move.v[2] = fminf(fmaxf(mInputState.move.v[2], -1.0f), 1.0f);
}

void Scene::update_fixed(float deltaTime)
{
    mPlayer.update_fixed(mInputState);
}

void Scene::update(float deltaTime)
{
    // ======== Attach RDP ======== //
    rdpq_attach(display_get(), display_get_zbuf());

    // === Set Camera === //
    mCamera.update(mViewport);

    // === Draw viewport === //
    t3d_viewport_attach(&mViewport);

    // === Process Inputs === //
    read_inputs(PlyNum::PLAYER_1);

    mPlayer.update(deltaTime, mInputState);

    // === Draw Background === //
    rdpq_set_mode_fill({(uint8_t)(0x80),
                        (uint8_t)(0xb8),
                        (uint8_t)(0xd8),
                        0xFF});
    rdpq_fill_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // === Draw 3D === //
    t3d_frame_start();

    t3d_screen_clear_depth();

    t3d_light_set_ambient(FranSoft::colorAmbient);
    t3d_light_set_directional(0, FranSoft::colorDir, &mLightDirVec);
    t3d_light_set_count(1);

    // === Draw players === //
    mPlayer.draw(mViewport, mCamera.position);

    // === Detach and show === //
    rdpq_detach_show();
}