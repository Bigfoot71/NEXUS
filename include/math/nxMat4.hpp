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

#ifndef NEXUS_MATH_MAT4_HPP
#define NEXUS_MATH_MAT4_HPP

#include "../platform/nxPlatform.hpp"
#include "../core/nxLog.hpp"

#ifdef SUPPORT_MODEL
#   include "assimp/matrix4x4.h"
#endif

#include <algorithm>
#include <cmath>

namespace nexus { namespace math {

    template <typename T> struct Vector3;
    template <typename T> struct Vector4;

    struct NEXUS_API Mat4
    {
        float m[16]{};  ///< 4x4 matrix elements

        /**
         * @brief Default constructor
         */
        constexpr Mat4() = default;

        /**
         * @brief Constructor from array
         * @param mat Pointer to an array of 16 floats representing the matrix elements
         */
        Mat4(const float *mat)
        {
            std::copy(mat, mat + 16, m);
        }

        /**
         * @brief Constructor from individual elements
         * @param m0 Element at index (0, 0)
         * @param m4 Element at index (1, 1)
         * @param m8 Element at index (2, 2)
         * @param m12 Element at index (3, 3)
         * @param m1 Element at index (0, 1)
         * @param m5 Element at index (1, 1)
         * @param m9 Element at index (2, 1)
         * @param m13 Element at index (3, 1)
         * @param m2 Element at index (0, 2)
         * @param m6 Element at index (1, 2)
         * @param m10 Element at index (2, 2)
         * @param m14 Element at index (3, 2)
         * @param m3 Element at index (0, 3)
         * @param m7 Element at index (1, 3)
         * @param m11 Element at index (2, 3)
         * @param m15 Element at index (3, 3)
         */
        constexpr Mat4(float m0, float m4, float m8,  float m12,
                    float m1, float m5, float m9,  float m13,
                    float m2, float m6, float m10, float m14,
                    float m3, float m7, float m11, float m15);

#       ifdef SUPPORT_MODEL

        /**
         * @brief Constructor from an aiMatrix4x4
         * @param aiMat An aiMatrix4x4 object to initialize from
         */
        constexpr Mat4(const aiMatrix4x4& aiMat)
        {
            std::copy(&aiMat.a1, &aiMat.a1 + 16, this->m);
            *this = this->Transpose();
        }

        /**
         * @brief Conversion operator to aiMatrix4x4
         * @return An aiMatrix4x4 equivalent to this Mat4
         */
        operator aiMatrix4x4()
        {
            return {
                m[0],  m[1],  m[2],  m[3],
                m[4],  m[5],  m[6],  m[7],
                m[8],  m[9],  m[10], m[11],
                m[12], m[13], m[14], m[15]
            };
        }

#       endif

        /**
         * @brief Returns the identity matrix
         * @return Identity matrix
         */
        static constexpr Mat4 Identity();

        /**
         * @brief Creates a translation matrix
         * @param x Translation in the x-axis
         * @param y Translation in the y-axis
         * @param z Translation in the z-axis
         * @return Translation matrix
         */
        static constexpr Mat4 Translate(float x, float y, float z);

        /**
         * @brief Creates a translation matrix from a 3D vector
         * @tparam T Type of vector components
         * @param v Translation vector
         * @return Translation matrix
         */
        template <typename T>
        static constexpr Mat4 Translate(const Vector3<T>& v);

        /**
         * @brief Creates a rotation matrix
         * @param x X component of the rotation axis
         * @param y Y component of the rotation axis
         * @param z Z component of the rotation axis
         * @param angle Rotation angle in radians
         * @return Rotation matrix
         */
        static constexpr Mat4 Rotate(float x, float y, float z, float angle);

        /**
         * @brief Creates a rotation matrix from an axis and angle
         * @tparam T Type of vector components
         * @param axis Rotation axis
         * @param angle Rotation angle in radians
         * @return Rotation matrix
         */
        template <typename T>
        static constexpr Mat4 Rotate(const Vector3<T>& axis, float angle);

