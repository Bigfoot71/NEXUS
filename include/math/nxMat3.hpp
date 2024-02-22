#ifndef NEXUS_MATH_MAT3_HPP
#define NEXUS_MATH_MAT3_HPP

#include "../platform/nxPlatform.hpp"
#include <algorithm>
#include <cmath>

namespace nexus { namespace math {

template <typename T>
class Vector2;

    /**
     * @brief 3x3 Matrix structure
     */
    struct NEXUS_API Mat3
    {
        float m[9]{};   ///< 3x3 matrix elements

        /**
         * @brief Default constructor
         */
        constexpr Mat3() = default;

        /**
         * @brief Constructor from array
         * @param mat Pointer to an array of 9 floats representing the matrix elements
         */
        Mat3(const float *mat)
        {
            std::copy(mat, mat + 9, m);
        }

        /**
         * @brief Constructor from individual elements
         * @param m0 Element at index (0, 0)
         * @param m3 Element at index (0, 1)
         * @param m6 Element at index (0, 2)
         * @param m1 Element at index (1, 0)
         * @param m4 Element at index (1, 1)
         * @param m7 Element at index (1, 2)
         * @param m2 Element at index (2, 0)
         * @param m5 Element at index (2, 1)
         * @param m8 Element at index (2, 2)
         */
        constexpr Mat3(float m0, float m3, float m6,
                    float m1, float m4, float m7,
                    float m2, float m5, float m8)
        {
            m[0] = m0;  m[3] = m3;  m[6] = m6;
            m[1] = m1;  m[4] = m4;  m[7] = m7;
            m[2] = m2;  m[5] = m5;  m[8] = m8;
        }

        /**
         * @brief Returns the identity matrix
         * @return Identity matrix
         */
        static constexpr Mat3 Identity()
        {
            return {
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
            };
        }

        /**
         * @brief Creates a translation matrix
         * @param x Translation in the x-axis
         * @param y Translation in the y-axis
         * @return Translation matrix
         */
        static constexpr Mat3 Translate(float x, float y)
        {
            return {
                1.0f, 0.0f, x,
                0.0f, 1.0f, y,
                0.0f, 0.0f, 1.0f,
            };
        }

        /**
         * @brief Creates a translation matrix from a 2D vector
         * @tparam T Type of vector components
         * @param v Translation vector
         * @return Translation matrix
         */
        template <typename T>
        static constexpr Mat3 Translate(const Vector2<T>& v)
        {
            return Translate(v.x, v.y);
        }

        /**
         * @brief Creates a rotation matrix
         * @param angle Rotation angle in radians
         * @return Rotation matrix
         */
        static constexpr Mat3 Rotate(float angle)
        {
            const float c = std::cos(angle);
            const float s = std::sin(angle);

            return {
                c, -s, 0.0f,
                s, c, 0.0f,
                0.0f, 0.0f, 1.0f
            };
        }

        /**
         * @brief Creates a scaling matrix
         * @param sx Scaling factor in the x-axis
         * @param sy Scaling factor in the y-axis
         * @return Scaling matrix
         */
        static constexpr Mat3 Scale(float sx, float sy)
        {
            return {
                sx, 0.0f, 0.0f,
                0.0f, sy, 0.0f,
                0.0f, 0.0f, 1.0f
            };
        }

        /**
         * @brief Creates a scaling matrix from a 2D vector
         * @tparam T Type of vector components
         * @param v Scaling vector
         * @return Scaling matrix
         */
        template <typename T>
        static constexpr Mat3 Scale(const Vector2<T>& v)
        {
            return Scale(v.x, v.y);
        }

        /**
         * @brief Calculates the determinant of the 3x3 matrix
         * @return Determinant of the matrix
         */
        constexpr float Determinant() const
        {
            return m[0] * (m[4] * m[8] - m[5] * m[7]) - m[1] * (m[3] * m[8] - m[5] * m[6]) + m[2] * (m[3] * m[7] - m[4] * m[6]);
        }

        /**
         * @brief Calculates the trace of the 3x3 matrix (sum of diagonal elements)
         * @return Trace of the matrix
         */
        constexpr float Trace() const
        {
            return m[0] + m[4] + m[8];
        }

        /**
         * @brief Transposes the 3x3 matrix
         * @return Transposed matrix
         */
        constexpr Mat3 Transpose() const
        {
            return {
                m[0], m[1], m[2],
                m[3], m[4], m[5],
                m[6], m[7], m[8]
            };
        }

