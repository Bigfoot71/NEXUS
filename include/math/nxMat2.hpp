#ifndef NEXUS_MATH_MAT2_HPP
#define NEXUS_MATH_MAT2_HPP

#include "./nxVec2.hpp"

namespace nexus { namespace math {

    /**
     * @brief 2x2 Matrix structure
     */
    struct NEXUS_API Mat2
    {
        float m[4]{};   ///< 2x2 matrix elements

        /**
         * @brief Default constructor
         */
        constexpr Mat2() = default;

        /**
         * @brief Constructor from array
         * @param mat Pointer to an array of 4 floats representing the matrix elements
         */
        Mat2(const float *mat)
        {
            std::copy(mat, mat + 4, m);
        }

        /**
         * @brief Constructor from individual elements
         * @param m0 Element at index (0, 0)
         * @param m2 Element at index (0, 1)
         * @param m1 Element at index (1, 0)
         * @param m3 Element at index (1, 1)
         */
        constexpr Mat2(float m0, float m2, float m1, float m3)
        {
            m[0] = m0; m[2] = m1;
            m[1] = m2; m[3] = m3;
        }

        /**
         * @brief Returns the identity matrix
         * @return Identity matrix
         */
        static constexpr Mat2 Identity()
        {
            return {
                1.0f, 0.0f,
                0.0f, 1.0f
            };
        }

        /**
         * @brief Calculates the determinant of the matrix
         * @return Determinant of the matrix
         */
        constexpr float Determinant() const
        {
            return m[0] * m[3] - m[1] * m[2];
        }

        /**
         * @brief Calculates the trace of the matrix
         * @return Trace of the matrix
         */
        constexpr float Trace() const
        {
            return m[0] + m[3];
        }

        /**
         * @brief Transposes the matrix
         * @return Transposed matrix
         */
        constexpr Mat2 Transpose() const
        {
            return {
                m[0], m[1],
                m[2], m[3]
            };
        }

        /**
         * @brief Inverts the matrix if it is invertible
         * @return Inverted matrix
         */
        constexpr Mat2 Invert() const
        {
            float det = Determinant();
            if (det == 0.0f) return Mat2::Identity();

            float invDet = 1.0f / det;
            return Mat2(m[3] * invDet, -m[1] * invDet, -m[2] * invDet, m[0] * invDet);
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
         * @brief Adds two matrices
         * @param other Matrix to add
         * @return Result of matrix addition
         */
        constexpr Mat2 operator+(const Mat2& other) const
        {
            return Mat2(m[0] + other.m[0], m[1] + other.m[1], m[2] + other.m[2], m[3] + other.m[3]);
        }

        /**
         * @brief Subtracts two matrices
         * @param other Matrix to subtract
         * @return Result of matrix subtraction
         */
        constexpr Mat2 operator-(const Mat2& other) const
        {
            return Mat2(m[0] - other.m[0], m[1] - other.m[1], m[2] - other.m[2], m[3] - other.m[3]);
        }

        /**
         * @brief Multiplies two matrices
         * @param other Matrix to multiply by
         * @return Result of matrix multiplication
         */
        constexpr Mat2 operator*(const Mat2& other) const
        {
            return {
                m[0] * other.m[0] + m[1] * other.m[2], m[2] * other.m[0] + m[3] * other.m[2],
                m[0] * other.m[1] + m[1] * other.m[3], m[2] * other.m[1] + m[3] * other.m[3]
            };
        }

        /**
         * @brief Multiplies the matrix by a scalar
         * @param scalar Scalar value to multiply by
         * @return Result of scalar multiplication
         */
        constexpr Mat2 operator*(float scalar) const
        {
            return Mat2(m[0] * scalar, m[1] * scalar, m[2] * scalar, m[3] * scalar);
        }

        // Compound assignment operators

        /**
         * @brief Adds another matrix to this matrix
         * @param other Matrix to add
         */
        void operator+=(const Mat2& other)
        {
            *this = *this + other;
        }

        /**
         * @brief Subtracts another matrix from this matrix
         * @param other Matrix to subtract
         */
        void operator-=(const Mat2& other)
        {
            *this = *this - other;
        }

        /**
         * @brief Multiplies this matrix by another matrix
         * @param other Matrix to multiply by
         */
        void operator*=(const Mat2& other)
        {
            *this = *this * other;
        }

        /**
         * @brief Multiplies this matrix by a scalar
         * @param scalar Scalar value to multiply by
         */
        void operator*=(float scalar)
        {
            *this = *this * scalar;
        }

        // Comparison operators

        /**
         * @brief Checks if two matrices are equal
         * @param other Matrix to compare with
         * @return True if matrices are equal, false otherwise
         */
        bool operator==(const Mat2& other) const
        {
            return (m[0] == other.m[0] && m[1] == other.m[1] && m[2] == other.m[2] && m[3] == other.m[3]);
        }

        /**
         * @brief Checks if two matrices are not equal
         * @param other Matrix to compare with
         * @return True if matrices are not equal, false otherwise
         */
        bool operator!=(const Mat2& other) const
        {
            return !(*this == other);
        }
    };

}}

#endif
