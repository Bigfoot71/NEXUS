#ifndef NEXUS_MATH_VEC2_HPP
#define NEXUS_MATH_VEC2_HPP

#include "../platform/nxPlatform.hpp"

#ifdef SUPPORT_PHYSICS_2D
#   include <box2d/b2_math.h>
#endif

#include "./nxMat3.hpp"
#include <SDL_rect.h>
#include <algorithm>
#include <cmath>

namespace nexus { namespace math {

    /**
     * @brief Represents a 2-dimensional vector.
     *
     * This class template represents a 2-dimensional vector with components x and y.
     * It provides various mathematical operations and utilities for manipulating 2D vectors.
     *
     * @tparam T The type of the components of the vector.
     */
    template <typename T>
    struct NEXUS_API Vector2
    {
        static constexpr Uint8 Dimensions = 2;  ///< The number of dimensions of the vector.
        typedef T value_type;                   ///< The type of each component of the vector.
        T x, y;                                 ///< The x and y components of the vector.

        /**
         * @brief Default constructor.
         *
         * Initializes both components x and y to zero.
         */
        constexpr Vector2()
        : x(0), y(0)
        { }

        /**
         * @brief Constructor initializing both components with a single value.
         *
         * Initializes both components x and y with the same given value.
         *
         * @param value The value to set for both x and y components.
         */
        constexpr explicit Vector2(T value)
        : x(value), y(value)
        { }

        /**
         * @brief Constructor initializing both components with specific values.
         *
         * Initializes the x component with the given x value and the y component with the given y value.
         *
         * @param x The value to set for the x component.
         * @param y The value to set for the y component.
         */
        constexpr Vector2(T x, T y)
        : x(x), y(y)
        { }

        /**
         * @brief Constructor initializing the vector from an SDL_Point.
         *
         * Initializes the components x and y of the vector with the x and y values of the given SDL_Point.
         *
         * @param p The SDL_Point containing the x and y values to initialize the vector with.
         */
        constexpr Vector2(const SDL_Point& p)
        : x(p.x), y(p.y)
        { }

        /**
         * @brief Constructor initializing the vector from an SDL_FPoint.
         *
         * Initializes the components x and y of the vector with the x and y values of the given SDL_FPoint.
         *
         * @param p The SDL_FPoint containing the x and y values to initialize the vector with.
         */
        constexpr Vector2(const SDL_FPoint& p)
        : x(p.x), y(p.y)
        { }

#       ifdef SUPPORT_PHYSICS_2D

        /**
         * @brief Constructor initializing the vector from a b2Vec2 (Box2D vector).
         *
         * Initializes the components x and y of the vector with the x and y values of the given b2Vec2.
         *
         * @param vec The b2Vec2 containing the x and y values to initialize the vector with.
         */
        constexpr Vector2(const b2Vec2& vec) : x(vec.x), y(vec.y) { }

        /**
         * @brief Conversion operator to convert the vector to a b2Vec2 (Box2D vector).
         *
         * Converts the current vector to a b2Vec2 with the same x and y values.
         *
         * @return A b2Vec2 containing the x and y values of the current vector.
         */
        constexpr operator b2Vec2() const { return b2Vec2(x, y); }

#       endif

        /**
         * @brief Conversion operator to convert the vector to a Vector2 of a different type.
         *
         * Converts the current vector to a Vector2 of a different type by casting its components to the new type.
         *
         * @tparam U The type to convert the components of the vector to.
         * @return A Vector2 containing the components of the current vector casted to the type U.
         */
        template <typename U>
        constexpr operator Vector2<U>() const
        {
            return Vector2<U>(static_cast<U>(x), static_cast<U>(y));
        }

        /**
         * @brief Conversion operator to convert the vector to an SDL_Point.
         *
         * Converts the current vector to an SDL_Point by casting its components to integers.
         *
         * @return An SDL_Point with the integer components of the current vector.
         */
        constexpr operator SDL_Point() const
        {
            return SDL_Point{ static_cast<int>(x), static_cast<int>(y) };
        }

