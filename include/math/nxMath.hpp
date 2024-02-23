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

#ifndef NEXUS_MATH_HPP
#define NEXUS_MATH_HPP

#include "../gfx/nxColor.hpp"
#include "./nxVec2.hpp"
#include "./nxVec3.hpp"
#include "./nxVec4.hpp"
#include <type_traits>
#include <cmath>

namespace nexus { namespace math {

    constexpr double Inv255     = 0.00392156862745098039;   ///< The reciprocal of 255, useful for converting 8-bit color values to the range [0, 1].
    constexpr double Sqrt2      = 1.41421356237309504880;   ///< The square root of 2.
    constexpr double Sqrt3      = 1.73205080756887729352;   ///< The square root of 3.
    constexpr double Phi        = 1.61803398874989484820;   ///< The golden ratio, approximately (1 + sqrt(5)) / 2.
    constexpr double Pi         = 3.14159265358979323846;   ///< The mathematical constant pi.
    constexpr double Tau        = 2.0 * Pi;                 ///< Tau (τ) is equal to 2 times pi, representing one full turn in radians.
    constexpr double Deg2Rad    = Pi / 180.0;               ///< Conversion factor from degrees to radians.
    constexpr double Rad2Deg    = 180.0 / Pi;               ///< Conversion factor from radians to degrees.

    /**
     * @brief Converts radians to degrees.
     * 
     * This function takes an angle in radians and converts it to degrees.
     * 
     * @tparam T The type of the angle (should be a floating-point type).
     * @param radians The angle in radians to be converted.
     * @return The equivalent angle in degrees.
     */
    template <typename T>
    inline constexpr T RadToDeg(T radians)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return radians * Rad2Deg; // Rad2Deg is assumed to be a constant representing the conversion factor from radians to degrees
    }

