#pragma once

#include <t3d/t3d.h>
#include "../main.h"
#include "camera.h"
#include "player.h"

class Scene
{
private:
    T3DViewport viewport{};
    Camera cam{};
    Player player{};
    InputState inputState{};

    void read_inputs(PlyNum plyNum);

public:
    void update_fixed(float deltaTime);
    void update(float deltaTime);

    Scene();
    ~Scene() = default;
};