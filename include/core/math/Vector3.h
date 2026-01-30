#pragma once

#include <cmath>
#include <type_traits>
#include <utility>

namespace Math
{
    struct __attribute__((packed)) Vector3
    {
        float x, y, z;

        float &operator[](int index) { return (&x)[index]; }
        const float &operator[](int index) const { return (&x)[index]; }

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

        friend Vector3 addScaled(const Vector3 &lhs, const Vector3 &normal, float scale)
        {
            Vector3 result;
            result.x = lhs.x + normal.x * scale;
            result.y = lhs.y + normal.y * scale;
            result.z = lhs.z + normal.z * scale;
            return result;
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
            const float x = in.x;
            const float y = in.y;
            const float z = in.z;

            const float len2 = x * x + y * y + z * z;

            if (len2 <= 1e-20f)
            {
                result.x = 0.0f;
                result.y = 0.0f;
                result.z = 0.0f;
                return result;
            }

            const float invLen = 1.0f / sqrtf(len2);
            const float s = scale * invLen;

            result.x = x * s;
            result.y = y * s;
            result.z = z * s;
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