        /**
         * @brief Creates a rotation matrix around the X-axis
         * @param angle Rotation angle in radians
         * @return Rotation matrix
         */
        static constexpr Mat4 RotateX(float angle);

        /**
         * @brief Creates a rotation matrix around the Y-axis
         * @param angle Rotation angle in radians
         * @return Rotation matrix
         */
        static constexpr Mat4 RotateY(float angle);

        /**
         * @brief Creates a rotation matrix around the Z-axis
         * @param angle Rotation angle in radians
         * @return Rotation matrix
         */
        static constexpr Mat4 RotateZ(float angle);

        /**
         * @brief Creates a rotation matrix around the X, Y, and Z axes in the order specified
         * @param angleX Rotation angle around the X-axis in radians
         * @param angleY Rotation angle around the Y-axis in radians
         * @param angleZ Rotation angle around the Z-axis in radians
         * @return Rotation matrix
         */
        static constexpr Mat4 RotateXYZ(float angleX, float angleY, float angleZ);

        /**
         * @brief Creates a rotation matrix around the Z, Y, and X axes in the order specified
         * @param angleZ Rotation angle around the Z-axis in radians
         * @param angleY Rotation angle around the Y-axis in radians
         * @param angleX Rotation angle around the X-axis in radians
         * @return Rotation matrix
         */
        static constexpr Mat4 RotateZYX(float angleZ, float angleY, float angleX);

        /**
         * @brief Creates a scaling matrix
         * @param sx Scaling factor in the x-axis
         * @param sy Scaling factor in the y-axis
         * @param sz Scaling factor in the z-axis
         * @return Scaling matrix
         */
        static constexpr Mat4 Scale(float sx, float sy, float sz);

        /**
         * @brief Creates a scaling matrix from a 3D vector
         * @tparam T Type of vector components
         * @param v Scaling factors in each axis
         * @return Scaling matrix
         */
        template <typename T>
        static constexpr Mat4 Scale(const Vector3<T>& v);

        /**
         * @brief Creates a transformation matrix from translation, scale, and rotation
         * @tparam T Type of vector components
         * @param translate Translation vector
         * @param scale Scaling vector
         * @param rotationAxis Axis of rotation
         * @param rotationAngle Angle of rotation in radians
         * @return Transformation matrix
         */
        template <typename T>
        static constexpr Mat4 Transform(const Vector3<T>& translate, const Vector3<T>& scale, const Vector3<T> rotationAxis, T rotationAngle);

        /**
         * @brief Creates a transformation matrix from translation, scale, and quaternion rotation
         * @tparam T Type of vector components
         * @param translate Translation vector
         * @param scale Scaling vector
         * @param quaternion Quaternion representing rotation
         * @return Transformation matrix
         */
        template <typename T>
        static constexpr Mat4 Transform(const Vector3<T>& translate, const Vector3<T>& scale, const Vector4<T> quaternion);

        /**
         * @brief Creates a perspective frustum matrix
         * @param left Coordinate for the left vertical clipping plane
         * @param right Coordinate for the right vertical clipping plane
         * @param bottom Coordinate for the bottom horizontal clipping plane
         * @param top Coordinate for the top horizontal clipping plane
         * @param near Distance to the near depth clipping plane (positive)
         * @param far Distance to the far depth clipping plane (positive)
         * @return Perspective frustum matrix
         */
        static constexpr Mat4 Frustum(float left, float right, float bottom, float top, float near, float far);

        /**
         * @brief Creates a perspective projection matrix
         * @param fovy Field of view angle in the y direction in radians
         * @param aspect Aspect ratio, defined as width divided by height
         * @param near Distance to the near depth clipping plane (positive)
         * @param far Distance to the far depth clipping plane (positive)
         * @return Perspective projection matrix
         */
        static constexpr Mat4 Perspective(float fovy, float aspect, float near, float far);