        /**
         * @brief Conversion operator to convert the vector to an SDL_FPoint.
         *
         * Converts the current vector to an SDL_FPoint by casting its components to floats.
         *
         * @return An SDL_FPoint with the float components of the current vector.
         */
        constexpr operator SDL_FPoint() const
        {
            return SDL_FPoint{ static_cast<float>(x), static_cast<float>(y) };
        }

        /**
         * @brief Subscript operator to access the components of the vector.
         *
         * Allows accessing the components of the vector using array-style indexing.
         *
         * @param axis The index of the component to access (0 for x, 1 for y).
         * @return A reference to the component at the specified index.
         */
        constexpr T& operator[](int axis)
        {
            return *(reinterpret_cast<T*>(this) + axis);
        }

        /**
         * @brief Subscript operator to access the components of the vector (const version).
         *
         * Allows accessing the components of the vector using array-style indexing.
         * This is the const version of the subscript operator.
         *
         * @param axis The index of the component to access (0 for x, 1 for y).
         * @return A const reference to the component at the specified index.
         */
        constexpr const T& operator[](int axis) const
        {
            return *(reinterpret_cast<const T*>(this) + axis);
        }

        /**
         * @brief Unary negation operator.
         *
         * Returns the negation of the current vector by negating each component.
         *
         * @return The negation of the current vector.
         */
        constexpr Vector2 operator-() const
        {
            return { -x, -y };
        }

        /**
         * @brief Subtraction operator with a scalar.
         *
         * Subtracts the given scalar value from each component of the current vector.
         *
         * @param scalar The scalar value to subtract from each component.
         * @return A new vector resulting from the subtraction operation.
         */
        constexpr Vector2 operator-(T scalar) const
        {
            return Vector2(x - scalar, y - scalar);
        }

        /**
         * @brief Addition operator with a scalar.
         *
         * Adds the given scalar value to each component of the current vector.
         *
         * @param scalar The scalar value to add to each component.
         * @return A new vector resulting from the addition operation.
         */
        constexpr Vector2 operator+(T scalar) const
        {
            return Vector2(x + scalar, y + scalar);
        }

        /**
         * @brief Multiplication operator with a scalar.
         *
         * Multiplies each component of the current vector by the given scalar value.
         *
         * @param scalar The scalar value to multiply each component by.
         * @return A new vector resulting from the multiplication operation.
         */
        constexpr Vector2 operator*(T scalar) const
        {
            return Vector2(x * scalar, y * scalar);
        }

        /**
         * @brief Division operator by a scalar.
         *
         * Divides each component of the current vector by the given scalar value.
         *
         * @param scalar The scalar value to divide each component by.
         * @return A new vector resulting from the division operation.
         *         If the scalar is zero, returns a zero vector.
         */
        constexpr Vector2 operator/(T scalar) const
        {
            if (scalar == 0) return { };

            if constexpr(std::is_floating_point<T>::value)
            {
                const T inv = static_cast<T>(1.0) / scalar;
                return Vector2(x * inv, y * inv);
            }

            return Vector2(x / scalar, y / scalar);
        }

        /**
         * @brief Subtraction operator between vectors.
         *
         * Subtracts each component of the other vector from the corresponding component of the current vector.
         *
         * @param other The vector to subtract from the current vector.
         * @return A new vector resulting from the subtraction operation.
         */
        constexpr Vector2 operator-(const Vector2& other) const
        {
            return Vector2(x - other.x, y - other.y);
        }

        /**
         * @brief Addition operator between vectors.
         *
         * Adds each component of the other vector to the corresponding component of the current vector.
         *
         * @param other The vector to add to the current vector.
         * @return A new vector resulting from the addition operation.
         */
        constexpr Vector2 operator+(const Vector2& other) const
        {
            return Vector2(x + other.x, y + other.y);
        }

        /**
         * @brief Multiplication operator between vectors.
         *
         * Multiplies each component of the other vector by the corresponding component of the current vector.
         *
         * @param other The vector to multiply with the current vector.
         * @return A new vector resulting from the multiplication operation.
         */
        constexpr Vector2 operator*(const Vector2& other) const
        {
            return Vector2(x * other.x, y * other.y);
        }

