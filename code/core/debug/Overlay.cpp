/**
 * @copyright 2024 - Max Bebök
 * @license MIT
 */
#include "Overlay.h"
#include "DebugDraw.h"
#include "../../scene/Scene.h"

namespace Debug
{
    constexpr float barWidth = 200.0f;
    constexpr float barHeight = 3.0f;
    constexpr float barRefTimeMs = 1000.0f / 60.0f; // FPS

    constexpr color_t COLOR_BVH{0, 0xAA, 0x22, 0xFF};
    constexpr color_t COLOR_COLL{0x22, 0xFF, 0, 0xFF};
    constexpr color_t COLOR_ACTOR_UPDATE{0xAA, 0, 0, 0xFF};
    constexpr color_t COLOR_CULL{0xFF, 0x11, 0x99, 0xFF};
    constexpr color_t COLOR_DEBUG{0xFF, 0xFF, 0xFF, 0xFF};

    uint64_t ticksSelf = 0;

    constexpr float usToWidth(long timeUs)
    {
        double timeMs = (double)timeUs / 1000.0;
        return (float)(timeMs / barRefTimeMs) * barWidth;
    }

    bool showCollMesh = false;
    bool showCollSpheres = false;
    bool actorDebug = false;
}

void Debug::Overlay::draw(Scene &scene, uint32_t vertCount, float deltaTime)
{
    const auto &collScene = scene.getCollScene();
    uint64_t newTicksSelf = get_ticks();

    collScene.debugDraw();

    float posX = 24;
    float posY = 24;

    Debug::printStart();
    Debug::printf(posX + barWidth + 2, 14, "FPS %.2f", display_get_fps());
    Debug::printf(posX + barWidth + 2, 34, "Buffers %.2d", display_get_num_buffers());

    heap_stats_t heap_stats;
    sys_get_heap_stats(&heap_stats);

    rdpq_set_prim_color(COLOR_ACTOR_UPDATE);
    posX = Debug::printf(posX, posY, "%.2f", (double)TICKS_TO_US(scene.ticksActorUpdate) / 1000.0) + 8;
    rdpq_set_prim_color(COLOR_COLL);
    posX = Debug::printf(posX, posY, "%.2f", (double)TICKS_TO_US(scene.ticksCollisionUpdate) / 1000.0) + 8;
    rdpq_set_prim_color(COLOR_CULL);
    posX = Debug::printf(posX, posY, "%.2f", (double)TICKS_TO_US(scene.ticksAnimationUpdate) / 1000.0) + 8;
    rdpq_set_prim_color(COLOR_DEBUG);
    posX = Debug::printf(posX, posY, "%.2f", (double)TICKS_TO_US(ticksSelf) / 1000.0) + 8;

    posX = 24 + barWidth - 50;
    posX = Debug::printf(posX, posY, "V:%d", vertCount) + 8;
    Debug::printf(posX, posY, "H:%d", heap_stats.used / 1024);

    // Performance graph
    posX = 24;
    posY = 16;

    float timeActorUpdate = usToWidth(TICKS_TO_US(scene.ticksActorUpdate));
    float timeColl = usToWidth(TICKS_TO_US(scene.ticksCollisionUpdate));
    float timeCull = usToWidth(TICKS_TO_US(scene.ticksAnimationUpdate));
    float timeSelf = usToWidth(TICKS_TO_US(ticksSelf));

    rdpq_set_mode_fill({0, 0, 0, 0xFF});
    rdpq_fill_rectangle(posX - 1, posY - 1, posX + (barWidth / 2), posY + barHeight + 1);
    rdpq_set_mode_fill({0x33, 0x33, 0x33, 0xFF});
    rdpq_fill_rectangle(posX - 1 + (barWidth / 2), posY - 1, posX + barWidth + 1, posY + barHeight + 1);

    rdpq_set_fill_color(COLOR_ACTOR_UPDATE);
    rdpq_fill_rectangle(posX, posY, posX + timeActorUpdate, posY + barHeight);
    posX += timeActorUpdate;
    rdpq_set_fill_color(COLOR_COLL);
    rdpq_fill_rectangle(posX, posY, posX + timeColl, posY + barHeight);
    posX += timeColl;
    rdpq_set_fill_color(COLOR_CULL);
    rdpq_fill_rectangle(posX, posY, posX + timeCull, posY + barHeight);
    posX += timeCull;
    rdpq_set_fill_color(COLOR_DEBUG);
    rdpq_fill_rectangle(posX, posY, posX + timeSelf, posY + barHeight);

    newTicksSelf = get_ticks() - newTicksSelf;
    if (newTicksSelf < TICKS_FROM_MS(2))
    {
        ticksSelf = newTicksSelf;
    }
}