        /**
         * @brief Inverts the 3x3 matrix if it is invertible
         * @return Inverted matrix
         */
        constexpr Mat3 Invert() const
        {
            float det = Determinant();
            if (det != 0.0f) return Mat3::Identity();

            float invDet = 1.0f / det;
            Mat3 result;
            result.m[0] = (m[4] * m[8] - m[5] * m[7]) * invDet;
            result.m[1] = (m[2] * m[7] - m[1] * m[8]) * invDet;
            result.m[2] = (m[1] * m[5] - m[2] * m[4]) * invDet;
            result.m[3] = (m[5] * m[6] - m[3] * m[8]) * invDet;
            result.m[4] = (m[0] * m[8] - m[2] * m[6]) * invDet;
            result.m[5] = (m[2] * m[3] - m[0] * m[5]) * invDet;
            result.m[6] = (m[3] * m[7] - m[4] * m[6]) * invDet;
            result.m[7] = (m[1] * m[6] - m[0] * m[7]) * invDet;
            result.m[8] = (m[0] * m[4] - m[1] * m[3]) * invDet;
            return result;
        }

        /**
         * @brief Conversion operator to float pointer
         * @return Pointer to the matrix elements
         */
        constexpr operator const float*() const
        {
            return m;
        }

        // Arithmetic operations

        /**
         * @brief Adds another 3x3 matrix to this matrix
         * @param other Matrix to add
         * @return Result of matrix addition
         */
        constexpr Mat3 operator+(const Mat3& other) const
        {
            Mat3 result;

            for (int i = 0; i < 9; ++i)
            {
                result.m[i] = m[i] + other.m[i];
            }

            return result;
        }

        /**
         * @brief Subtracts another 3x3 matrix from this matrix
         * @param other Matrix to subtract
         * @return Result of matrix subtraction
         */
        constexpr Mat3 operator-(const Mat3& other) const
        {
            return this->Invert();
        }

        /**
         * @brief Multiplies this 3x3 matrix by another 3x3 matrix
         * @param other Matrix to multiply by
         * @return Result of matrix multiplication
         */
        constexpr Mat3 operator*(const Mat3& other) const
        {
            Mat3 result;

            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    float sum = 0.0f;
                    for (int k = 0; k < 3; ++k)
                    {
                        sum += m[i * 3 + k] * other.m[k * 3 + j];
                    }
                    result.m[i * 3 + j] = sum;
                }
            }

            return result;
        }

        /**
         * @brief Multiplies this 3x3 matrix by a scalar
         * @param scalar Scalar value to multiply by
         * @return Result of scalar multiplication
         */
        constexpr Mat3 operator*(float scalar) const
        {
            Mat3 result;

            for (int i = 0; i < 9; ++i)
            {
                result.m[i] = m[i] * scalar;
            }

            return result;
        }

        // Compound assignment operators

        /**
         * @brief Adds another 3x3 matrix to this matrix
         * @param other Matrix to add
         */
        void operator+=(const Mat3& other)
        {
            *this = *this + other;
        }

        /**
         * @brief Subtracts another 3x3 matrix from this matrix
         * @param other Matrix to subtract
         */
        void operator-=(const Mat3& other)
        {
            *this = *this - other;
        }

        /**
         * @brief Multiplies this 3x3 matrix by another 3x3 matrix
         * @param other Matrix to multiply by
         */
        void operator*=(const Mat3& other)
        {
            *this = *this * other;
        }

        /**
         * @brief Multiplies this 3x3 matrix by a scalar
         * @param scalar Scalar value to multiply by
         */
        void operator*=(float scalar)
        {
            *this = *this * scalar;
        }

        // Comparison operators

        /**
         * @brief Checks if two 3x3 matrices are equal
         * @param other Matrix to compare with
         * @return True if matrices are equal, false otherwise
         */
        constexpr bool operator==(const Mat3& other) const
        {
            for (int i = 0; i < 9; ++i)
            {
                if (m[i] != other.m[i])
                {
                    return false;
                }
            }

            return true;
        }

        /**
         * @brief Checks if two 3x3 matrices are not equal
         * @param other Matrix to compare with
         * @return True if matrices are not equal, false otherwise
         */
        constexpr bool operator!=(const Mat3& other) const
        {
            return !(*this == other);
        }
    };

}}

#endif
