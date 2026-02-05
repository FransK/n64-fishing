#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <type_traits>
#include <utility>

namespace Math
{
    struct Vector2
    {
        union {
            std::array<float, 2> data;
            struct {
                float x, y;
            };
        };

        constexpr Vector2() : data({0.0f, 0.0f}) {}
        constexpr Vector2(float x, float y) : data({x, y}) {}

        constexpr float &operator[](size_t index)
        {
            assert(index < 2);
            return data[index];
        }
        constexpr const float &operator[](size_t index) const
        {
            assert(index < 2);
            return data[index];
        }

        template <typename T,
                  typename = std::enable_if_t<std::is_base_of<Vector2, std::decay_t<T>>::value>>
        Vector2 operator+=(T &&other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        template <typename T,
                  typename = std::enable_if_t<std::is_base_of<Vector2, std::decay_t<T>>::value>>
        Vector2 operator-=(T &&other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector2 operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vector2 operator-() const
        {
            Vector2 result;
            result.x = -x;
            result.y = -y;
            return result;
        }

        bool isZero() const
        {
            return x == 0.0f && y == 0.0f;
        }

        float magSqrd() const
        {
            return x * x + y * y;
        }

        friend Vector2 operator+(Vector2 lhs, const Vector2 &rhs)
        {
            lhs += rhs;
            return lhs;
        }

        friend Vector2 operator-(Vector2 lhs, const Vector2 &rhs)
        {
            lhs -= rhs;
            return lhs;
        }

        friend Vector2 operator*(Vector2 lhs, float scalar)
        {
            lhs.x *= scalar;
            lhs.y *= scalar;
            return lhs;
        }

        friend float dot(const Vector2 &a, const Vector2 &b)
        {
            return a.x * b.x + a.y * b.y;
        }

        friend Vector2 normalize(const Vector2 &in)
        {
            Vector2 result;
            const float len2 = in.x * in.x + in.y * in.y;

            if (len2 < 1e-20f)
            {
                result.x = 0.0f;
                result.y = 0.0f;
                return result;
            }

            const float invLen = 1.0f / sqrtf(len2);

            result.x = in.x * invLen;
            result.y = in.y * invLen;
            return result;
        }

        friend Vector2 normAndScale(const Vector2 &in, float scale)
        {
            Vector2 result;
            const float x = in.x;
            const float y = in.y;

            const float len2 = x * x + y * y;

            if (len2 <= 1e-20f)
            {
                result.x = 0.0f;
                result.y = 0.0f;
                return result;
            }

            const float invLen = 1.0f / sqrtf(len2);
            const float s = scale * invLen;

            result.x = x * s;
            result.y = y * s;
            return result;
        }

        friend Vector2 addScaled(const Vector2 &lhs, const Vector2 &dir, float scale)
        {
            Vector2 result;
            result.x = lhs.x + dir.x * scale;
            result.y = lhs.y + dir.y * scale;
            return result;
        }
    };

    constexpr Vector2 Vec2Right = {1.0f, 0.0f};
    constexpr Vector2 Vec2Up = {0.0f, 1.0f};
    constexpr Vector2 Vec2Zero = {0.0f, 0.0f};
    constexpr Vector2 Vec2One = {1.0f, 1.0f};
}
