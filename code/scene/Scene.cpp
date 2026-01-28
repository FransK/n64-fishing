#include <algorithm>
#include <string>
#include <variant>
#include "timer.h"

#include "ActorFlags.h"
#include "GameSettings.h"
#include "GlobalSettingsInterface.h"

#include "Scene.h"
#include "Config.h"
#include "debug/DebugDraw.h"
#include "debug/Overlay.h"
#include "input/AIInputStrategy.h"
#include "input/PlayerInputStrategy.h"
#include "input/InputComponentUpdate.h"
#include "math/Vector2.h"

bool showFPS = false;
bool debugOverlay = false;

constexpr std::string_view FS_BASE = "rom:/";

const std::string playerPath = std::string(FS_BASE) + "player3.t3dm";
const std::string mapPath = std::string(FS_BASE) + "map.t3dm";

Scene::Scene()
    : mPlayerModel(playerPath),
      mMapModel(mapPath),
      mFontBillboard(FS_BASE_PATH "squarewave.font64", Core::FONT_BILLBOARD),
      mFontText(FS_BASE_PATH "squarewave.font64", Core::FONT_TEXT)
{
    Debug::init();

    t3d_mat4fp_from_srt_euler(mMapMatFP.get(), (float[3]){PLAYER_SCALE, 1.0f, PLAYER_SCALE}, (float[3]){0, 0, 0}, (float[3]){0, 0, 0});

    rspq_block_begin();
    t3d_matrix_push(mMapMatFP.get());
    t3d_model_draw(mMapModel.getModel());
    t3d_matrix_pop(1);
    mDplMap = Adapters::RspqBlockAdapter(rspq_block_end());

    for (size_t i = 0; i < Core::MAX_PLAYERS; i++)
    {
        const rdpq_fontstyle_t style{.color = COLORS[i]};
        rdpq_font_style(mFontText.get(), i, &style);
    }
    const rdpq_fontstyle_t countdown_style{.color = RGBA32(255, 255, 255, 255)};
    rdpq_font_style(mFontText.get(), 4, &countdown_style);

    // === Setup viewport and lighting ==== //
    mViewport = t3d_viewport_create();
    mCamera.position = (T3DVec3){{0, 100.0f, 75.0f}};
    mCamera.target = (T3DVec3){{0, 0, 30}};

    mLightDirVec = (T3DVec3){{1.0f, 1.0f, 1.0f}};
    t3d_vec3_norm(&mLightDirVec);

    // === Initialize the players and components === //
    Vector3 initialPositions[Core::MAX_PLAYERS] = {
        {-25, 0.0f, 0},
        {0, 0.0f, -25},
        {25, 0.0f, 0},
        {0, 0.0f, 25}};
    Vector2 initialRotations[Core::MAX_PLAYERS] = {
        {1, 0},
        {0, -1},
        {-1, 0},
        {0, 1}};

    mInputComponents.reserve(Core::MAX_PLAYERS);
    mAnimationComponents.reserve(Core::MAX_PLAYERS);
    mPlayers.reserve(Core::MAX_PLAYERS);
    mAIPlayers.reserve(Core::MAX_PLAYERS);
    mWinners.reserve(Core::MAX_PLAYERS);

    GlobalSettingsInterface *globalSettings = getGlobalSettingsInterface();
    std::any playerCountAny = globalSettings->getGlobalSettingValue(static_cast<size_t>(GameSettingsKeys::PLAYER_COUNT));
    size_t playerCount = std::any_cast<size_t>(playerCountAny);

    for (size_t i = 0; i < Core::MAX_PLAYERS; i++)
    {
        mPlayers.emplace_back(&mCollisionScene, &mPlayerStates[i], i);
        mPlayers.back().setPosition(initialPositions[i]);
        mPlayers.back().setRotation(initialRotations[i]);

        AIBehavior behavior = (i == Core::MAX_PLAYERS - 1) ? AIBehavior::BEHAVE_BULLY : AIBehavior::BEHAVE_FISHERMAN;
        mAIPlayers.emplace_back(&mPlayers.back());
        mAIPlayers.back().setBehavior(behavior);

        if (i < playerCount)
        {
            mInputComponents.emplace_back(PlayerInputStrategy((joypad_port_t)i));
        }
        else
        {
            mInputComponents.emplace_back(AIInputStrategy(&mAIPlayers[i]));
        }

        mAnimationComponents.emplace_back(mPlayerModel.getModel(), &mPlayerStates[i], COLORS[i]);
        mPlayerStates[i].attach(&mAnimationComponents.back());
        mWinners.push_back(false);
    }

    // === Initialize Game State === //
    mState = State::INTRO;
    mStateTime = INTRO_TIME;
}

