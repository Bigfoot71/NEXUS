/**
 * Copyright (c) 2023-2024 Le Juez Victor
 *
 * This software is provided "as-is", without any express or implied warranty. In no event 
 * will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial 
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not claim that you 
 *   wrote the original software. If you use this software in a product, an acknowledgment 
 *   in the product documentation would be appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *   as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 */

#ifndef NEXUS_MATH_VEC4_HPP
#define NEXUS_MATH_VEC4_HPP

#ifdef SUPPORT_MODEL
#   include <assimp/quaternion.h>
#endif

#ifdef SUPPORT_PHYSICS_3D
#   include <LinearMath/btQuaternion.h>
#endif

#include "./nxMat4.hpp"
#include "./nxVec3.hpp"
#include <algorithm>
#include <cmath>

namespace nexus { namespace math {

    template <typename T>
    struct NEXUS_API Vector4
    {
        static constexpr Uint8 Dimensions = 4;
        typedef T value_type;

        T x, y, z, w;

        /**
         * @brief Default constructor. Constructs a vector with all components set to zero.
         */
        constexpr Vector4()
        : x(0), y(0), z(0), w(0)
        { }

        /**
         * @brief Constructs a vector with all components set to the specified value.
         *
         * @param value The value to set for all components.
         */
        constexpr explicit Vector4(T value)
        : x(value), y(value), z(value), w(value)
        { }

        /**
         * @brief Constructs a vector with the specified components.
         *
         * @param x The x-component.
         * @param y The y-component.
         * @param z The z-component.
         * @param w The w-component.
         */
        constexpr Vector4(T x, T y, T z, T w = 1.0f)
        : x(x), y(y), z(z), w(w)
        { }

        /**
         * @brief Constructs a Vector4 from a Vector3 with an optional w-component.
         *
         * @param vec3 The Vector3 to use for the x, y, and z components.
         * @param w The w-component.
         */
        constexpr Vector4(const Vector3<T>& vec3, float w = 1.0f)
        : x(vec3.x), y(vec3.y), z(vec3.z), w(w)
        { }

#       ifdef SUPPORT_MODEL

        /**
         * @brief Constructor initializing a Vector4 from an aiQuaternion.
         *
         * This constructor initializes a Vector4 object with the x, y, z, and w components extracted from
         * the provided aiQuaternion object.
         *
         * @param quat The aiQuaternion object to initialize from.
         */
        constexpr Vector4(const aiQuaternion& quat)
        : x(quat.x), y(quat.y), z(quat.z), w(quat.w)
        { }

        /**
         * @brief Conversion operator to aiQuaternion.
         *
         * This operator converts the current Vector4 object to an aiQuaternion object.
         * The x, y, z, and w components of the current Vector4 object are used to construct the resulting aiQuaternion object.
         *
         * @return An aiQuaternion object equivalent to this Vector4.
         */
        operator aiQuaternion() const
        {
            return aiQuaternion(x, y, z, w);
        }

#       endif

#       ifdef SUPPORT_PHYSICS_3D

        /**
         * @brief Constructs a Vector4 from a btVector4.
         *
         * @param vec The btVector4 to construct from.
         */
        constexpr Vector4(const btVector4& vec)
        : x(vec.m_floats[0]), y(vec.m_floats[1]), z(vec.m_floats[2]), w(vec.m_floats[3])
        { }

        /**
         * @brief Conversion operator to btVector4.
         *
         * @return btVector4 The converted btVector4.
         */
        /*constexpr*/ operator btVector4() const
        {
            return btVector4(x, y, z, w);
        }

        /**
         * @brief Constructs a Vector4 from a btQuaternion.
         *
         * @param quat The btQuaternion to construct from.
         */
        constexpr Vector4(const btQuaternion& quat)
        : x(quat.x()), y(quat.y()), z(quat.z()), w(quat.w())
        { }

        /**
         * @brief Conversion operator to btQuaternion.
         *
         * @return btQuaternion The converted btQuaternion.
         */
        /*constexpr*/ operator btQuaternion() const
        {
            return btQuaternion(x, y, z, w);
        }

#       endif

