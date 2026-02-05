/**
 * @copyright 2024 - Max Bebök
 * @license MIT
 */
#pragma once

#include <t3d/t3dmath.h>

#include "math/Box3d.h"

namespace Debug
{
    class DebugDraw
    {
    public:
        DebugDraw();
        ~DebugDraw();
        DebugDraw(const DebugDraw &) = delete;
        DebugDraw &operator=(const DebugDraw &) = delete;
        DebugDraw(DebugDraw &&) = delete;
        DebugDraw &operator=(DebugDraw &&) = delete;

        void drawLine(const T3DVec3 &a, const T3DVec3 &b, color_t color = {0xFF, 0xFF, 0xFF, 0xFF}) const;
        void drawBox(const Math::Box3D &box) const;
        void drawSphere(const T3DVec3 &center, float radius, color_t color = {0xFF, 0xFF, 0xFF, 0xFF}) const;
        void draw(uint16_t *fb) const;

        void printStart() const;
        float print(float x, float y, const char *str) const;
        float printf(float x, float y, const char *fmt, ...) const;
    };
}