void Scene::updateFixed(float deltaTime)
{
    // === Update Game State === //
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
            getGlobalSettingsInterface()->setGlobalSettingValue(static_cast<size_t>(GameSettingsKeys::EXIT_GAME), true);
        }
        return;
    default:
        return;
    }

    // === Update Inputs and AI === //
    ticksActorUpdate = get_ticks();

    std::any playerCountAny = getGlobalSettingsInterface()->getGlobalSettingValue(static_cast<size_t>(GameSettingsKeys::PLAYER_COUNT));
    size_t playerCount = std::any_cast<size_t>(playerCountAny);
    for (size_t i = playerCount; i < Core::MAX_PLAYERS; i++)
    {
        mAIPlayers[i].update(deltaTime, &mPlayers.front(), mWinners);
    }

    for (size_t i = 0; i < Core::MAX_PLAYERS; i++)
    {
        std::visit(InputComponentUpdate{deltaTime,
                                        mPlayers[i],
                                        mCollisionScene},
                   mInputComponents[i]);
    }

    ticksCollisionUpdate = get_ticks();
    ticksActorUpdate = ticksCollisionUpdate - ticksActorUpdate;

    // === Update Collision Scene === //
    mCollisionScene.update(deltaTime);
    ticksCollisionUpdate = get_ticks() - ticksCollisionUpdate;

    // === Keep Track of Leaders === //
    mCurrTopScore = 0;
    for (auto &state : mPlayerStates)
    {
        mCurrTopScore = std::max(mCurrTopScore, state.getFishCaught());
    }

    for (int i = 0; i < Core::MAX_PLAYERS; i++)
    {
        mWinners[i] = mPlayerStates[i].getFishCaught() >= mCurrTopScore;
    }
}