        /**
         * @brief Conversion operator to convert the vector to a Vector4 of a different type.
         *
         * Converts the current vector to a Vector4 of a different type by casting its components to the new type.
         *
         * @tparam U The type to convert the components of the vector to.
         * @return A Vector4 containing the components of the current vector casted to the type U.
         */
        template <typename U>
        constexpr operator Vector4<U>() const
        {
            return Vector4<U>(
                static_cast<U>(x),
                static_cast<U>(y),
                static_cast<U>(z),
                static_cast<U>(w));
        }

        /**
         * @brief Accesses the component at the specified index.
         *
         * @param axis The index of the component to access.
         * @return T& A reference to the component at the specified index.
         */
        constexpr T& operator[](int axis)
        {
            return *(reinterpret_cast<T*>(this) + axis);
        }

        /**
         * @brief Accesses the component at the specified index.
         *
         * @param axis The index of the component to access.
         * @return const T& A const reference to the component at the specified index.
         */
        constexpr const T& operator[](int axis) const
        {
            return *(reinterpret_cast<const T*>(this) + axis);
        }

        /**
         * @brief Negates each component of the vector.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @return Vector4 The resulting negated vector.
         */
        constexpr Vector4 operator-()
        {
            Vector4 result = *this;
            for (Uint8 i = 0; i < 4; i++) result[i] = -result[i];
            return result;
        }

        /**
         * @brief Subtracts a scalar value from each component of the vector.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param scalar The scalar value to subtract.
         * @return Vector4 The resulting vector.
         */
        constexpr Vector4 operator-(T scalar) const
        {
            Vector4 result = *this;
            for (Uint8 i = 0; i < 4; i++) result[i] -= scalar;
            return result;
        }

        /**
         * @brief Adds a scalar value to each component of the vector.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param scalar The scalar value to add.
         * @return Vector4 The resulting vector.
         */
        constexpr Vector4 operator+(T scalar) const
        {
            Vector4 result = *this;
            for (Uint8 i = 0; i < 4; i++) result[i] += scalar;
            return result;
        }

        /**
         * @brief Multiplies each component of the vector by a scalar value.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param scalar The scalar value to multiply by.
         * @return Vector4 The resulting vector.
         */
        constexpr Vector4 operator*(T scalar) const
        {
            Vector4 result = *this;
            for (Uint8 i = 0; i < 4; i++) result[i] *= scalar;
            return result;
        }

        /**
         * @brief Scalar division operator.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param scalar The scalar value to divide by.
         * @return Vector4 The result of the division.
         */
        constexpr Vector4 operator/(T scalar) const
        {
            if (scalar == 0) return { 0, 0, 0, 0 };
            const double inv = 1.0 / scalar;

            Vector4 result = *this;
            for (Uint8 i = 0; i < 4; i++) result[i] *= inv;
            return result;
        }

        /**
         * @brief Vector subtraction operator.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param other The vector to subtract.
         * @return Vector4 The result of the subtraction.
         */
        constexpr Vector4 operator-(const Vector4& other) const
        {
            Vector4 result = *this;
            for (Uint8 i = 0; i < 4; i++) result[i] -= other[i];
            return result;
        }

        /**
         * @brief Vector addition operator.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param other The vector to add.
         * @return Vector4 The result of the addition.
         */
        constexpr Vector4 operator+(const Vector4& other) const
        {
            Vector4 result = *this;
            for (Uint8 i = 0; i < 4; i++) result[i] += other[i];
            return result;
        }

        /**
         * @brief Vector multiplication operator.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param other The vector to multiply by.
         * @return Vector4 The result of the multiplication.
         */
        constexpr Vector4 operator*(const Vector4& other) const
        {
            Vector4 result = *this;
            for (Uint8 i = 0; i < 4; i++) result[i] *= other[i];
            return result;
        }

        /**
         * @brief Vector division operator.
         * 
         * @param other The vector to divide by.
         * @return Vector4 The result of the division.
         */
        constexpr Vector4 operator/(const Vector4& other) const
        {
            return Vector4(
                other.x != 0 ? x / other.x : 0,
                other.y != 0 ? y / other.y : 0,
                other.z != 0 ? z / other.z : 0,
                other.w != 0 ? w / other.w : 0);
        }

