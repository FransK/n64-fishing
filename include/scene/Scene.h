#pragma once

#include <memory>
#include <t3d/t3d.h>
#include "containers/Vector.h"

#include "Config.h"
#include "GlobalSettingsInterface.h"

#include "adapters/T3d.h"
#include "animation/AnimationComponent.h"
#include "animation/PlayerAnimatable.h"
#include "collision/CollisionScene.h"
#include "debug/Overlay.h"
#include "input/InputComponent.h"
#include "input/InputComponentVariant.h"
#include "services/AssetData.h"

#include "scene/Camera.h"
#include "scene/Player.h"
#include "scene/PlayerAi.h"

using Collision::CollisionScene;

constexpr float INTRO_TIME = 1.f;
constexpr float GAME_TIME = 10.f;
constexpr float GAME_OVER_TIME = 3.f;

constexpr uint8_t COLOR_AMBIENT[4] = {0xAA, 0xAA, 0xAA, 0xFF};
constexpr uint8_t COLOR_DIR[4] = {0xFF, 0xAA, 0xAA, 0xFF};

constexpr color_t COLORS[] = {
    Core::PLAYER_COLOR_1,
    Core::PLAYER_COLOR_2,
    Core::PLAYER_COLOR_3,
    Core::PLAYER_COLOR_4,
};

class Scene
{
public:
    Scene();
    ~Scene();

    long ticksActorUpdate{0};
    long ticksCollisionUpdate{0};
    long ticksAnimationUpdate{0};

    const CollisionScene &getCollScene();
    void updateFixed(float deltaTime);
    void update(float deltaTime);
    void reset();

private:
    using PlayerAnimationComponent = AnimationComponent<PlayerAnimatable>;

    enum struct State : uint8_t
    {
        INTRO = 0,
        GAME,
        GAME_OVER
    } mState{};

    float mStateTime{};

    Containers::vector<Player> mPlayers{};
    Containers::vector<PlayerAi> mAIPlayers{};
    Containers::vector<InputComponentVariant> mInputComponents{};
    Containers::vector<PlayerAnimationComponent> mAnimationComponents{};
    std::unique_ptr<CollisionScene> mCollisionScene{std::make_unique<CollisionScene>()};
    Containers::vector<Services::PlayerData> mInitialPlayerData;

    Containers::vector<bool> mWinners;
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