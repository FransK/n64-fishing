#pragma once

#include <memory>
#include <t3d/t3d.h>
#include <vector>

#include "GameSettingsInterface.h"

#include "adapters/T3d.h"
#include "animation/AnimationComponent.h"
#include "collision/CollisionScene.h"
#include "debug/Overlay.h"
#include "input/InputComponent.h"
#include "input/InputComponentVariant.h"
#include "input/PlayerData.h"
#include "input/PlayerState.h"

// #include "ComponentVector.h"

#include "Camera.h"
#include "Player.h"
#include "PlayerAi.h"

using CollisionScene = ::Collision::CollisionScene;
using namespace Core;

constexpr float INTRO_TIME = 3.f;
constexpr float GAME_TIME = 60.f;
constexpr float GAME_OVER_TIME = 3.f;

constexpr uint8_t COLOR_AMBIENT[4] = {0xAA, 0xAA, 0xAA, 0xFF};
constexpr uint8_t COLOR_DIR[4] = {0xFF, 0xAA, 0xAA, 0xFF};

constexpr color_t COLORS[] = {
    PLAYER_COLOR_1,
    PLAYER_COLOR_2,
    PLAYER_COLOR_3,
    PLAYER_COLOR_4,
};

class Scene
{
public:
    Scene();
    ~Scene() = default;

    long ticksActorUpdate{0};
    long ticksCollisionUpdate{0};
    long ticksAnimationUpdate{0};

    const CollisionScene &getCollScene();
    void updateFixed(float deltaTime);
    void update(float deltaTime);
    void reset();

private:
    enum struct State : uint8_t
    {
        INTRO = 0,
        GAME,
        GAME_OVER
    } mState{};

    float mStateTime{};

    /* Player Data Block - Positions, Velocities, etc*/
    std::array<PlayerData, MAX_PLAYERS> mPlayerData{};
    std::array<PlayerState, MAX_PLAYERS> mPlayerStates{};
    std::vector<PlayerAi> mAIPlayers;
    std::vector<InputComponentVariant> mInputComponents;
    CollisionScene mCollisionScene;
    std::vector<AnimationComponent> mAnimationComponents;
    std::array<int, MAX_PLAYERS> mStunnedIds{-1};

    /* Container class? */
    std::vector<Player> mPlayers;
    std::array<uint8_t, MAX_PLAYERS> mWinners{0};
    int mCurrTopScore{0};

    Adapters::ModelAdapter mPlayerModel;
    Adapters::ModelAdapter mMapModel;

    surface_t *mCurrentFB{};
    surface_t *mLastFB{};
    Adapters::Mat4FPAdapter mMapMatFP;
    Adapters::RspqBlockAdapter mDplMap;

    Adapters::FontAdapter mFontBillboard;
    Adapters::FontAdapter mFontText;

    T3DViewport mViewport{};
    Camera mCamera{};
    T3DVec3 mLightDirVec{};

    Debug::Overlay mDebugOverlay{};
};