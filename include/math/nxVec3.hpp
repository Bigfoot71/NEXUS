#ifndef NEXUS_MATH_VEC3_HPP
#define NEXUS_MATH_VEC3_HPP

#ifdef SUPPORT_MODEL
#   include <assimp/vector3.h>
#endif

#ifdef SUPPORT_PHYSICS_2D
#   include <box2d/b2_math.h>
#endif

#ifdef SUPPORT_PHYSICS_3D
#   include <LinearMath/btVector3.h>
#endif

#include "./nxMat4.hpp"
#include "./nxVec2.hpp"
#include <algorithm>
#include <cmath>

namespace nexus { namespace math {

    /**
     * @brief Represents a 3-dimensional vector.
     *
     * This class template represents a 3-dimensional vector with components x, y, and z.
     * It provides various mathematical operations and utilities for manipulating 3D vectors.
     *
     * @tparam T The type of the components of the vector.
     */
    template <typename T>
    struct NEXUS_API Vector3
    {
        static constexpr Uint8 Dimensions = 3;  ///< The number of dimensions of the vector.
        typedef T value_type;                   ///< The type of each component of the vector.
        T x, y, z;                              ///< The x, y, and z components of the vector.

        /**
         * @brief Default constructor.
         *
         * Initializes all components x, y, and z to zero.
         */
        constexpr Vector3()
        : x(0), y(0), z(0)
        { }

        /**
         * @brief Constructor initializing all components with a single value.
         *
         * Initializes all components x, y, and z with the same given value.
         *
         * @param value The value to set for all components.
         */
        constexpr explicit Vector3(T value)
        : x(value), y(value), z(value)
        { }

        /**
         * @brief Constructor initializing all components with specific values.
         *
         * Initializes the x, y, and z components with the given values.
         *
         * @param x The value to set for the x component.
         * @param y The value to set for the y component.
         * @param z The value to set for the z component.
         */
        constexpr Vector3(T x, T y, T z = 0.0f)
        : x(x), y(y), z(z)
        { }

        /**
         * @brief Constructor initializing the vector from a 2D vector and an optional z value.
         *
         * Initializes the x and y components of the vector with the x and y values of the given 2D vector,
         * and sets the z component to the specified value (default is 0.0).
         *
         * @param vec2 The 2D vector to initialize the x and y components with.
         * @param z The value to set for the z component (default is 0.0).
         */
        constexpr Vector3(const Vector2<T>& vec2, float z = 0.0f)
        : x(vec2.x), y(vec2.y), z(z)
        { }

#       ifdef SUPPORT_MODEL

        /**
         * @brief Constructor initializing a Vector3 from an aiVector3D.
         *
         * This constructor initializes a Vector3 object with the x, y, and z components extracted from
         * the provided aiVector3D object.
         *
         * @param vec The aiVector3D object to initialize from.
         */
        constexpr Vector3(const aiVector3D& vec) : x(vec.x), y(vec.y), z(vec.z) { }

        /**
         * @brief Conversion operator to aiVector3D.
         *
         * This operator converts the current Vector3 object to an aiVector3D object.
         * The x, y, and z components of the current Vector3 object are used to construct the resulting aiVector3D object.
         *
         * @return An aiVector3D object equivalent to this Vector3.
         */
        operator aiVector3D() const { return aiVector3D(x, y, z); }

#       endif

#       ifdef SUPPORT_PHYSICS_2D

        /**
         * @brief Constructor initializing the vector from a b2Vec3 (Box2D vector).
         *
         * Initializes the x, y, and z components of the vector with the x, y, and z values of the given b2Vec3.
         *
         * @param vec The b2Vec3 containing the x, y, and z values to initialize the vector with.
         */
        constexpr Vector3(const b2Vec3& vec) : x(vec.x), y(vec.y), z(vec.z) { }

        /**
         * @brief Conversion operator to convert the vector to a b2Vec3 (Box2D vector).
         *
         * Converts the current vector to a b2Vec3 with the same x, y, and z values.
         *
         * @return A b2Vec3 containing the x, y, and z values of the current vector.
         */
        constexpr operator b2Vec3() const { return b2Vec3(x, y, z); }

#       endif

#       ifdef SUPPORT_PHYSICS_3D