        /**
         * @brief Equality operator.
         * 
         * @param other The vector to compare with.
         * @return bool True if the vectors are equal, false otherwise.
         */
        constexpr bool operator==(const Vector4& other) const
        {
            return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
        }

        /**
         * @brief Inequality operator.
         * 
         * @param other The vector to compare with.
         * @return bool True if the vectors are not equal, false otherwise.
         */
        constexpr bool operator!=(const Vector4& other) const
        {
            return (x != other.x) || (y != other.y) || (z != other.z) || (w != other.w);
        }

        /**
         * @brief Scalar subtraction and assignment operator.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param scalar The scalar value to subtract.
         * @return Vector4& Reference to the modified vector.
         */
        Vector4& operator-=(T scalar)
        {
            for (Uint8 i = 0; i < 4; i++) (*this)[i] -= scalar;
            return *this;
        }

        /**
         * @brief Scalar addition and assignment operator.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param scalar The scalar value to add.
         * @return Vector4& Reference to the modified vector.
         */
        Vector4& operator+=(T scalar)
        {
            for (Uint8 i = 0; i < 4; i++) (*this)[i] += scalar;
            return *this;
        }

        /**
         * @brief Scalar multiplication and assignment operator.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param scalar The scalar value to multiply by.
         * @return Vector4& Reference to the modified vector.
         */
        Vector4& operator*=(T scalar)
        {
            for (Uint8 i = 0; i < 4; i++) (*this)[i] *= scalar;
            return *this;
        }

        /**
         * @brief Scalar division and assignment operator.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param scalar The scalar value to divide by.
         * @return Vector4& Reference to the modified vector.
         */
        Vector4& operator/=(T scalar)
        {
            if (scalar == 0) { *this = { 0, 0, 0, 0 }; return *this; };
            const double inv = 1.0 / scalar;

            for (Uint8 i = 0; i < 4; i++) (*this)[i] *= inv;
            return *this;
        }

        /**
         * @brief Vector subtraction and assignment operator.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param other The vector to subtract.
         * @return Vector4& Reference to the modified vector.
         */
        Vector4& operator-=(const Vector4& other)
        {
            for (Uint8 i = 0; i < 4; i++) (*this)[i] -= other[i];
            return *this;
        }

        /**
         * @brief Vector addition and assignment operator.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param other The vector to add.
         * @return Vector4& Reference to the modified vector.
         */
        Vector4& operator+=(const Vector4& other)
        {
            for (Uint8 i = 0; i < 4; i++) (*this)[i] += other[i];
            return *this;
        }

        /**
         * @brief Vector multiplication and assignment operator.
         * @note: Hope this loop will be vectorized by the compiler or at least unroll in the worst case.
         *
         * @param other The vector to multiply by.
         * @return Vector4& Reference to the modified vector.
         */
        Vector4& operator*=(const Vector4& other)
        {
            for (Uint8 i = 0; i < 4; i++) (*this)[i] *= other[i];
            return *this;
        }

        /**
         * @brief Vector division and assignment operator.
         *
         * @param other The vector to divide by.
         * @return Vector4& Reference to the modified vector.
         */
        Vector4& operator/=(const Vector4& other)
        {
            if (other.x != 0.0f) x /= other.x;
            if (other.y != 0.0f) y /= other.y;
            if (other.z != 0.0f) z /= other.z;
            if (other.w != 0.0f) w /= other.w;
            return *this;
        }

        /**
         * @brief Method to check if the vector is equal to (0,0,0,0).
         *
         * @return bool True if the vector is equal to (0,0,0,0), false otherwise.
         */
        bool IsZero() const
        {
            return !(x + y + z + w);
        }

        /**
         * @brief Method to calculate the magnitude (length) of the vector.
         *
         * @return double The magnitude of the vector.
         */
        double Magnitude() const
        {
            return std::sqrt(x * x + y * y + z * z + w * w);
        }

        /**
         * @brief Function to calculate the length (magnitude) of the vector.
         *
         * @return double The length of the vector.
         */
        double Length() const
        {
            return Magnitude();
        }

