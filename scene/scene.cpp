#include <algorithm>
#include <string>
#include "scene.h"

Scene::Scene()
{
    mFontBillboard = rdpq_font_load("rom:/squarewave.font64");
    rdpq_text_register_font(FONT_BILLBOARD, mFontBillboard);
    mFontText = rdpq_font_load("rom:/squarewave.font64");
    rdpq_text_register_font(FONT_TEXT, mFontText);

    // === Setup viewport and lighting ==== //
    mViewport = t3d_viewport_create();
    mCamera.position = (T3DVec3){{0, 125.0f, 100.0f}};
    mCamera.target = (T3DVec3){{0, 0, 40}};

    mLightDirVec = (T3DVec3){{1.0f, 1.0f, 1.0f}};
    t3d_vec3_norm(&mLightDirVec);

    // === Initialize the players === //
    T3DVec3 startPositions[] = {
        (T3DVec3){{-100, 0.15f, 0}},
        (T3DVec3){{0, 0.15f, -100}},
        (T3DVec3){{100, 0.15f, 0}},
        (T3DVec3){{0, 0.15f, 100}},
    };

    float startRotations[] = {
        M_PI / 2,
        0,
        3 * M_PI / 2,
        M_PI};

    for (size_t i = 0; i < MAXPLAYERS; i++)
    {
        mPlayers[i].init(i, startPositions[i], startRotations[i], COLORS[i], i < core_get_playercount());
    }

    mState = State::INTRO;
    mStateTime = INTRO_TIME;
}

Scene::~Scene()
{
    rdpq_text_unregister_font(FONT_TEXT);
    rdpq_font_free(mFontText);
    rdpq_text_unregister_font(FONT_BILLBOARD);
    rdpq_font_free(mFontBillboard);
}

void Scene::read_inputs(PlyNum plyNum)
{
    joypad_port_t port = core_get_playercontroller(plyNum);
    auto btn = joypad_get_buttons_pressed(port);
    auto inputs = joypad_get_inputs(port);

    mInputState[plyNum] = {
        .move = {{(float)inputs.stick_x, 0, -(float)inputs.stick_y}},
        .fish = btn.a != 0,
        .attack = btn.b != 0};

    if (mInputState[plyNum].attack)
    {
        process_attacks(plyNum);
    }
}

void Scene::process_attacks(PlyNum attacker)
{
    for (size_t i = 0; i < MAXPLAYERS; i++)
    {
        if (attacker == i)
        {
            continue;
        }

        fm_vec2_t attack_pos{};
        mPlayers[attacker].get_attack_position(attack_pos);

        float pos_diff[] = {
            mPlayers[i].get_position().v[0] - attack_pos.v[0],
            mPlayers[i].get_position().v[2] - attack_pos.v[1],
        };

        float square_distance = pos_diff[0] * pos_diff[0] + pos_diff[1] * pos_diff[1];

        fprintf(stderr, "Player %d attacking. Distance to %d: %f\n", attacker, i, square_distance);

        if (square_distance < powf((ATTACK_RADIUS + HITBOX_RADIUS), 2))
        {
            float direction = atan2f(pos_diff[0], pos_diff[1]);
            mPlayers[i].shove(direction);
        }
    }
}

void Scene::update_fixed(float deltaTime)
{
    // === Update State === //
    mStateTime -= deltaTime;
    switch (mState)
    {
    case State::INTRO:
        if (mStateTime <= 0)
        {
            mState = State::GAME;
            mStateTime = GAME_TIME;
        }
        return;
    case State::GAME:
        if (mStateTime <= 0)
        {
            mState = State::GAME_OVER;
            mStateTime = GAME_OVER_TIME;
        }
        break;
    case State::GAME_OVER:
        if (mStateTime <= 0)
        {
            minigame_end();
        }
        return;
    default:
        return;
    }

    // === Update Fixed Players === //
    for (size_t i = 0; i < MAXPLAYERS; i++)
    {
        mPlayers[i].update_fixed(mInputState[i]);
    }

    // === Keep Track of Leader === //
    mCurrTopScore = 0;
    for (auto &p : mPlayers)
    {
        mCurrTopScore = std::max(mCurrTopScore, p.get_fish_caught());
    }
}

void Scene::update(float deltaTime)
{
    // === Attach RDP === //
    rdpq_attach(display_get(), display_get_zbuf());

    // === Set Camera === //
    mCamera.update(mViewport);

    // === Draw Viewport === //
    t3d_viewport_attach(&mViewport);

    // === Process Inputs === //
    for (size_t i = 0; i < core_get_playercount(); i++)
    {
        read_inputs((PlyNum)i);
    }

    // === Update Players === //
    if (mState == State::GAME)
    {
        for (size_t i = 0; i < MAXPLAYERS; i++)
        {
            mPlayers[i].update(deltaTime, mInputState[i]);
        }
    }

    // === Draw Background === //
    rdpq_set_mode_fill({(uint8_t)(0x80),
                        (uint8_t)(0xb8),
                        (uint8_t)(0xd8),
                        0xFF});
    rdpq_fill_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // === Draw 3D === //
    t3d_frame_start();

    t3d_screen_clear_depth();

    t3d_light_set_ambient(COLOR_AMBIENT);
    t3d_light_set_directional(0, COLOR_DIR, &mLightDirVec);
    t3d_light_set_count(1);

    // === Draw players (3D Pass) === //
    for (size_t i = 0; i < MAXPLAYERS; i++)
    {
        mPlayers[i].draw(mViewport, mCamera.position);
    }

    // === Draw billboards (2D Pass) === //
    for (size_t i = 0; i < MAXPLAYERS; i++)
    {
        mPlayers[i].draw_billboard(mViewport, mCamera.position);
    }

    // === Draw UI === //
    const rdpq_textparms_t center_text_h{
        .width = (int16_t)display_get_width(),
        .align = ALIGN_CENTER,
    };
    rdpq_text_printf(&center_text_h, FONT_TEXT, 0, TIMER_Y, "%d", (int)ceilf(mStateTime));

    const rdpq_textparms_t score_param{};
    for (int i = 0; i < MAXPLAYERS; i++)
    {
        rdpq_text_printf(&score_param, FONT_TEXT, SCORE_X + i * SCORE_X_SPACING, SCORE_Y, "%d", mPlayers[i].get_fish_caught());
    }

    if (mState == State::GAME_OVER)
    {
        const rdpq_textparms_t center_text_hv{
            .width = (int16_t)display_get_width(),
            .height = (int16_t)display_get_height(),
            .align = ALIGN_CENTER,
            .valign = VALIGN_CENTER,
        };
        std::string message{};
        for (int i = 0; i < MAXPLAYERS; i++)
        {
            mWinners[i] = mPlayers[i].get_fish_caught() >= mCurrTopScore;
            if (mWinners[i])
            {
                core_set_winner((PlyNum)i);
                message += ("Player " + std::to_string(i + 1) + " wins!\n");
            }
        }
        rdpq_text_printf(&center_text_hv, FONT_TEXT, 0, 0, message.c_str());
    }

    // === Detach and show === //
    rdpq_detach_show();
}