        /**
         * @brief Creates an orthographic projection matrix
         * @param left Coordinate for the left vertical clipping plane
         * @param right Coordinate for the right vertical clipping plane
         * @param bottom Coordinate for the bottom horizontal clipping plane
         * @param top Coordinate for the top horizontal clipping plane
         * @param near Distance to the near depth clipping plane (positive)
         * @param far Distance to the far depth clipping plane (positive)
         * @return Orthographic projection matrix
         */
        static constexpr Mat4 Ortho(float left, float right, float bottom, float top, float near, float far);

        /**
         * @brief Creates a view matrix using the look-at algorithm
         * @tparam T Type of vector components
         * @param eye Position of the camera
         * @param target Position where the camera is looking
         * @param up Up direction in world space (usually [0, 1, 0])
         * @return View matrix
         */
        template <typename T>
        static constexpr Mat4 LookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up);

        /**
         * @brief Creates a matrix from a quaternion rotation
         * @tparam T Type of vector components
         * @param q Quaternion rotation
         * @return Matrix representing the quaternion rotation
         */
        template <typename T>
        static constexpr Mat4 FromQuaternion(const Vector4<T>& q); // NOTE: Defined in bgVec4.hpp

        /**
         * @brief Gets the translation component of the matrix
         * @tparam T Type of vector components
         * @return Translation vector
         */
        template <typename T = float>
        Vector3<T> GetTranslation() const;

        /**
         * @brief Gets the rotation component of the matrix as a quaternion
         * @tparam T Type of vector components
         * @return Rotation quaternion
         */
        template <typename T = float>
        Vector4<T> GetRotation() const;

        /**
         * @brief Calculates the determinant of the matrix
         * @return Determinant value
         */
        constexpr float Determinant() const;

        /**
         * @brief Calculates the trace of the matrix (sum of values on the diagonal)
         * @return Trace value
         */
        constexpr float Trace() const;

        /**
         * @brief Transposes the matrix
         * @return Transposed matrix
         */
        constexpr Mat4 Transpose() const;

        /**
         * @brief Inverts the matrix (if it is invertible)
         * @return Inverted matrix
         */
        constexpr Mat4 Invert() const;

        /**
         * @brief Returns the array/pointer of the matrix
         * @return Pointer to the matrix array
         */
        constexpr operator const float*() const { return m; }

        /**
         * @brief Addition operator for the 4x4 matrix
         * @param other Matrix to add
         * @return Result of addition
         */
        constexpr Mat4 operator+(const Mat4& other) const;

        /**
         * @brief Subtraction operator for the 4x4 matrix
         * @param other Matrix to subtract
         * @return Result of subtraction
         */
        constexpr Mat4 operator-(const Mat4& other) const;

        /**
         * @brief Multiplication operator for matrices (4x4)
         * @param other Matrix to multiply with
         * @return Result of multiplication
         */
        constexpr Mat4 operator*(const Mat4& other) const;

        /**
         * @brief Multiplication operator for a 4x4 matrix and a 3D vector (transform)
         * @tparam T Type of vector components
         * @param v 3D vector to multiply with
         * @return Result of multiplication
         */
        template <typename T>
        constexpr Vector4<T> operator*(const Vector3<T>& v) const; // NOTE: Defined in bgMat4.hpp

        /**
         * @brief Multiplication operator for a 4x4 matrix and a 4D vector (transform)
         * @tparam T Type of vector components
         * @param v 4D vector to multiply with
         * @return Result of multiplication
         */
        template <typename T>
        constexpr Vector4<T> operator*(const Vector4<T>& v) const; // NOTE: Defined in bgMat4.hpp

        /**
         * @brief Scalar multiplication operator for the 4x4 matrix
         * @param scalar Scalar value to multiply with
         * @return Result of multiplication
         */
        constexpr Mat4 operator*(float scalar) const;

        /**
         * @brief Addition and assignment operator for the 4x4 matrix
         * @param other Matrix to add
         */
        void operator+=(const Mat4& other) { *this = *this + other; }

