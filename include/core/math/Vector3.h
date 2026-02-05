#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <type_traits>
#include <utility>

namespace Math
{
    struct Vector3
    {
        union
        {
            std::array<float, 3> data;
            struct
            {
                float x, y, z;
            };
        };

        constexpr Vector3() : data({0.0f, 0.0f, 0.0f}) {}
        constexpr Vector3(float x, float y, float z) : data({x, y, z}) {}

        constexpr float &operator[](size_t index)
        {
            assert(index < 3);
            return data[index];
        }
        constexpr const float &operator[](size_t index) const
        {
            assert(index < 3);
            return data[index];
        }

        template <typename T,
                  typename = std::enable_if_t<std::is_base_of<Vector3, std::decay_t<T>>::value>>
        Vector3 operator+=(T &&other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        template <typename T,
                  typename = std::enable_if_t<std::is_base_of<Vector3, std::decay_t<T>>::value>>
        Vector3 operator-=(T &&other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vector3 operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        Vector3 operator-() const
        {
            Vector3 result;
            result.x = -x;
            result.y = -y;
            result.z = -z;
            return result;
        }

        bool isZero() const
        {
            return x == 0.0f && y == 0.0f && z == 0.0f;
        }

        float magSqrd() const
        {
            return x * x + y * y + z * z;
        }

        friend Vector3 operator+(Vector3 lhs, const Vector3 &rhs)
        {
            lhs += rhs;
            return lhs;
        }

        friend Vector3 operator-(Vector3 lhs, const Vector3 &rhs)
        {
            lhs -= rhs;
            return lhs;
        }

        friend Vector3 operator*(Vector3 lhs, float scalar)
        {
            lhs.x *= scalar;
            lhs.y *= scalar;
            lhs.z *= scalar;
            return lhs;
        }

        friend Vector3 cross(const Vector3 &a, const Vector3 &b)
        {
            Vector3 result;
            result.x = a.y * b.z - a.z * b.y;
            result.y = a.z * b.x - a.x * b.z;
            result.z = a.x * b.y - a.y * b.x;
            return result;
        }

        friend float dot(const Vector3 &a, const Vector3 &b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        friend Vector3 normalize(const Vector3 &in);

        friend Vector3 normAndScale(const Vector3 &in, float scale)
        {
            Vector3 result;
            const float x_ = in.x;
            const float y_ = in.y;
            const float z_ = in.z;

            const float len2 = x_ * x_ + y_ * y_ + z_ * z_;

            if (len2 <= 1e-20f)
            {
                result.x = 0.0f;
                result.y = 0.0f;
                result.z = 0.0f;
                return result;
            }

            const float invLen = 1.0f / sqrtf(len2);
            const float s = scale * invLen;

            result.x = x_ * s;
            result.y = y_ * s;
            result.z = z_ * s;
            return result;
        }

        friend Vector3 perp(const Vector3 &in);

        friend Vector3 tripleProduct(const Vector3 &a, const Vector3 &b, const Vector3 &c)
        {
            Vector3 result = b * dot(a, c);
            result += a * -dot(b, c);
            return result;
        }
    };

    constexpr Vector3 Vec3Right = {1.0f, 0.0f, 0.0f};
    constexpr Vector3 Vec3Up = {0.0f, 1.0f, 0.0f};
    constexpr Vector3 Vec3Forward = {0.0f, 0.0f, 1.0f};
    constexpr Vector3 Vec3Zero = {0.0f, 0.0f, 0.0f};
    constexpr Vector3 Vec3One = {1.0f, 1.0f, 1.0f};
}