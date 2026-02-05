#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <type_traits>
#include <utility>

#include "Vector2.h"
#include "Vector3.h"

namespace Math
{
    struct Quaternion
    {
        union {
            std::array<float, 4> data;
            struct {
                float x, y, z, w;
            };
        };

        constexpr Quaternion() : data({0.0f, 0.0f, 0.0f, 0.0f}) {}
        constexpr Quaternion(float x, float y, float z, float w) : data({x, y, z, w}) {}

        constexpr float &operator[](size_t index)
        {
            assert(index < 4);
            return data[index];
        }
        constexpr const float &operator[](size_t index) const
        {
            assert(index < 4);
            return data[index];
        }

        template <typename T,
                  typename = std::enable_if_t<std::is_base_of<Quaternion, std::decay_t<T>>::value>>
        Quaternion operator*=(T &&other)
        {
            float newW = w * other.w - x * other.x - y * other.y - z * other.z;
            float newX = w * other.x + x * other.w + y * other.z - z * other.y;
            float newY = w * other.y - x * other.z + y * other.w + z * other.x;
            float newZ = w * other.z + x * other.y - y * other.x + z * other.w;
            x = newX;
            y = newY;
            z = newZ;
            w = newW;
            return *this;
        }

        Quaternion operator-() const
        {
            Quaternion result;
            result.x = -x;
            result.y = -y;
            result.z = -z;
            result.w = -w;
            return result;
        }

        float magSqrd() const
        {
            return x * x + y * y + z * z + w * w;
        }

        friend Quaternion operator*(Quaternion lhs, const Quaternion &rhs)
        {
            lhs *= rhs;
            return lhs;
        }

        friend float dot(const Quaternion &a, const Quaternion &b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }

        friend Quaternion conjugate(const Quaternion &q)
        {
            Quaternion result;
            result.x = -q.x;
            result.y = -q.y;
            result.z = -q.z;
            result.w = q.w;
            return result;
        }

        friend Quaternion normalize(const Quaternion &in)
        {
            Quaternion result;
            const float len2 = in.x * in.x + in.y * in.y + in.z * in.z + in.w * in.w;

            if (len2 < 1e-20f)
            {
                result.x = 0.0f;
                result.y = 0.0f;
                result.z = 0.0f;
                result.w = 1.0f;
                return result;
            }

            const float invLen = 1.0f / sqrtf(len2);

            result.x = in.x * invLen;
            result.y = in.y * invLen;
            result.z = in.z * invLen;
            result.w = in.w * invLen;
            return result;
        }

        friend Quaternion axisComplex(const Vector3 &axis, const Vector2 &complex)
        {
            Quaternion result;

            float sinTheta = 0.5f - complex.x * 0.5f;

            if (sinTheta < 0.0f)
            {
                sinTheta = 0.0f;
            }
            else
            {
                sinTheta = sqrtf(sinTheta);

                if (complex.y < 0.0f)
                {
                    sinTheta = -sinTheta;
                }
            }

            float cosTheta = 0.5f + complex.x * 0.5f;

            if (cosTheta < 0.0f)
            {
                cosTheta = 0.0f;
            }
            else
            {
                cosTheta = sqrtf(cosTheta);
            }

            result.x = axis.x * sinTheta;
            result.y = axis.y * sinTheta;
            result.z = axis.z * sinTheta;
            result.w = cosTheta;
            return result;
        }
    };

    constexpr Quaternion QuatIdentity = {0.0f, 0.0f, 0.0f, 1.0f};

    inline Quaternion axisComplex(const Vector3 &axis, const Vector2 &complex);
}