        /**
         * @brief Subtraction and assignment operator for the 4x4 matrix
         * @param other Matrix to subtract
         */
        void operator-=(const Mat4& other) { *this = *this - other; }

        /**
         * @brief Multiplication and assignment operator for matrices (4x4)
         * @param other Matrix to multiply with
         */
        void operator*=(const Mat4& other) { *this = *this * other; }

        /**
         * @brief Scalar multiplication and assignment operator for the 4x4 matrix
         * @param scalar Scalar value to multiply with
         */
        void operator*=(float scalar) { *this = *this * scalar; }

        /**
         * @brief Equality operator for the 4x4 matrix
         * @param other Matrix to compare with
         * @return True if matrices are equal, false otherwise
         */
        constexpr bool operator==(const Mat4& other) const;

        /**
         * @brief Inequality operator for the 4x4 matrix
         * @param other Matrix to compare with
         * @return True if matrices are not equal, false otherwise
         */
        constexpr bool operator!=(const Mat4& other) const
        {
            return !(*this == other);
        }

        /* DEBUG */

        void Print() const
        {
            NEXUS_LOG(Debug) << "*~~~~~~ MATRIX 4x4 ~~~~~~*\n"
                << "m0 = " << m[0] << ", m4 = " << m[4] << ", m8  = " << m[8] << ",  m12 = " << m[12] << "\n"
                << "m1 = " << m[1] << ", m5 = " << m[5] << ", m9  = " << m[9] << ",  m13 = " << m[13] << "\n"
                << "m2 = " << m[2] << ", m6 = " << m[6] << ", m10 = " << m[10] << ", m14 = " << m[14] << "\n"
                << "m3 = " << m[3] << ", m7 = " << m[7] << ", m11 = " << m[11] << ", m15 = " << m[15] << "\n"
                << "*~~~~~~~~~~~~~~~~~~~~~~~~*\n";
        }
    };

    constexpr Mat4::Mat4(float m0, float m4, float m8,  float m12,
                         float m1, float m5, float m9,  float m13,
                         float m2, float m6, float m10, float m14,
                         float m3, float m7, float m11, float m15)
    {
        m[0] = m0; m[4] = m4; m[8]  = m8;  m[12] = m12;
        m[1] = m1; m[5] = m5; m[9]  = m9;  m[13] = m13;
        m[2] = m2; m[6] = m6; m[10] = m10; m[14] = m14;
        m[3] = m3; m[7] = m7; m[11] = m11; m[15] = m15;
    }