        /**
         * @brief Constructor initializing the vector from a btVector3 (Bullet Physics vector).
         *
         * Initializes the x, y, and z components of the vector with the x, y, and z values of the given btVector3.
         *
         * @param vec The btVector3 containing the x, y, and z values to initialize the vector with.
         */
        constexpr Vector3(const btVector3& vec) : x(vec.m_floats[0]), y(vec.m_floats[1]), z(vec.m_floats[2]) { }

        /**
         * @brief Conversion operator to convert the vector to a btVector3 (Bullet Physics vector).
         *
         * Converts the current vector to a btVector3 with the same x, y, and z values.
         *
         * @return A btVector3 containing the x, y, and z values of the current vector.
         */
        /*constexpr*/ operator btVector3() const { return btVector3(x, y, z); }

#       endif

        /**
         * @brief Conversion operator to convert the vector to a Vector3 of a different type.
         *
         * Converts the current vector to a Vector3 of a different type by casting its components to the new type.
         *
         * @tparam U The type to convert the components of the vector to.
         * @return A Vector3 containing the components of the current vector casted to the type U.
         */
        template <typename U>
        constexpr operator Vector3<U>() const
        {
            return Vector3<U>(
                static_cast<U>(x),
                static_cast<U>(y),
                static_cast<U>(z));
        }

        /**
         * @brief Subscript operator to access individual components of the vector.
         *
         * @param axis The index of the component to access (0 for x, 1 for y, 2 for z).
         * @return A reference to the component at the specified index.
         */
        constexpr T& operator[](int axis)
        {
            return *(reinterpret_cast<T*>(this) + axis);
        }

        /**
         * @brief Subscript operator to access individual components of the vector (const version).
         *
         * @param axis The index of the component to access (0 for x, 1 for y, 2 for z).
         * @return A const reference to the component at the specified index.
         */
        constexpr const T& operator[](int axis) const
        {
            return *(reinterpret_cast<const T*>(this) + axis);
        }

        /**
         * @brief Unary negation operator.
         *
         * @return A new vector with each component negated.
         */
        constexpr Vector3 operator-() const
        {
            return { -x, -y, -z };
        }

        /**
         * @brief Subtraction operator with a scalar value.
         *
         * @param scalar The scalar value to subtract from each component of the vector.
         * @return A new vector resulting from the subtraction operation.
         */
        constexpr Vector3 operator-(T scalar) const
        {
            return Vector3(x - scalar, y - scalar, z - scalar);
        }

        /**
         * @brief Addition operator with a scalar value.
         *
         * @param scalar The scalar value to add to each component of the vector.
         * @return A new vector resulting from the addition operation.
         */
        constexpr Vector3 operator+(T scalar) const
        {
            return Vector3(x + scalar, y + scalar, z + scalar);
        }