        /**
         * @brief Division operator between vectors.
         *
         * Divides each component of the current vector by the corresponding component of the other vector.
         * If any component of the other vector is zero, the result for that component is set to zero.
         *
         * @param other The vector by which to divide the current vector.
         * @return A new vector resulting from the division operation.
         */
        constexpr Vector2 operator/(const Vector2& other) const
        {
            return Vector2(
                other.x ? x / other.x : 0,
                other.y ? y / other.y : 0);
        }

        /**
         * @brief Equality operator.
         *
         * Checks if each component of the current vector is equal to the corresponding component of the other vector.
         *
         * @param other The vector to compare with the current vector.
         * @return True if the vectors are equal (i.e., all components are equal), false otherwise.
         */
        constexpr bool operator==(const Vector2& other) const
        {
            return (x == other.x) && (y == other.y);
        }

        /**
         * @brief Inequality operator.
         *
         * Checks if any component of the current vector is not equal to the corresponding component of the other vector.
         *
         * @param other The vector to compare with the current vector.
         * @return True if the vectors are not equal (i.e., any component is not equal), false otherwise.
         */
        constexpr bool operator!=(const Vector2& other) const
        {
            return (x != other.x) || (y != other.y);
        }

        /**
         * @brief Subtraction and assignment operator with a scalar.
         *
         * Subtracts the given scalar value from each component of the current vector and assigns the result back to the current vector.
         *
         * @param scalar The scalar value to subtract from each component.
         * @return A reference to the modified current vector.
         */
        Vector2& operator-=(T scalar)
        {
            x -= scalar;
            y -= scalar;
            return *this;
        }

        /**
         * @brief Addition and assignment operator with a scalar.
         *
         * Adds the given scalar value to each component of the current vector and assigns the result back to the current vector.
         *
         * @param scalar The scalar value to add to each component.
         * @return A reference to the modified current vector.
         */
        Vector2& operator+=(T scalar)
        {
            x += scalar;
            y += scalar;
            return *this;
        }