void Scene::update(float deltaTime)
{
    // === Debug Controls === //
    {
        using PlayerJoypadsArray = std::array<Core::PlayerJoypad, JOYPAD_PORT_COUNT>;
        std::any playerJoypadsAny = getGlobalSettingsInterface()->getGlobalSettingValue(static_cast<size_t>(GameSettingsKeys::PLAYER_JOYPADS));
        auto playerJoypads = std::any_cast<PlayerJoypadsArray>(playerJoypadsAny);
        auto ctrl = playerJoypads[static_cast<size_t>(Core::PlyNum::PLAYER_1)].port;
        auto btn = joypad_get_buttons_pressed(ctrl);
        auto held = joypad_get_buttons_held(ctrl);

        if (held.z)
        {
            if (btn.d_up)
                debugOverlay = !debugOverlay;
            if (btn.d_down)
                showFPS = !showFPS;
        }
    }

    // === Update Animations === //
    ticksAnimationUpdate = get_ticks();
    for (auto &animComp : mAnimationComponents)
    {
        animComp.update(deltaTime);
    }
    ticksAnimationUpdate = get_ticks() - ticksAnimationUpdate;

    // === Attach RDP === //
    mLastFB = mCurrentFB;
    mCurrentFB = display_get();
    rdpq_attach(mCurrentFB, display_get_zbuf());

    // === Set Camera === //
    mCamera.update(mViewport);

    // === Draw Viewport === //
    t3d_viewport_attach(&mViewport);

    // === Draw Background === //
    rdpq_set_mode_fill({(uint8_t)(0x80),
                        (uint8_t)(0xb8),
                        (uint8_t)(0xd8),
                        0xFF});
    rdpq_fill_rectangle(0, 0, Core::SCREEN_WIDTH, Core::SCREEN_HEIGHT);

    // === Draw 3D === //
    t3d_frame_start();

    t3d_screen_clear_depth();

    t3d_light_set_ambient(COLOR_AMBIENT);
    t3d_light_set_directional(0, COLOR_DIR, &mLightDirVec);
    t3d_light_set_count(1);

    uint32_t vertices = mMapModel.getModel()->totalVertCount;
    rspq_block_run(mDplMap.get());

    // === Draw players (3D Pass) === //
    for (size_t i = 0; i < Core::MAX_PLAYERS; i++)
    {
        mAnimationComponents[i].draw(mPlayers[i].getPosition(), mPlayers[i].getRotation());
    }

    // === Draw billboards (2D Pass) === //
    for (size_t i = 0; i < Core::MAX_PLAYERS; i++)
    {
        mPlayers[i].drawBillboard(mViewport);
    }

    // === Draw UI === //
    const rdpq_textparms_t center_text_h{
        .style_id = 4,
        .width = (int16_t)display_get_width(),
        .align = ALIGN_CENTER,
    };
    rdpq_text_printf(&center_text_h, Core::FONT_TEXT, 0, Core::TIMER_Y, "%d", (int)ceilf(mStateTime));

    for (int i = 0; i < Core::MAX_PLAYERS; i++)
    {
        const rdpq_textparms_t score_params{.style_id = (int16_t)i};
        rdpq_text_printf(&score_params,
                         Core::FONT_TEXT,
                         Core::SCORE_X + i * Core::SCORE_X_SPACING,
                         Core::SCORE_Y,
                         "%d",
                         mPlayerStates[i].getFishCaught());
    }

    if (mState == State::GAME_OVER)
    {
        const rdpq_textparms_t center_text_hv{
            .style_id = 4,
            .width = (int16_t)display_get_width(),
            .height = (int16_t)display_get_height(),
            .align = ALIGN_CENTER,
            .valign = VALIGN_CENTER,
        };
        std::string message{};
        for (int i = 0; i < Core::MAX_PLAYERS; i++)
        {
            mWinners[i] = mPlayerStates[i].getFishCaught() >= mCurrTopScore;
            if (mWinners[i])
            {
                using WinningPlayersArray = std::array<bool, Core::MAX_PLAYERS>;
                std::any winningPlayersAny = getGlobalSettingsInterface()->getGlobalSettingValue(static_cast<size_t>(GameSettingsKeys::WINNING_PLAYERS));
                WinningPlayersArray winningPlayers{false, false, false, false};

                if (winningPlayersAny.has_value())
                {
                    winningPlayers = std::any_cast<WinningPlayersArray>(winningPlayersAny);
                }

                winningPlayers[i] = true;
                getGlobalSettingsInterface()->setGlobalSettingValue(static_cast<size_t>(GameSettingsKeys::WINNING_PLAYERS), winningPlayers);

                message += ("Player " + std::to_string(i + 1) + " wins!\n");
            }
        }
        rdpq_text_printf(&center_text_hv, Core::FONT_TEXT, 0, 0, message.c_str());
    }

    // Debug UI
    if (debugOverlay)
    {
        mDebugOverlay.draw(*this, vertices, deltaTime);
        Debug::draw((uint16_t *)mCurrentFB->buffer);
    }
    else if (showFPS)
    {
        Debug::printStart();
        Debug::printf(24, 220, "FPS %.2f", display_get_fps());
    }

    // === Detach and show === //
    rdpq_detach_show();
}

const CollisionScene &Scene::getCollScene()
{
    return mCollisionScene;
}

void Scene::reset()
{
    // Reset player positions and rotations
    Vector3 initialPositions[Core::MAX_PLAYERS] = {
        {-25, 0.0f, 0},
        {0, 0.0f, -25},
        {25, 0.0f, 0},
        {0, 0.0f, 25}};
    Vector2 initialRotations[Core::MAX_PLAYERS] = {
        {1, 0},
        {0, -1},
        {-1, 0},
        {0, 1}};

    for (size_t i = 0; i < Core::MAX_PLAYERS; i++)
    {
        mPlayers[i].setPosition(initialPositions[i]);
        mPlayers[i].setRotation(initialRotations[i]);
        mPlayers[i].setVelocity({0.0f, 0.0f, 0.0f});
        mPlayers[i].clearFlag(static_cast<uint32_t>(ActorFlags::FLAG_IS_STUNNED));
        mPlayerStates[i].reset(mPlayers[i], mCollisionScene);
        mAIPlayers[i].reset();
        mWinners[i] = false;
    }

    // Reset game state
    mState = State::INTRO;
    mStateTime = INTRO_TIME;
    mCurrTopScore = 0;
}