        /**
         * @brief Multiplication operator with a scalar value.
         *
         * @param scalar The scalar value to multiply each component of the vector by.
         * @return A new vector resulting from the multiplication operation.
         */
        constexpr Vector3 operator*(T scalar) const
        {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        /**
         * @brief Division operator by a scalar value.
         *
         * @param scalar The scalar value to divide each component of the vector by.
         * @return A new vector resulting from the division operation.
         */
        constexpr Vector3 operator/(T scalar) const
        {
            if (scalar == 0) return { };

            if constexpr(std::is_floating_point<T>::value)
            {
                const T inv = static_cast<T>(1.0) / scalar;
                return Vector3(x * inv, y * inv, z * inv);
            }

            return Vector3(x / scalar, y / scalar, z / scalar);
        }

        /**
         * @brief Subtraction operator between two vectors.
         *
         * @param other The vector to subtract from the current vector.
         * @return A new vector resulting from the subtraction operation.
         */
        constexpr Vector3 operator-(const Vector3& other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        /**
         * @brief Addition operator between two vectors.
         *
         * @param other The vector to add to the current vector.
         * @return A new vector resulting from the addition operation.
         */
        constexpr Vector3 operator+(const Vector3& other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        /**
         * @brief Multiplication operator with another vector.
         *
         * @param other The vector to multiply element-wise.
         * @return A new vector resulting from the element-wise multiplication.
         */
        constexpr Vector3 operator*(const Vector3& other) const
        {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }

        /**
         * @brief Division operator by another vector.
         *
         * @param other The vector to divide element-wise.
         * @return A new vector resulting from the element-wise division.
         */
        constexpr Vector3 operator/(const Vector3& other) const
        {
            return Vector3(
                other.x ? x / other.x : 0,
                other.y ? y / other.y : 0,
                other.z ? z / other.z : 0);
        }

        /**
         * @brief Equality operator.
         *
         * @param other The vector to compare with.
         * @return True if the vectors are equal (all components are equal), false otherwise.
         */
        constexpr bool operator==(const Vector3& other) const
        {
            return (x == other.x) && (y == other.y) && (z == other.z);
        }

        /**
         * @brief Inequality operator.
         *
         * @param other The vector to compare with.
         * @return True if the vectors are not equal (at least one component is different), false otherwise.
         */
        constexpr bool operator!=(const Vector3& other) const
        {
            return (x != other.x) || (y != other.y) || (z != other.z);
        }

        /**
         * @brief Subtraction and assignment operator with a scalar value.
         *
         * @param scalar The scalar value to subtract from each component of the vector.
         * @return A reference to the modified vector.
         */
        Vector3& operator-=(T scalar)
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;
            return *this;
        }

        /**
         * @brief Addition and assignment operator with a scalar value.
         *
         * @param scalar The scalar value to add to each component of the vector.
         * @return A reference to the modified vector.
         */
        Vector3& operator+=(T scalar)
        {
            x += scalar;
            y += scalar;
            z += scalar;
            return *this;
        }

        /**
         * @brief Multiplication and assignment operator with a scalar value.
         *
         * @param scalar The scalar value to multiply each component of the vector by.
         * @return A reference to the modified vector.
         */
        Vector3& operator*=(T scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        /**
         * @brief Division and assignment operator with a scalar value.
         *
         * @param scalar The scalar value to divide each component of the vector by.
         * @return A reference to the modified vector.
         */
        Vector3& operator/=(T scalar)
        {
            if (scalar == 0) { *this = { }; return *this; }

            if constexpr(std::is_floating_point<T>::value)
            {
                const T inv = static_cast<T>(1.0) / scalar;
                x *= inv, y *= inv, z *= inv;
                return *this;
            }

            x /= scalar, y /= scalar, z /= scalar;

            return *this;
        }

        /**
         * @brief Subtraction and assignment operator with another vector.
         *
         * @param other The vector to subtract.
         * @return Reference to the modified vector after subtraction.
         */
        Vector3& operator-=(const Vector3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        /**
         * @brief Addition and assignment operator with another vector.
         *
         * @param other The vector to add.
         * @return Reference to the modified vector after addition.
         */
        Vector3& operator+=(const Vector3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        /**
         * @brief Multiplication and assignment operator with another vector.
         *
         * @param other The vector to multiply.
         * @return Reference to the modified vector after multiplication.
         */
        Vector3& operator*=(const Vector3& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        /**
         * @brief Division and assignment operator with another vector.
         *
         * @param other The vector to divide.
         * @return Reference to the modified vector after division.
         */
        Vector3& operator/=(const Vector3& other)
        {
            if (other.x != 0.0f) x /= other.x;
            if (other.y != 0.0f) y /= other.y;
            if (other.z != 0.0f) z /= other.z;
            return *this;
        }

        /**
         * @brief Method to check if the vector is equal to (0,0,0).
         *
         * @return True if the vector is equal to (0,0,0), false otherwise.
         */
        bool IsZero() const
        {
            return !(x + y + z);
        }

        /**
         * @brief Method to calculate the magnitude (length) of the vector.
         *
         * @return The magnitude (length) of the vector.
         */
        double Magnitude() const
        {
            return std::sqrt(x * x + y * y + z * z);
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
         * @brief Function to calculate the length (magnitude) squared of the vector.
         *
         * @return The length (magnitude) squared of the vector.
         */
        T LengthSqr() const
        {
            return x * x + y * y + z * z;
        }

        /**
         * @brief Method to calculate the dot product of the vector with another vector.
         *
         * @param other The other vector for dot product calculation.
         * @return The dot product of the two vectors.
         */
        T Dot(const Vector3& other) const
        {
            return x * other.x + y * other.y + z * other.z;
        }

        /**
         * @brief Method to normalize the vector.
         *
         * If the magnitude of the vector is not zero, this method normalizes the vector.
         */
        void Normalize()
        {
            const double mag = Magnitude();
            if (mag != 0.0) (*this) *= 1.0 / mag;
        }

        /**
         * @brief Method to obtain a normalized vector.
         *
         * @return A normalized vector.
         */
        Vector3 Normalized() const
        {
            Vector3 result(*this);
            result.Normalize();
            return result;
        }

        /**
         * @brief Makes vectors normalized and orthogonal to each other using Gram-Schmidt process.
         *
         * @param tangent A vector orthogonal to this vector after normalization.
         */
        void OrthoNormalize(Vector3& tangent)
        {
            this->Normalize();
            tangent = this->Cross(tangent).Normalized().Cross(*this);
        }

        /**
         * @brief Method to calculate the distance between two vectors.
         *
         * @param other The other vector to calculate the distance to.
         * @return The distance between this vector and the other vector.
         */
        double Distance(const Vector3& other) const
        {
            return (*this - other).Magnitude();
        }

        /**
         * @brief Function to calculate the distance squared between two vectors.
         *
         * @param other The other vector to calculate the distance to.
         * @return The distance squared between this vector and the other vector.
         */
        T DistanceSqr(const Vector3& other) const
        {
            const Vector3 diff = *this - other;
            return diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
        }

        /**
         * @brief Function to calculate the angle between two vectors.
         *
         * @param other The other vector.
         * @return The angle between the two vectors in radians.
         */
        double Angle(const Vector3& other)
        {
            return std::atan2(this->Cross(other).Length(), this->Dot(other));
        }

        /**
         * @brief Function to rotate the vector around an axis by a certain angle (Euler-Rodrigues).
         *
         * @param axis The axis of rotation.
         * @param angle The angle of rotation in radians.
         */
        void Rotate(Vector3 axis, double angle)
        {
            axis.Normalize();
            angle *= 0.5f;

            Vector3 w = axis * std::sin(angle);
            Vector3 wv = w.Cross(*this);
            Vector3 wwv = w.Cross(wv);

            wv *= 2 * std::cos(angle);
            wwv *= 2;

            (*this) += wv + wwv;
        }

        /**
         * @brief Function to rotate the vector by a quaternion (Vec4).
         *
         * @param q The quaternion representing the rotation.
         */
        void Rotate(const Vector4<T>& q);

        /**
         * @brief Function to calculate the rotation of the vector by an axis and an angle.
         *
         * @param axis The axis of rotation.
         * @param angle The angle of rotation in radians.
         * @return The rotated vector.
         */
        Vector3 Rotated(const Vector3& axis, double angle) const
        {
            math::Vector3 result(*this);
            result.Rotate(axis, angle);
            return result;
        }

        /**
         * @brief Function to calculate the rotation of the vector by a quaternion (Vec4).
         *
         * @param q The quaternion representing the rotation.
         * @return The rotated vector.
         */
        Vector3 Rotated(const Vector4<T>& q);

        /**
         * @brief Function to perform a reflection of the vector with respect to another vector.
         *
         * @param normal The normal vector (assumed to be a unit vector).
         * @return The reflected vector.
         */
        Vector3 Reflect(const Vector3& normal) const
        {
            // Assuming 'normal' is a unit vector
            double dot = this->Dot(normal);

            return Vector3(
                x - 2.0f * this->Dot(normal) * normal.x,
                y - 2.0f * this->Dot(normal) * normal.y,
                z - 2.0f * this->Dot(normal) * normal.z);
        }

        /**
         * @brief Function to perform a cross product of the vector with another vector.
         *
         * @param other The other vector.
         * @return The cross product vector.
         */
        Vector3 Cross(const Vector3& other) const
        {
            return Vector3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            );
        }

        /**
         * @brief Function to obtain the direction vector from this vector to another vector.
         *
         * @param other The target vector.
         * @return The direction vector from this vector to the target vector.
         */
        Vector3 Direction(const Vector3& other) const
        {
            return (other - *this).Normalized();
        }

        /**
         * @brief Function to transform the vector by a 3x3 matrix.
         *
         * @param matrix The 3x3 transformation matrix.
         */
        void Transform(const Mat3& matrix)
        {
            *this = {
                x * matrix.m[0] + y * matrix.m[3] + z * matrix.m[6],
                x * matrix.m[1] + y * matrix.m[4] + z * matrix.m[7],
                x * matrix.m[2] + y * matrix.m[5] + z * matrix.m[8]
            };
        }

        /**
         * @brief Function to obtain the vector transformed by a 3x3 matrix.
         *
         * @param matrix The 3x3 transformation matrix.
         * @return The transformed vector.
         */
        Vector3 Transformed(const Mat3& matrix) const
        {
            return {
                x * matrix.m[0] + y * matrix.m[3] + z * matrix.m[6],
                x * matrix.m[1] + y * matrix.m[4] + z * matrix.m[7],
                x * matrix.m[2] + y * matrix.m[5] + z * matrix.m[8]
            };
        }

        /**
         * @brief Function to transform the vector by a 4x4 matrix.
         *
         * @param matrix The 4x4 transformation matrix.
         */
        void Transform(const Mat4& matrix)
        {
            *this = {
                x * matrix.m[0] + y * matrix.m[4] + z * matrix.m[8] + matrix.m[12],
                x * matrix.m[1] + y * matrix.m[5] + z * matrix.m[9] + matrix.m[13],
                x * matrix.m[2] + y * matrix.m[6] + z * matrix.m[10] + matrix.m[14],
            };
        }

        /**
         * @brief Function to obtain the vector transformed by a 4x4 matrix.
         *
         * @param matrix The 4x4 transformation matrix.
         * @return The transformed vector.
         */
        Vector3 Transformed(const Mat4& matrix) const
        {
            return {
                x * matrix.m[0] + y * matrix.m[4] + z * matrix.m[8] + matrix.m[12],
                x * matrix.m[1] + y * matrix.m[5] + z * matrix.m[9] + matrix.m[13],
                x * matrix.m[2] + y * matrix.m[6] + z * matrix.m[10] + matrix.m[14]
            };
        }

        /**
         * @brief Calculate a perpendicular vector to the given vector.
         *
         * @param other The input vector.
         * @return A perpendicular vector to the input vector.
         */
        static Vector3 Perpendicular(const Vector3& other)
        {
            Vector3 cardinalAxis = {1.0, 0.0, 0.0};
            const Vector3 oabs = other.Abs();
            T min = oabs.x;

            if (oabs.y < min)
            {
                min = oabs.y;
                cardinalAxis = {0.0, 1.0, 0.0};
            }

            if (oabs.z < min)
            {
                cardinalAxis = {0.0, 0.0, 1.0};
            }

            return other.Cross(cardinalAxis);
        }

        /**
         * @brief Get the component-wise minimum of this vector and another vector.
         *
         * @param other The other vector.
         * @return A vector containing the component-wise minimum values.
         */
        Vector3 Min(const Vector3& other) const
        {
            return {
                std::min(x, other.x),
                std::min(y, other.y),
                std::min(z, other.z)
            };
        }

        /**
         * @brief Get the component-wise maximum of this vector and another vector.
         *
         * @param other The other vector.
         * @return A vector containing the component-wise maximum values.
         */
        Vector3 Max(const Vector3& other) const
        {
            return {
                std::max(x, other.x),
                std::max(y, other.y),
                std::max(z, other.z)
            };
        }

        /**
         * @brief Clamp each component of this vector between the corresponding components of min and max vectors.
         *
         * @param min The vector containing the minimum values.
         * @param max The vector containing the maximum values.
         * @return A vector with each component clamped between the corresponding components of min and max.
         */
        Vector3 Clamp(const Vector3& min, const Vector3& max) const
        {
            return Vector3(
                std::clamp(x, min.x, max.x),
                std::clamp(y, min.y, max.y),
                std::clamp(z, min.z, max.z)
            );
        }

        /**
         * @brief Clamp each component of this vector between a minimum and maximum value.
         *
         * @param min The minimum value.
         * @param max The maximum value.
         * @return A vector with each component clamped between the minimum and maximum values.
         */
        Vector3 Clamp(T min, T max) const
        {
            return Vector3(
                std::clamp(x, min, max),
                std::clamp(y, min, max),
                std::clamp(z, min, max)
            );
        }

        /**
         * @brief Get the absolute value of each component of this vector.
         *
         * @return A vector containing the absolute values of each component.
         */
        Vector3 Abs() const
        {
            return Vector3(std::abs(x), std::abs(y), std::abs(z));
        }
    };

    // Specific class for 3D floating point vectors
    using Vec3 = Vector3<float>;

    // Specific class for 3D vectors in integers
    using IVec3 = Vector3<int>;

}}

#endif