    constexpr Mat4 Mat4::Identity()
    {
        return {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    constexpr Mat4 Mat4::Translate(float x, float y, float z)
    {
        return {
            1.0f, 0.0f, 0.0f, x,
            0.0f, 1.0f, 0.0f, y,
            0.0f, 0.0f, 1.0f, z,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    template <typename T>
    constexpr Mat4 Mat4::Translate(const Vector3<T>& v)
    {
        return Translate(v.x, v.y, v.z);
    }

    constexpr Mat4 Mat4::Rotate(float x, float y, float z, float angle)
    {
        float lenSq = x * x + y * y + z * z;

        if (lenSq != 1.0f && lenSq != 0.0f)
        {
            float len = 1.0f / std::sqrt(lenSq);
            x *= len, y *= len, z *= len;
        }

        float s = std::sin(angle);
        float c = std::cos(angle);
        float t = 1.0f - c;

        return {
            x * x * t + c, x * y * t - z * s, x * z * t + y * s, 0.0f,
            y * x * t + z * s, y * y * t + c, y * z * t - x * s, 0.0f,
            z * x * t - y * s, z * y * t + x * s, z * z * t + c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    template <typename T>
    constexpr Mat4 Mat4::Rotate(const Vector3<T>& axis, float angle)
    {
        return Rotate(axis.x, axis.y, axis.z, angle);
    }

    constexpr Mat4 Mat4::RotateX(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, c,    -s,   0.0f,
            0.0f, s,     c,   0.0f,
            0.0f, 0.0f,  0.0f, 1.0f
        };
    }

    constexpr Mat4 Mat4::RotateY(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return {
            c,    0.0f, s,    0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -s,   0.0f, c,   0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    constexpr Mat4 Mat4::RotateZ(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);

        return {
            c,    -s,   0.0f, 0.0f,
            s,     c,   0.0f, 0.0f,
            0.0f,  0.0f, 1.0f, 0.0f,
            0.0f,  0.0f, 0.0f, 1.0f
        };
    }

    constexpr Mat4 Mat4::RotateXYZ(float angleX, float angleY, float angleZ)
    {
        float cx = std::cos(angleX);
        float sx = std::sin(angleX);
        float cy = std::cos(angleY);
        float sy = std::sin(angleY);
        float cz = std::cos(angleZ);
        float sz = std::sin(angleZ);

        return {
            cy * cz, -cy * sz, sy, 0.0f,
            sx * sy * cz + cx * sz, -sx * sy * sz + cx * cz, -sx * cy, 0.0f,
            -cx * sy * cz + sx * sz, cx * sy * sz + sx * cz, cx * cy, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    constexpr Mat4 Mat4::RotateZYX(float angleZ, float angleY, float angleX)
    {
        float cx = std::cos(angleX);
        float sx = std::sin(angleX);
        float cy = std::cos(angleY);
        float sy = std::sin(angleY);
        float cz = std::cos(angleZ);
        float sz = std::sin(angleZ);

        return {
            cy * cz, -sz, cz * sy, 0.0f,
            cx * sz + sx * sy * cz, cx * cz - sx * sy * sz, -sx * cy, 0.0f,
            sx * sz - cx * sy * cz, cx * sy * sz + sx * cz, cx * cy, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    constexpr Mat4 Mat4::Scale(float sx, float sy, float sz)
    {
        return {
            sx, 0.0f, 0.0f, 0.0f,
            0.0f, sy, 0.0f, 0.0f,
            0.0f, 0.0f, sz, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    template <typename T>
    constexpr Mat4 Mat4::Scale(const Vector3<T>& v)
    {
        return Mat4::Scale(v.x, v.y, v.z);
    }

    template <typename T>
    constexpr Mat4 Mat4::Transform(const Vector3<T>& translate, const Vector3<T>& scale, const Vector3<T> rotationAxis, T rotationAngle)
    {
        return math::Mat4::Scale(scale) * math::Mat4::Rotate(rotationAxis, rotationAngle) * math::Mat4::Translate(translate);
    }

    template <typename T>
    constexpr Mat4 Mat4::Transform(const Vector3<T>& translate, const Vector3<T>& scale, const Vector4<T> quaternion)
    {
        return math::Mat4::Scale(scale) * math::Mat4::FromQuaternion(quaternion) * math::Mat4::Translate(translate);
    }

    constexpr Mat4 Mat4::Frustum(float left, float right, float bottom, float top, float near, float far)
    {
        float rl = right - left;
        float tb = top - bottom;
        float fn = far - near;

        return {
            2.0f * near / rl, 0.0f, (right + left) / rl, 0.0f,
            0.0f, 2.0f * near / tb, (top + bottom) / tb, 0.0f,
            0.0f, 0.0f, -(far + near) / fn, -2.0f * far * near / fn,
            0.0f, 0.0f, -1.0f, 0.0f
        };
    }

    constexpr Mat4 Mat4::Perspective(float fovy, float aspect, float near, float far)
    {
        float tanHalfFovy = std::tan(fovy / 2.0f);

        return {
            1.0f / (aspect * tanHalfFovy), 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f / tanHalfFovy, 0.0f, 0.0f,
            0.0f, 0.0f, -(far + near) / (far - near), -2.0f * far * near / (far - near),
            0.0f, 0.0f, -1.0f, 0.0f
        };
    }

    constexpr Mat4 Mat4::Ortho(float left, float right, float bottom, float top, float near, float far)
    {
        float rl = right - left;
        float tb = top - bottom;
        float fn = far - near;

        return {
            2.0f / rl, 0.0f, 0.0f, -(right + left) / rl,
            0.0f, 2.0f / tb, 0.0f, -(top + bottom) / tb,
            0.0f, 0.0f, -2.0f / fn, -(far + near) / fn,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    template <typename T>
    constexpr Mat4 Mat4::LookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up)
    {
        Vector3 zaxis = (eye - target).Normalized();
        Vector3 xaxis = up.Cross(zaxis).Normalized();
        Vector3 yaxis = zaxis.Cross(xaxis);

        Mat4 viewMatrix;

        viewMatrix.m[0]  = xaxis.x;
        viewMatrix.m[1]  = yaxis.x;
        viewMatrix.m[2]  = zaxis.x;
        viewMatrix.m[3]  = 0.0f;

        viewMatrix.m[4]  = xaxis.y;
        viewMatrix.m[5]  = yaxis.y;
        viewMatrix.m[6]  = zaxis.y;
        viewMatrix.m[7]  = 0.0f;

        viewMatrix.m[8]  = xaxis.z;
        viewMatrix.m[9]  = yaxis.z;
        viewMatrix.m[10] = zaxis.z;
        viewMatrix.m[11] = 0.0f;

        viewMatrix.m[12] = -xaxis.Dot(eye);
        viewMatrix.m[13] = -yaxis.Dot(eye);
        viewMatrix.m[14] = -zaxis.Dot(eye);
        viewMatrix.m[15] = 1.0f;

        return viewMatrix;
    }

    template <typename T>
    Vector3<T> Mat4::GetTranslation() const
    {
        return { m[12], m[13], m[14] };
    }

    template <typename T>
    Vector4<T> Mat4::GetRotation() const
    {
        Vector4<T> result{};

        T fourWSquaredMinus1 = m[0] + m[5] + m[10];
        T fourXSquaredMinus1 = m[0] - m[5] - m[10];
        T fourYSquaredMinus1 = m[5] - m[0] - m[10];
        T fourZSquaredMinus1 = m[10] - m[0] - m[5];

        int biggestIndex = 0;
        T fourBiggestSquaredMinus1 = fourWSquaredMinus1;

        if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourXSquaredMinus1;
            biggestIndex = 1;
        }

        if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourYSquaredMinus1;
            biggestIndex = 2;
        }

        if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourZSquaredMinus1;
            biggestIndex = 3;
        }

        T biggestVal = std::sqrt(fourBiggestSquaredMinus1 + static_cast<T>(1.0)) * static_cast<T>(0.5);
        T mult = static_cast<T>(0.25) / biggestVal;

        switch (biggestIndex)
        {
            case 0:
                result.w = biggestVal;
                result.x = (m[6] - m[9]) * mult;
                result.y = (m[8] - m[2]) * mult;
                result.z = (m[1] - m[4]) * mult;
                break;
            case 1:
                result.x = biggestVal;
                result.w = (m[6] - m[9]) * mult;
                result.y = (m[1] + m[4]) * mult;
                result.z = (m[8] + m[2]) * mult;
                break;
            case 2:
                result.y = biggestVal;
                result.w = (m[8] - m[2]) * mult;
                result.x = (m[1] + m[4]) * mult;
                result.z = (m[6] + m[9]) * mult;
                break;
            case 3:
                result.z = biggestVal;
                result.w = (m[1] - m[4]) * mult;
                result.x = (m[8] + m[2]) * mult;
                result.y = (m[6] + m[9]) * mult;
                break;
        }

        return result;
    }

    constexpr float Mat4::Determinant() const
    {
        return m[0] * (m[5] * m[10] - m[6] * m[9]) - m[1] * (m[4] * m[10] - m[6] * m[8]) + m[2] * (m[4] * m[9] - m[5] * m[8]);
    }

    constexpr float Mat4::Trace() const
    {
        return m[0] + m[5] + m[10] + m[15];
    }

    constexpr Mat4 Mat4::Transpose() const
    {
        return {
            m[0], m[1], m[2], m[3],
            m[4], m[5], m[6], m[7],
            m[8], m[9], m[10], m[11],
            m[12], m[13], m[14], m[15]
        };
    }

    constexpr Mat4 Mat4::Invert() const
    {
        float a00 = m[0], a01 = m[1], a02 = m[2], a03 = m[3];
        float a10 = m[4], a11 = m[5], a12 = m[6], a13 = m[7];
        float a20 = m[8], a21 = m[9], a22 = m[10], a23 = m[11];
        float a30 = m[12], a31 = m[13], a32 = m[14], a33 = m[15];

        float b00 = a00*a11 - a01*a10;
        float b01 = a00*a12 - a02*a10;
        float b02 = a00*a13 - a03*a10;
        float b03 = a01*a12 - a02*a11;
        float b04 = a01*a13 - a03*a11;
        float b05 = a02*a13 - a03*a12;
        float b06 = a20*a31 - a21*a30;
        float b07 = a20*a32 - a22*a30;
        float b08 = a20*a33 - a23*a30;
        float b09 = a21*a32 - a22*a31;
        float b10 = a21*a33 - a23*a31;
        float b11 = a22*a33 - a23*a32;

        float invDet = 1.0f/(b00*b11 - b01*b10 + b02*b09 + b03*b08 - b04*b07 + b05*b06);

        return {
            (a11 * b11 - a12 * b10 + a13 * b09) * invDet,
            (-a10 * b11 + a12 * b08 - a13 * b07) * invDet,
            (a10 * b10 - a11 * b08 + a13 * b06) * invDet,
            (-a10 * b09 + a11 * b07 - a12 * b06) * invDet,
            (-a01 * b11 + a02 * b10 - a03 * b09) * invDet,
            (a00 * b11 - a02 * b08 + a03 * b07) * invDet,
            (-a00 * b10 + a01 * b08 - a03 * b06) * invDet,
            (a00 * b09 - a01 * b07 + a02 * b06) * invDet,
            (a31 * b05 - a32 * b04 + a33 * b03) * invDet,
            (-a30 * b05 + a32 * b02 - a33 * b01) * invDet,
            (a30 * b04 - a31 * b02 + a33 * b00) * invDet,
            (-a30 * b03 + a31 * b01 - a32 * b00) * invDet,
            (-a21 * b05 + a22 * b04 - a23 * b03) * invDet,
            (a20 * b05 - a22 * b02 + a23 * b01) * invDet,
            (-a20 * b04 + a21 * b02 - a23 * b00) * invDet,
            (a20 * b03 - a21 * b01 + a22 * b00) * invDet
        };
    }

    constexpr Mat4 Mat4::operator+(const Mat4& other) const
    {
        Mat4 result;

        for (int i = 0; i < 16; ++i)
        {
            result.m[i] = m[i] + other.m[i];
        }

        return result;
    }

    constexpr Mat4 Mat4::operator-(const Mat4& other) const
    {
        Mat4 result;

        for (int i = 0; i < 16; ++i)
        {
            result.m[i] = m[i] - other.m[i];
        }

        return result;
    }

    constexpr Mat4 Mat4::operator*(const Mat4& other) const
    {
        Mat4 result;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                float sum = 0.0f;
                for (int k = 0; k < 4; k++)
                {
                    sum += m[i * 4 + k] * other.m[k * 4 + j];
                }
                result.m[i * 4 + j] = sum;
            }
        }

        return result;
    }

    constexpr Mat4 Mat4::operator*(float scalar) const
    {
        Mat4 result;

        for (int i = 0; i < 16; i++)
        {
            result.m[i] = m[i] * scalar;
        }

        return result;
    }

    constexpr bool Mat4::operator==(const Mat4& other) const
    {
        for (int i = 0; i < 16; i++)
        {
            if (m[i] != other.m[i])
            {
                return false;
            }
        }

        return true;
    }

}}

#endif