        /**
         * @brief Function to calculate the length (magnitude) squared of the vector.
         *
         * @return T The length squared of the vector.
         */
        T LengthSqr() const
        {
            return x * x + y * y + z * z + w * w;
        }

        /**
         * @brief Method to calculate the dot product of the vector with another vector.
         *
         * @param other The other vector.
         * @return T The dot product of the two vectors.
         */
        T Dot(const Vector4& other) const
        {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        /**
         * @brief Method to normalize the vector.
         */
        void Normalize()
        {
            const double mag = Magnitude();
            if (mag != 0.0) (*this) *= 1.0 / mag;
        }

        /**
         * @brief Method to get a normalized vector.
         *
         * @return Vector4 The normalized vector.
         */
        Vector4 Normalized() const
        {
            Vector4 result(*this);
            result.Normalize();
            return result;
        }

        /**
         * @brief Method to invert a quaternion.
         */
        void QuatInvert()
        {
            const double lenSq = this->LengthSqr();

            if (lenSq != 0.0)
            {
                const double inv = 1.0 / lenSq;
                x *= -inv, y *= -inv, z *= -inv, w *= inv;
            }
        }

        /**
         * @brief Method to get the inverted quaternion.
         *
         * @return Vector4 The inverted quaternion.
         */
        Vector4 QuatInverted() const
        {
            Vector4 result(*this);
            result.QuatInvert();
            return result;
        }

        /**
         * @brief Method to multiply the quaternion by another quaternion.
         *
         * @param other The other quaternion.
         */
        void QuatMultiply(const Vector4& other)
        {
            x = x * other.w + w * other.x + y * other.z - z * other.y;
            y = y * other.w + w * other.y + z * other.x - x * other.z;
            z = z * other.w + w * other.z + x * other.y - y * other.x;
            w = w * other.w - x * other.x - y * other.y - z * other.z;
        }

        /**
         * @brief Method to get the quaternion multiplied by another quaternion.
         *
         * @param other The other quaternion.
         * @return Vector4 The resulting quaternion.
         */
        Vector4 QuatMultiplied(const Vector4& other) const
        {
            return {
                x * other.w + w * other.x + y * other.z - z * other.y,
                y * other.w + w * other.y + z * other.x - x * other.z,
                z * other.w + w * other.z + x * other.y - y * other.x,
                w * other.w - x * other.x - y * other.y - z * other.z
            };
        }

        /**
         * @brief Method to calculate the distance between two vectors.
         *
         * @param other The other vector.
         * @return double The distance between the two vectors.
         */
        double Distance(const Vector4& other) const
        {
            return (*this - other).Magnitude();
        }

        /**
         * @brief Function to calculate the squared distance between two vectors.
         *
         * @param other The other vector.
         * @return T The squared distance between the two vectors.
         */
        T DistanceSqr(const Vector4& other) const
        {
            const Vector4 diff = *this - other;
            return diff.x * diff.x + diff.y * diff.y + diff.z * diff.z + diff.w * diff.w;
        }

        /**
         * @brief Function to transform the vector by a 4x4 matrix.
         *
         * @param matrix The 4x4 matrix.
         */
        void Transform(const Mat4& matrix)
        {
            *this = {
                x * matrix.m[0] + y * matrix.m[4] + z * matrix.m[8] + w * matrix.m[12],
                x * matrix.m[1] + y * matrix.m[5] + z * matrix.m[9] + w * matrix.m[13],
                x * matrix.m[2] + y * matrix.m[6] + z * matrix.m[10] + w * matrix.m[14],
                x * matrix.m[3] + y * matrix.m[7] + z * matrix.m[11] + w * matrix.m[15]
            };
        }

        /**
         * @brief Function to get the vector transformed by a 4x4 matrix.
         *
         * @param matrix The 4x4 matrix.
         * @return Vector4 The transformed vector.
         */
        Vector4 Transformed(const Mat4& matrix) const
        {
            return {
                x * matrix.m[0] + y * matrix.m[4] + z * matrix.m[8] + w * matrix.m[12],
                x * matrix.m[1] + y * matrix.m[5] + z * matrix.m[9] + w * matrix.m[13],
                x * matrix.m[2] + y * matrix.m[6] + z * matrix.m[10] + w * matrix.m[14],
                x * matrix.m[3] + y * matrix.m[7] + z * matrix.m[11] + w * matrix.m[15]
            };
        }

        /**
         * @brief Function to clamp the vector components within a specified range.
         *
         * @param min The minimum value vector.
         * @param max The maximum value vector.
         * @return Vector4 The clamped vector.
         */
        Vector4 Clamp(const Vector4& min, const Vector4& max)
        {
            return Vector4(
                std::clamp(x, min.x, max.x),
                std::clamp(y, min.y, max.y),
                std::clamp(z, min.z, max.z),
                std::clamp(w, min.w, max.w)
            );
        }

        /**
         * @brief Function to clamp the vector components within a specified range.
         *
         * @param min The minimum value.
         * @param max The maximum value.
         * @return Vector4 The clamped vector.
         */
        Vector4 Clamp(T min, T max)
        {
            return Vector4(
                std::clamp(x, min, max),
                std::clamp(y, min, max),
                std::clamp(z, min, max),
                std::clamp(w, min, max)
            );
        }

        /**
         * @brief Function to get the absolute values of the vector components.
         *
         * @return Vector4 The vector with absolute values.
         */
        Vector4 Abs()
        {
            return Vector4(std::abs(x), std::abs(y), std::abs(z), std::abs(w));
        }
    };