    /**
     * @brief Converts degrees to radians.
     * 
     * This function takes an angle in degrees and converts it to radians.
     * 
     * @tparam T The type of the angle (should be a floating-point type).
     * @param degrees The angle in degrees to be converted.
     * @return The equivalent angle in radians.
     */
    template <typename T>
    inline constexpr T DegToRad(T degrees)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return degrees * Deg2Rad; // Deg2Rad is assumed to be a constant representing the conversion factor from degrees to radians
    }

    /**
     * @brief Computes the fractional part of a floating-point number.
     * 
     * This function calculates the fractional part of a floating-point number.
     * For example, Fract(3.14) returns 0.14.
     * 
     * @tparam T The type of the value (should be a floating-point type).
     * @param value The value whose fractional part is to be computed.
     * @return The fractional part of the input value.
     */
    template <typename T>
    inline constexpr T Fract(T value)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return value - static_cast<int>(value); // Subtracting the integer part to get the fractional part
    }

    /**
     * @brief Computes the sign of a value.
     * 
     * This function determines the sign of a numeric value.
     * It returns -1 if the value is negative, 0 if the value is zero, and 1 if the value is positive.
     * 
     * @tparam T The type of the value.
     * @param value The value whose sign is to be computed.
     * @return -1 if value is negative, 0 if value is zero, and 1 if value is positive.
     */
    template <typename T>
    inline constexpr Sint8 Sign(T value)
    {
        return (T(0) < value) - (value < T(0));
    }

    /**
     * @brief Computes the sign of each component of a 2D vector.
     * 
     * This function determines the sign of each component of a 2D vector.
     * It returns a 2D vector where each component indicates the sign of the corresponding component in the input vector.
     * 
     * @tparam T The type of the components in the input vector.
     * @param v The input 2D vector.
     * @return A 2D vector where each component indicates the sign of the corresponding component in the input vector.
     */
    template <typename T>
    inline constexpr Vector2<Sint8> Sign(const Vector2<T>& v)
    {
        return Vector2<Sint8>(Sign(v.x), Sign(v.y));
    }

    /**
     * @brief Computes the sign of each component of a 3D vector.
     * 
     * This function determines the sign of each component of a 3D vector.
     * It returns a 3D vector where each component indicates the sign of the corresponding component in the input vector.
     * 
     * @tparam T The type of the components in the input vector.
     * @param v The input 3D vector.
     * @return A 3D vector where each component indicates the sign of the corresponding component in the input vector.
     */
    template <typename T>
    inline constexpr Vector3<Sint8> Sign(const Vector3<T>& v)
    {
        return Vector3<Sint8>(Sign(v.x), Sign(v.y), Sign(v.z));
    }

    /**
     * @brief Computes the sign of each component of a 4D vector.
     * 
     * This function determines the sign of each component of a 4D vector.
     * It returns a 4D vector where each component indicates the sign of the corresponding component in the input vector.
     * 
     * @tparam T The type of the components in the input vector.
     * @param v The input 4D vector.
     * @return A 4D vector where each component indicates the sign of the corresponding component in the input vector.
     */
    template <typename T>
    inline constexpr Vector4<Sint8> Sign(const Vector4<T>& v)
    {
        return Vector3<Sint8>(Sign(v.x), Sign(v.y), Sign(v.z), Sign(v.w));
    }

    /**
     * @brief Computes the factorial of an integer.
     * 
     * This function calculates the factorial of a non-negative integer.
     * The factorial of a non-negative integer n, denoted by n!, is the product of all positive integers less than or equal to n.
     * For example, Factorial(5) returns 5*4*3*2*1 = 120.
     * 
     * @param n The non-negative integer for which the factorial is to be computed.
     * @return The factorial of the input integer.
     */
    inline constexpr int Factorial(int n)
    {
        return (n == 0 || n == 1) ? 1 : n * Factorial(n - 1);
    }

    /**
     * @brief Finds the next power of two greater than or equal to the given value.
     * 
     * This function calculates the next power of two that is greater than or equal to the given value.
     * If the given value is already a power of two, it returns the value itself.
     * 
     * @param value The input value.
     * @return The next power of two greater than or equal to the input value.
     */
    inline constexpr Uint32 NextPowerOfTwo(Uint32 value)
    {
        if (value == 0) return 1;

        value--;
        value |= value >> 1;
        value |= value >> 2;
        value |= value >> 4;
        value |= value >> 8;
        value |= value >> 16;

        return value + 1;
    }

    /**
     * @brief Finds the previous power of two less than or equal to the given value.
     * 
     * This function calculates the previous power of two that is less than or equal to the given value.
     * If the given value is already a power of two, it returns the value itself.
     * 
     * @param value The input value.
     * @return The previous power of two less than or equal to the input value.
     */
    inline constexpr Uint32 PreviousPowerOfTwo(Uint32 value)
    {
        if (value == 0) return 1;

        value--;
        value |= value >> 1;
        value |= value >> 2;
        value |= value >> 4;
        value |= value >> 8;
        value |= value >> 16;

        return (value + 1) >> 1;
    }

    /**
     * @brief Finds the closest power of two to the given value.
     * 
     * This function calculates the closest power of two to the given value.
     * If the given value is already a power of two, it returns the value itself.
     * 
     * @param value The input value.
     * @return The closest power of two to the input value.
     */
    inline constexpr Uint32 ClosestPowerOfTwo(Uint32 value)
    {
        if (value == 0) return 1;

        Uint32 nextPOT = value;

        nextPOT--;
        nextPOT |= nextPOT >> 1;
        nextPOT |= nextPOT >> 2;
        nextPOT |= nextPOT >> 4;
        nextPOT |= nextPOT >> 8;
        nextPOT |= nextPOT >> 16;
        nextPOT++;

        Uint32 prevPOT = nextPOT >> 1;
        return (nextPOT - value) > (value - prevPOT) ? prevPOT : nextPOT;
    }

    /**
     * @brief Determines whether two floating-point values are approximately equal.
     * 
     * This function checks whether two floating-point values are approximately equal within a specified epsilon value.
     * 
     * @tparam T The type of the values.
     * @param a The first floating-point value.
     * @param b The second floating-point value.
     * @param epsilon The maximum allowed difference between a and b to consider them equal (default is std::numeric_limits<T>::epsilon()).
     * @return True if the absolute difference between a and b is less than epsilon, false otherwise.
     */
    template <typename T>
    inline constexpr bool Approximately(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return std::abs(a - b) < epsilon;
    }

    /**
     * @brief Performs linear interpolation between two values.
     * 
     * This function performs linear interpolation (lerp) between two values.
     * It returns the value that is linearly interpolated between start and end based on the interpolation parameter t.
     * 
     * @tparam T The type of the values.
     * @param start The starting value.
     * @param end The ending value.
     * @param t The interpolation parameter (should be in the range [0, 1]).
     * @return The linearly interpolated value between start and end based on t.
     */
    template <typename T>
    inline constexpr T Lerp(T start, T end, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return start + t * (end - start);
    }

    /**
     * @brief Performs linear interpolation between two 2D vectors.
     * 
     * This function performs linear interpolation (lerp) between two 2D vectors.
     * It returns the vector that is linearly interpolated between a and b based on the interpolation parameter t.
     * 
     * @tparam T The type of the vector components.
     * @param a The starting 2D vector.
     * @param b The ending 2D vector.
     * @param t The interpolation parameter (should be in the range [0, 1]).
     * @return The linearly interpolated 2D vector between a and b based on t.
     */
    template <typename T>
    inline constexpr Vector2<T> Lerp(const Vector2<T>& a, const Vector2<T>& b, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return { a.x + t * (b.x - a.x), a.y + t * (b.y - a.y) };
    }

    /**
     * @brief Performs linear interpolation between two 3D vectors.
     * 
     * This function performs linear interpolation (lerp) between two 3D vectors.
     * It returns the vector that is linearly interpolated between a and b based on the interpolation parameter t.
     * 
     * @tparam T The type of the vector components.
     * @param a The starting 3D vector.
     * @param b The ending 3D vector.
     * @param t The interpolation parameter (should be in the range [0, 1]).
     * @return The linearly interpolated 3D vector between a and b based on t.
     */
    template <typename T>
    inline constexpr Vector3<T> Lerp(const Vector3<T>& a, const Vector3<T>& b, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");

        return {
            a.x + t * (b.x - a.x),
            a.y + t * (b.y - a.y),
            a.z + t * (b.z - a.z)
        };
    }

    /**
     * @brief Performs linear interpolation between two 4D vectors.
     * 
     * This function performs linear interpolation (lerp) between two 4D vectors.
     * It returns the vector that is linearly interpolated between a and b based on the interpolation parameter t.
     * 
     * @tparam T The type of the vector components.
     * @param a The starting 4D vector.
     * @param b The ending 4D vector.
     * @param t The interpolation parameter (should be in the range [0, 1]).
     * @return The linearly interpolated 4D vector between a and b based on t.
     */
    template <typename T>
    inline constexpr Vector4<T> Lerp(const Vector4<T>& a, const Vector4<T>& b, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");

        return {
            a.x + t * (b.x - a.x),
            a.y + t * (b.y - a.y),
            a.z + t * (b.z - a.z),
            a.w + t * (b.w - a.w)
        };
    }

    /**
     * @brief Performs a normalized linear interpolation between two 4D vectors.
     * 
     * This function performs a normalized linear interpolation (NLerp) between two 4D vectors.
     * The interpolation parameter `t` is clamped between 0 and 1.
     * 
     * @tparam T The type of the components in the vectors (should be a floating-point type).
     * @param a The start vector.
     * @param b The end vector.
     * @param t The interpolation parameter. It should be in the range [0, 1].
     * @return The result of the NLerp operation, normalized.
     */
    template <typename T>
    inline constexpr Vector4<T> NLerp(const Vector4<T>& a, const Vector4<T>& b, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");

        return Lerp(a, b, t).Normalized();
    }

    /**
     * @brief Performs a spherical linear interpolation between two quaternions.
     * 
     * This function performs a spherical linear interpolation (SLerp) between two quaternions.
     * The interpolation is performed using the provided interpolation amount `amount`.
     * 
     * @tparam T The type of the components in the quaternions (should be a floating-point type).
     * @param q1 The first quaternion.
     * @param q2 The second quaternion.
     * @param amount The interpolation amount. It should be in the range [0, 1].
     * @return The result of the SLerp operation.
     */
    template <typename T>
    inline constexpr Vector4<T> SLerp(const Vector4<T>& q1, Vector4<T> q2, T amount)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");

        T cosHalfTheta = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
        if (cosHalfTheta < 0) q2 = -q2, cosHalfTheta = -cosHalfTheta;

        if (std::abs(cosHalfTheta) >= static_cast<T>(1.0)) return q1;
        else if (cosHalfTheta > static_cast<T>(0.95)) return NLerp(q1, q2, amount);

        T halfTheta = std::acos(cosHalfTheta);
        T sinHalfTheta = std::sqrt(static_cast<T>(1.0) - cosHalfTheta * cosHalfTheta);

        if (std::abs(sinHalfTheta) < static_cast<T>(0.000001)) return (q1 + q2) * 0.5f;

        T ratioA = std::sin((1 - amount) * halfTheta) / sinHalfTheta;
        T ratioB = std::sin(amount * halfTheta) / sinHalfTheta;
        return q1 * ratioA + q2 * ratioB;
    }

    /**
     * @brief Linearly interpolates between two colors.
     * 
     * This function performs linear interpolation between two colors `a` and `b`
     * based on the interpolation factor `t`.
     * 
     * @tparam T The type of the interpolation factor (should be a floating-point type).
     * @param a The starting color.
     * @param b The ending color.
     * @param t The interpolation factor. Value of 0 corresponds to `a`, 1 corresponds to `b`.
     * @return The interpolated color.
     */
    template <typename T>
    inline constexpr gfx::Color Lerp(const gfx::Color& a, const gfx::Color& b, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");

        return {
            static_cast<Uint8>(a.r + t * (b.r - a.r)),
            static_cast<Uint8>(a.g + t * (b.g - a.g)),
            static_cast<Uint8>(a.b + t * (b.b - a.b)),
            static_cast<Uint8>(a.a + t * (b.a - a.a))
        };
    }

    /**
     * @brief Performs smooth interpolation between two values using Hermite interpolation.
     * 
     * This function performs smooth interpolation between two values `a` and `b`
     * based on the interpolation factor `t`, using Hermite interpolation.
     * 
     * @tparam T The type of the values (should be a floating-point type).
     * @param a The starting value.
     * @param b The ending value.
     * @param t The interpolation factor. Value of 0 corresponds to `a`, 1 corresponds to `b`.
     * @return The smoothly interpolated value.
     */
    template <typename T>
    inline constexpr T SmoothStep(T a, T b, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return Lerp(a, b, t * t * (static_cast<T>(3.0) - static_cast<T>(2.0) * t));
    }

    /**
     * @brief Performs smooth interpolation between two 2D vectors using Hermite interpolation.
     * 
     * This function performs smooth interpolation between two 2D vectors `a` and `b`
     * based on the interpolation factor `t`, using Hermite interpolation.
     * 
     * @tparam T The type of the vector components (should be a floating-point type).
     * @param a The starting 2D vector.
     * @param b The ending 2D vector.
     * @param t The interpolation factor. Value of 0 corresponds to `a`, 1 corresponds to `b`.
     * @return The smoothly interpolated 2D vector.
     */
    template <typename T>
    inline constexpr Vector2<T> SmoothStep(const Vector2<T>& a, const Vector2<T>& b, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return Lerp(a, b, t * t * (static_cast<T>(3.0) - static_cast<T>(2.0) * t));
    }

    /**
     * @brief Performs smooth interpolation between two 3D vectors using Hermite interpolation.
     * 
     * This function performs smooth interpolation between two 3D vectors `a` and `b`
     * based on the interpolation factor `t`, using Hermite interpolation.
     * 
     * @tparam T The type of the vector components (should be a floating-point type).
     * @param a The starting 3D vector.
     * @param b The ending 3D vector.
     * @param t The interpolation factor. Value of 0 corresponds to `a`, 1 corresponds to `b`.
     * @return The smoothly interpolated 3D vector.
     */
    template <typename T>
    inline constexpr Vector3<T> SmoothStep(const Vector3<T>& a, const Vector3<T>& b, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return Lerp(a, b, t * t * (static_cast<T>(3.0) - static_cast<T>(2.0) * t));
    }

    /**
     * @brief Performs smooth interpolation between two 4D vectors using Hermite interpolation.
     * 
     * This function performs smooth interpolation between two 4D vectors `a` and `b`
     * based on the interpolation factor `t`, using Hermite interpolation.
     * 
     * @tparam T The type of the vector components (should be a floating-point type).
     * @param a The starting 4D vector.
     * @param b The ending 4D vector.
     * @param t The interpolation factor. Value of 0 corresponds to `a`, 1 corresponds to `b`.
     * @return The smoothly interpolated 4D vector.
     */
    template <typename T>
    inline constexpr Vector4<T> SmoothStep(const Vector4<T>& a, const Vector4<T>& b, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return Lerp(a, b, t * t * (static_cast<T>(3.0) - static_cast<T>(2.0) * t));
    }

    /**
     * @brief Performs smooth interpolation between two colors using Hermite interpolation.
     * 
     * This function performs smooth interpolation between two colors `a` and `b`
     * based on the interpolation factor `t`, using Hermite interpolation.
     * 
     * @tparam T The type of the color components (should be a floating-point type).
     * @param a The starting color.
     * @param b The ending color.
     * @param t The interpolation factor. Value of 0 corresponds to `a`, 1 corresponds to `b`.
     * @return The smoothly interpolated color.
     */
    template <typename T>
    inline constexpr gfx::Color SmoothStep(const gfx::Color& a, const gfx::Color& b, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return Lerp(a, b, t * t * (static_cast<T>(3.0) - static_cast<T>(2.0) * t));
    }

    /**
     * @brief Normalizes a value within a specified range.
     * 
     * This function normalizes the value within the range defined by `start` and `end`.
     * 
     * @tparam T The type of the value and the range (should be a floating-point type).
     * @param value The value to normalize.
     * @param start The start of the range.
     * @param end The end of the range.
     * @return The normalized value within the range [0, 1].
     */
    template <typename T>
    inline constexpr T Normalize(T value, T start, T end)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return (value - start) / (end - start);
    }

    /**
     * @brief Normalizes a 2D vector.
     * 
     * This function normalizes the given 2D vector `v`.
     * 
     * @tparam T The type of the vector components (should be a floating-point type).
     * @param v The 2D vector to normalize.
     * @return The normalized 2D vector.
     */
    template <typename T>
    inline constexpr Vector2<T> Normalize(const Vector2<T>& v)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return v.Normalized();
    }

    /**
     * @brief Normalizes a 3D vector.
     * 
     * This function normalizes the given 3D vector `v`.
     * 
     * @tparam T The type of the vector components (should be a floating-point type).
     * @param v The 3D vector to normalize.
     * @return The normalized 3D vector.
     */
    template <typename T>
    inline constexpr Vector3<T> Normalize(const Vector3<T>& v)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return v.Normalized();
    }

    /**
     * @brief Normalizes a 4D vector.
     * 
     * This function normalizes the given 4D vector `v`.
     * 
     * @tparam T The type of the vector components (should be a floating-point type).
     * @param v The 4D vector to normalize.
     * @return The normalized 4D vector.
     */
    template <typename T>
    inline constexpr Vector4<T> Normalize(const Vector4<T>& v)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return v.Normalized();
    }

    /**
     * @brief Ortho-normalizes two 3D vectors using Gram-Schmidt process.
     * 
     * This function ortho-normalizes two 3D vectors `v1` and `v2` using the Gram-Schmidt process.
     * After ortho-normalization, `v1` remains unchanged, while `v2` becomes orthogonal to `v1`
     * and both `v1` and `v2` are normalized (have a magnitude of 1).
     * 
     * @tparam T The type of the vector components (should be a floating-point type).
     * @param v1 The first 3D vector.
     * @param v2 The second 3D vector to be ortho-normalized. After the operation, it becomes orthogonal to `v1`.
     */
    template <typename T>
    inline constexpr void OrthoNormalize(Vector3<T>& v1, Vector3<T>& v2)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        v1.Normalize(); v2 = v1.Cross(v2).Normalized().Cross(v1);
    }

    /**
     * @brief Computes the cross product of two 3D vectors.
     * 
     * This function computes the cross product of two 3D vectors `v1` and `v2`.
     * The cross product yields a vector that is perpendicular to both `v1` and `v2`.
     * 
     * @tparam T The type of the vector components (should be a floating-point type).
     * @param v1 The first 3D vector.
     * @param v2 The second 3D vector.
     * @return The cross product vector of `v1` and `v2`.
     */
    template <typename T>
    inline constexpr Vector3<T> Cross(const Vector3<T>& v1, const Vector3<T>& v2)
    {
        return v1.Cross(v2);
    }

    /**
     * @brief Computes the dot product of two 2D vectors.
     * 
     * This function computes the dot product of two 2D vectors `v1` and `v2`.
     * The dot product is the sum of the products of the corresponding components of the vectors.
     * 
     * @tparam T The type of the vector components (should be a floating-point type).
     * @param v1 The first 2D vector.
     * @param v2 The second 2D vector.
     * @return The dot product of `v1` and `v2`.
     */
    template <typename T>
    inline constexpr T Dot(const Vector2<T>& v1, const Vector2<T>& v2)
    {
        return v1.Dot(v2);
    }

    /**
     * @brief Computes the dot product of two 3D vectors.
     * 
     * This function computes the dot product of two 3D vectors `v1` and `v2`.
     * The dot product is the sum of the products of the corresponding components of the vectors.
     * 
     * @tparam T The type of the vector components (should be a floating-point type).
     * @param v1 The first 3D vector.
     * @param v2 The second 3D vector.
     * @return The dot product of `v1` and `v2`.
     */
    template <typename T>
    inline constexpr T Dot(const Vector3<T>& v1, const Vector3<T>& v2)
    {
        return v1.Dot(v2);
    }

    /**
     * @brief Computes the dot product of two 4D vectors.
     * 
     * This function computes the dot product of two 4D vectors `v1` and `v2`.
     * The dot product is the sum of the products of the corresponding components of the vectors.
     * 
     * @tparam T The type of the vector components (should be a floating-point type).
     * @param v1 The first 4D vector.
     * @param v2 The second 4D vector.
     * @return The dot product of `v1` and `v2`.
     */
    template <typename T>
    inline constexpr T Dot(const Vector4<T>& v1, const Vector4<T>& v2)
    {
        return v1.Dot(v2);
    }

    /**
     * @brief Remaps a value from one range to another linearly.
     * 
     * This function remaps a value `value` from the input range defined by `inStart` and `inEnd`
     * to the output range defined by `outStart` and `outEnd`. The remapping is done linearly.
     * 
     * @tparam T The type of the value and the ranges (should be a floating-point type).
     * @param value The value to remap.
     * @param inStart The start of the input range.
     * @param inEnd The end of the input range.
     * @param outStart The start of the output range.
     * @param outEnd The end of the output range.
     * @return The remapped value in the output range.
     */
    template <typename T>
    inline constexpr T Remap(T value, T inStart, T inEnd, T outStart, T outEnd)
    {
        return (value - inStart) / (inEnd - inStart) * (outEnd - outStart) + outStart;
    }

    /**
     * @brief Wraps a value within a range.
     * 
     * This function wraps a value `value` within the range defined by `min` and `max`.
     * If `value` exceeds the range, it wraps around to the other end of the range.
     * 
     * @tparam T The type of the value and the range (should be a floating-point type).
     * @param value The value to wrap.
     * @param min The minimum value of the range.
     * @param max The maximum value of the range.
     * @return The wrapped value within the range [min, max].
     */
    template <typename T>
    inline constexpr T Wrap(T value, T min, T max)
    {
        return value - (max - min) * std::floor((value - min) / (max - min));
    }

    /**
     * @brief Wraps an angle in radians to the range from -π to π.
     * 
     * This function wraps an angle `th` in radians to the range from -π to π.
     * If the angle exceeds this range, it wraps around accordingly.
     * 
     * @tparam T The type of the angle (should be a floating-point type).
     * @param th The angle to wrap.
     * @return The wrapped angle within the range [-π, π].
     */
    template <typename T>
    inline constexpr T WrapMinusPiToPi(T th)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return std::atan2(std::sin(th), std::cos(th));
    }

    /**
     * @brief Computes the angular difference between two angles in radians.
     * 
     * This function computes the angular difference between two angles `current` and `target` given in radians.
     * It returns the difference angle, which may be positive or negative depending on the direction of rotation.
     * 
     * @tparam T The type of the angles (should be a floating-point type).
     * @param current The current angle in radians.
     * @param target The target angle in radians.
     * @return The angular difference between `current` and `target`.
     */
    template <typename T>
    inline constexpr T DeltaRad(T current, T target)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        const float c0 = std::cos(current), s0 = std::sin(current);
        const float c1 = std::cos(target), s1 = std::sin(target);
        return std::atan2(c0 * s1 - c1 * s0, c0 * c1 + s1 * s0);
    }

    /**
     * @brief Performs linear interpolation (lerp) between two angles in radians.
     * 
     * This function performs linear interpolation (lerp) between two angles `start` and `end` given in radians,
     * using the parameter `t` to determine the interpolation factor.
     * 
     * @tparam T The type of the angles (should be a floating-point type).
     * @param start The starting angle in radians.
     * @param end The ending angle in radians.
     * @param t The interpolation factor between 0 and 1.
     * @return The interpolated angle between `start` and `end`.
     */
    template <typename T>
    inline constexpr T LerpRad(T start, T end, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        start = WrapMinusPiToPi(start), end = WrapMinusPiToPi(end);
        return WrapMinusPiToPi(start + t * DeltaAngle(start, end));
    }

    /**
     * @brief Performs smooth step interpolation between two angles in radians.
     * 
     * This function performs smooth step interpolation between two angles `start` and `end` given in radians,
     * using the parameter `t` to determine the interpolation factor.
     * 
     * @tparam T The type of the angles (should be a floating-point type).
     * @param start The starting angle in radians.
     * @param end The ending angle in radians.
     * @param t The interpolation factor between 0 and 1.
     * @return The smoothly interpolated angle between `start` and `end`.
     */
    template <typename T>
    inline constexpr T SmoothStepRad(T start, T end, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return LerpRad(start, end, t * t * (static_cast<T>(3.0) - static_cast<T>(2.0) * t));
    }

    /**
     * @brief Wraps an angle to the range [0, 360).
     * 
     * This function wraps an angle `angle` to the range [0, 360) degrees.
     * If the angle is negative, it wraps around to the positive range.
     * 
     * @tparam T The type of the angle.
     * @param angle The angle to wrap in degrees.
     * @return The wrapped angle within the range [0, 360).
     */
    template <typename T>
    inline constexpr T WrapTo360(T angle)
    {
        return std::fmod(std::abs(angle), static_cast<T>(360.0));
    }

    /**
     * @brief Computes the angular difference between two angles in degrees.
     * 
     * This function computes the angular difference between two angles `current` and `target`
     * given in degrees. It returns the difference angle in degrees, which may be positive or negative
     * depending on the direction of rotation.
     * 
     * @tparam T The type of the angles (should be a floating-point type).
     * @param current The current angle in degrees.
     * @param target The target angle in degrees.
     * @return The angular difference between `current` and `target` in degrees.
     */
    template <typename T>
    inline constexpr T DeltaDeg(T current, T target)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return DeltaRad(current * Deg2Rad, target * Deg2Rad) * Rad2Deg;
    }

    /**
     * @brief Performs linear interpolation (lerp) between two angles in degrees.
     * 
     * This function performs linear interpolation (lerp) between two angles `start` and `end`
     * given in degrees, using the parameter `t` to determine the interpolation factor.
     * 
     * @tparam T The type of the angles (should be a floating-point type).
     * @param start The starting angle in degrees.
     * @param end The ending angle in degrees.
     * @param t The interpolation factor between 0 and 1.
     * @return The interpolated angle between `start` and `end`.
     */
    template <typename T>
    inline constexpr T LerpDeg(T start, T end, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return std::fmod(start + t * DeltaDeg(start, end) + 360.0f, 360.0f);
    }

    /**
    * @brief Performs smooth step interpolation between two angles in degrees.
    * 
    * This function performs smooth step interpolation between two angles `start` and `end`
    * given in degrees, using the parameter `t` to determine the interpolation factor.
    * 
    * @tparam T The type of the angles (should be a floating-point type).
    * @param start The starting angle in degrees.
    * @param end The ending angle in degrees.
    * @param t The interpolation factor between 0 and 1.
    * @return The smoothly interpolated angle between `start` and `end`.
    */
    template <typename T>
    inline constexpr T SmoothStepDeg(T start, T end, T t)
    {
        static_assert(std::is_floating_point<T>::value, "Only floating-point types are allowed.");
        return LerpDeg(start, end, t * t * (static_cast<T>(3.0) - static_cast<T>(2.0) * t));
    }

}}

#endif //NEXUS_MATH_HPP