        /**
         * @brief Multiplication and assignment operator with a scalar.
         *
         * Multiplies each component of the current vector by the given scalar value and assigns the result back to the current vector.
         *
         * @param scalar The scalar value to multiply each component by.
         * @return A reference to the modified current vector.
         */
        Vector2& operator*=(T scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        /**
         * @brief Division and assignment operator with a scalar.
         *
         * Divides each component of the current vector by the given scalar value and assigns the result back to the current vector.
         * If the scalar is zero, sets the vector to a zero vector.
         *
         * @param scalar The scalar value to divide each component by.
         * @return A reference to the modified current vector.
         */
        Vector2& operator/=(T scalar)
        {
            if (scalar == 0) { *this = { }; return *this; }

            if constexpr(std::is_floating_point<T>::value)
            {
                const T inv = static_cast<T>(1.0) / scalar;
                x *= inv, y *= inv;
                return *this;
            }

            x /= scalar, y /= scalar;

            return *this;
        }

        /**
         * @brief Subtraction and assignment operator.
         *
         * Subtracts each component of the other vector from the corresponding component of the current vector and assigns the result back to the current vector.
         *
         * @param other The vector to subtract from the current vector.
         * @return A reference to the modified current vector.
         */
        Vector2& operator-=(const Vector2& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        /**
         * @brief Addition and assignment operator.
         *
         * Adds each component of the other vector to the corresponding component of the current vector and assigns the result back to the current vector.
         *
         * @param other The vector to add to the current vector.
         * @return A reference to the modified current vector.
         */
        Vector2& operator+=(const Vector2& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        /**
         * @brief Multiplication and assignment operator with another vector.
         *
         * Multiplies each component of the current vector by the corresponding component of the other vector and assigns the result back to the current vector.
         *
         * @param other The vector to multiply with the current vector.
         * @return A reference to the modified current vector.
         */
        Vector2& operator*=(const Vector2& other)
        {
            x *= other.x;
            y *= other.y;
            return *this;
        }

        /**
         * @brief Division and assignment operator with another vector.
         *
         * Divides each component of the current vector by the corresponding non-zero component of the other vector
         * and assigns the result back to the current vector.
         *
         * @param other The vector by which to divide the current vector.
         * @return A reference to the modified current vector.
         */
        Vector2& operator/=(const Vector2& other)
        {
            if (other.x != 0.0f) x /= other.x;
            if (other.y != 0.0f) y /= other.y;
            return *this;
        }

        /**
         * @brief Method to check if the vector is equal to (0,0).
         *
         * @return True if the vector is equal to (0,0), false otherwise.
         */
        constexpr bool IsZero() const
        {
            return !(x + y);
        }

        /**
         * @brief Method to calculate the magnitude (length) of the vector.
         *
         * @return The magnitude (length) of the vector.
         */
        double Magnitude() const
        {
            return std::sqrt(x * x + y * y);
        }

        /**
         * @brief Function to calculate the length (magnitude) of the vector.
         *
         * @return The length (magnitude) of the vector.
         */
        double Length() const
        {
            return Magnitude();
        }

        /**
         * @brief Function to calculate the squared length (magnitude) of the vector.
         *
         * @return The squared length (magnitude) of the vector.
         */
        constexpr T LengthSqr() const
        {
            return x * x + y * y;
        }

        /**
         * @brief Method to calculate the dot product of the vector with another vector.
         *
         * @param other The other vector to calculate the dot product with.
         * @return The dot product of the two vectors.
         */
        constexpr T Dot(const Vector2& other) const
        {
            return x * other.x + y * other.y;
        }

        /**
         * @brief Method to normalize the vector.
         *
         * Normalizes the current vector, making it a unit vector (a vector with a magnitude of 1).
         * If the magnitude of the vector is zero, no operation is performed.
         */
        void Normalize()
        {
            const double mag = Magnitude();
            if (mag != 0.0) (*this) *= 1.0 / mag;
        }

        /**
         * @brief Method to get a normalized vector.
         *
         * Returns a new vector that is the normalized version of the current vector.
         *
         * @return A normalized vector.
         */
        Vector2 Normalized() const
        {
            Vector2 result(*this);
            result.Normalize();
            return result;
        }

        /**
         * @brief Method to calculate the Euclidean distance between two vectors.
         *
         * @param other The other vector to calculate the distance to.
         * @return The Euclidean distance between the two vectors.
         */
        double Distance(const Vector2& other) const
        {
            return (*this - other).Magnitude();
        }

        /**
         * @brief Function to calculate the squared Euclidean distance between two vectors.
         *
         * @param other The other vector to calculate the squared distance to.
         * @return The squared Euclidean distance between the two vectors.
         */
        constexpr T DistanceSqr(const Vector2& other) const
        {
            const Vector2 diff = *this - other;
            return diff.x * diff.x + diff.y * diff.y;
        }

        /**
         * @brief Function to rotate the vector around the origin by an angle in radians.
         *
         * @param angle The angle in radians by which to rotate the vector.
         */
        void Rotate(double angle)
        {
            const double c = std::cos(angle);
            const double s = std::sin(angle);
            x = x * c - y * s, y = x * s + y * c;
        }

        /**
         * @brief Function to get a rotated copy of the vector around the origin by an angle in radians.
         *
         * @param angle The angle in radians by which to rotate the vector.
         * @return A rotated copy of the vector.
         */
        Vector2 Rotated(double angle) const
        {
            const double c = std::cos(angle);
            const double s = std::sin(angle);
            return Vector2(x * c - y * s, x * s + y * c);
        }

        /**
         * @brief Function to rotate the vector around the origin by given cosine and sine values.
         *
         * @param c The cosine value of the rotation angle.
         * @param s The sine value of the rotation angle.
         */
        void Rotate(double c, double s)
        {
            x = x * c - y * s, y = x * s + y * c;
        }

        /**
         * @brief Function to get a rotated copy of the vector around the origin by given cosine and sine values.
         *
         * @param c The cosine value of the rotation angle.
         * @param s The sine value of the rotation angle.
         * @return A rotated copy of the vector.
         */
        Vector2 Rotated(double c, double s) const
        {
            return Vector2(x * c - y * s, x * s + y * c);
        }

        /**
         * @brief Function to reflect the vector with respect to a given normal vector.
         *
         * Reflects the vector with respect to the provided normal vector assuming 'normal' is a unit vector.
         * The reflection is computed using the formula: v - 2 * dot(v, normal) * normal.
         *
         * @param normal The normal vector used for reflection.
         * @return The reflected vector.
         */
        constexpr Vector2 Reflect(const Vector2& normal) const
        {
            double dot = this->Dot(normal);

            return Vector2(
                x - 2.0 * dot * normal.x,
                y - 2.0 * dot * normal.y
            );
        }

        /**
         * @brief Function to calculate the angle in radians of the vector with respect to the positive x-axis.
         *
         * @return The angle in radians of the vector with respect to the positive x-axis.
         */
        double Angle() const
        {
            return std::atan2(y, x);
        }

        /**
         * @brief Function to calculate the angle in radians between two vectors with respect to the positive x-axis.
         *
         * @param other The other vector to calculate the angle with respect to.
         * @return The angle in radians between the two vectors with respect to the positive x-axis.
         */
        double Angle(const Vector2& other) const
        {
            return std::atan2(y - other.y, x - other.x);
        }

        /**
         * @brief Function to transform the vector by a 2D transformation matrix.
         *
         * Transforms the vector using the provided 2D transformation matrix.
         * The transformation is applied as a 2x3 matrix multiplication: [x' y'] = [x y 1] * matrix.
         *
         * @param matrix The 2D transformation matrix to apply.
         */
        constexpr void Transform(const Mat3& matrix)
        {
            *this = {
                x * matrix.m[0] + y * matrix.m[3] + matrix.m[6],
                x * matrix.m[1] + y * matrix.m[4] + matrix.m[7]
            };
        }

        /**
         * @brief Function to transform the vector by a 2D transformation matrix and return the result.
         *
         * Transforms the vector using the provided 2D transformation matrix and returns the transformed vector.
         * The transformation is applied as a 2x3 matrix multiplication: [x' y'] = [x y 1] * matrix.
         *
         * @param matrix The 2D transformation matrix to apply.
         * @return The transformed vector.
         */
        constexpr Vector2 Transformed(const Mat3& matrix)
        {
            return {
                x * matrix.m[0] + y * matrix.m[3] + matrix.m[6],
                x * matrix.m[1] + y * matrix.m[4] + matrix.m[7]
            };
        }

        /**
         * @brief Function to calculate the component-wise minimum between the current vector and another vector.
         *
         * @param other The other vector to compare with.
         * @return A vector where each component is the minimum of the corresponding components of the current vector and the other vector.
         */
        constexpr Vector2 Min(const Vector2& other) const
        {
            return {
                std::min(x, other.x),
                std::min(y, other.y)
            };
        }

        /**
         * @brief Function to calculate the component-wise maximum between the current vector and another vector.
         *
         * @param other The other vector to compare with.
         * @return A vector where each component is the maximum of the corresponding components of the current vector and the other vector.
         */
        constexpr Vector2 Max(const Vector2& other) const
        {
            return {
                std::max(x, other.x),
                std::max(y, other.y)
            };
        }

        /**
         * @brief Function to clamp each component of the current vector within a specified range defined by minimum and maximum vectors.
         *
         * @param min The minimum vector defining the lower bounds for each component.
         * @param max The maximum vector defining the upper bounds for each component.
         * @return A vector where each component is clamped within the specified range.
         */
        constexpr Vector2 Clamp(const Vector2& min, const Vector2& max) const
        {
            return Vector2(
                std::clamp(x, min.x, max.x),
                std::clamp(y, min.y, max.y)
            );
        }

        /**
         * @brief Function to clamp each component of the current vector within a specified scalar range defined by minimum and maximum values.
         *
         * @param min The minimum value defining the lower bounds for each component.
         * @param max The maximum value defining the upper bounds for each component.
         * @return A vector where each component is clamped within the specified scalar range.
         */
        constexpr Vector2 Clamp(T min, T max) const
        {
            return Vector2(
                std::clamp(x, min, max),
                std::clamp(y, min, max)
            );
        }

        /**
         * @brief Function to calculate the absolute value of each component of the vector.
         *
         * @return A vector where each component is the absolute value of the corresponding component of the current vector.
         */
        Vector2 Abs() const
        {
            return Vector2(std::abs(x), std::abs(y));
        }
    };

    // Specific class for 2D floating point vectors
    using Vec2 = Vector2<float>;

    // Specific class for 2D vectors in integers
    using IVec2 = Vector2<int>;

}}

#endif //NEXUS_MATH_VEC2_HPP