    // Specific class for quaternions in double precision
    using Quaternion = Vector4<double>;

    // Specific class for 4D floating point vectors
    using Vec4 = Vector4<float>;

    // Specific class for 4D vectors in integers
    using IVec4 = Vector4<int>;


    /* Matrix Rotation methods */

    template <typename T>
    constexpr Mat4 Mat4::FromQuaternion(const Vector4<T>& q)
    {
        Mat4 result = Identity();

        float a2 = q.x * q.x;
        float b2 = q.y * q.y;
        float c2 = q.z * q.z;
        float ac = q.x * q.z;
        float ab = q.x * q.y;
        float bc = q.y * q.z;
        float ad = q.w * q.x;
        float bd = q.w * q.y;
        float cd = q.w * q.z;

        result.m[0] = 1 - 2 * (b2 + c2);
        result.m[1] = 2 * (ab + cd);
        result.m[2] = 2 * (ac - bd);

        result.m[4] = 2 * (ab - cd);
        result.m[5] = 1 - 2 * (a2 + c2);
        result.m[6] = 2 * (bc + ad);

        result.m[8] = 2 * (ac + bd);
        result.m[9] = 2 * (bc - ad);
        result.m[10] = 1 - 2 * (a2 + b2);

        return result;
    }

    template <typename T>
    constexpr Vector4<T> Mat4::operator*(const Vector3<T>& v) const
    {
        return {
            m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12],
            m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13],
            m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14],
            m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15]
        };
    }

    template <typename T>
    constexpr Vector4<T> Mat4::operator*(const Vector4<T>& v) const
    {
        return {
            m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w,
            m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w,
            m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w,
            m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w
        };
    }

    /* Vector3 Rotation methods */

    template <typename T>
    void Vector3<T>::Rotate(const Vector4<T>& q)
    {
        x = x * (q.x * q.x + q.w * q.w - q.y * q.y - q.z * q.z) + y * (2 * q.x * q.y - 2 * q.w * q.z) + z * (2 * q.x * q.z + 2 * q.w * q.y);
        y = x * (2 * q.w * q.z + 2 * q.x * q.y) + y * (q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z) + z * (-2 * q.w * q.x + 2 * q.y * q.z);
        z = x * (-2 * q.w * q.y + 2 * q.x * q.z) + y * (2 * q.w * q.x + 2 * q.y * q.z) + z * (q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
    }

    template <typename T>
    Vector3<T> Vector3<T>::Rotated(const Vector4<T>& q)
    {
        math::Vector3 result = *this;
        result.Rotate(q);
        return result;
    }

}}

#endif //NEXUS_MATH_VEC4_HPP
