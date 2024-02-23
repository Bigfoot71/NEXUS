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

#ifndef NEXUS_SHAPE_2D_TRIANGLE_HPP
#define NEXUS_SHAPE_2D_TRIANGLE_HPP

#include "./nxCollisionShape.hpp"
#include "../../math/nxVec3.hpp"

namespace nexus { namespace shape2D {

    /**
     * @brief A struct representing a 2D triangle with integer corrdinates.
     * This class with integer coordinates exists for optimization purposes for rasterization.
     */
    struct NEXUS_API Triangle : CollisionShape
    {
        math::IVec2 a, b, c;    ///< Vertices of the triangle.

        /**
         * @brief Default constructor initializing an empty triangle.
         */
        constexpr Triangle() = default;

        /**
         * @brief Constructor initializing the triangle with three vertices.
         * @param a The first vertex of the triangle.
         * @param b The second vertex of the triangle.
         * @param c The third vertex of the triangle.
         */
        constexpr Triangle(const math::IVec2& a, const math::IVec2& b, const math::IVec2& c)
        : a(a), b(b), c(c)
        { }

        /**
         * @brief Constructor initializing the triangle with three floating-point vertices.
         * @param tri The triangle with floating-point coordinates to initialize from.
         */
        constexpr Triangle(const TriangleF& tri);

        /**
         * @brief Equality comparison operator for triangles.
         * @param other Another triangle to compare against.
         * @return True if the two triangles are equal, false otherwise.
         */
        constexpr bool operator==(const Triangle& other) const
        {
            return a == other.a && b == other.b && c == other.c;
        }

        /**
         * @brief Inequality comparison operator for triangles.
         * @param other Another triangle to compare against.
         * @return True if the two triangles are not equal, false otherwise.
         */
        constexpr bool operator!=(const Triangle& other) const
        {
            return *this != other;
        }

        /**
         * @brief Accesses the vertices of the triangle by index.
         * @param index The index of the vertex (0, 1, or 2).
         * @return A reference to the vertex at the specified index.
         */
        math::IVec2& operator[](int index)
        {
            return *(reinterpret_cast<math::IVec2*>(this) + index);
        }

        /**
         * @brief Accesses the vertices of the triangle by index (const version).
         * @param index The index of the vertex (0, 1, or 2).
         * @return A reference to the vertex at the specified index.
         */
        const math::IVec2& operator[](int index) const
        {
            return *(reinterpret_cast<const math::IVec2*>(this) + index);
        }

        /**
         * @brief Computes the barycentric coordinates of a point within the triangle.
         * @param p The point for which to compute the barycentric coordinates.
         * @return The barycentric coordinates of the point.
         */
        math::Vec3 BarycentricCoordinates(const math::IVec2& p) const;

        /**
         * @brief Computes the signed area of the triangle.
         * @details If the signed area is positive, the vertices are ordered ClockWise (CW),
         *          if it is negative, they are ordered CounterClockWise (CCW), and if it is zero,
         *          the triangle is degenerate (flat).
         * @return The signed area of the triangle.
         */
        int GetSignedArea() const
        {
            return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
        }

        /**
         * @brief Computes the area of the triangle.
         * @details The area is always non-negative.
         * @return The area of the triangle.
         */
        unsigned GetArea() const
        {
            return std::abs(GetSignedArea()) / 2;
        }

        /**
         * @brief Get the Axis-Aligned Bounding Box (AABB) of the triangle.
         * @return The AABB representing the bounding box of the triangle.
         */
        AABB GetAABB() const;

        /**
         * @brief Check collision between the triangle and an AABB.
         * @param aabb The AABB to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionAABB(const AABB& aabb) const override;

        /**
         * @brief Check collision between the triangle and a point.
         * @param point The point to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec2& point) const override;

        /**
         * @brief Check collision between the triangle and a line.
         * @param line The line to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionLine(const Line& line) const override;

        /**
         * @brief Check collision between the triangle and a mesh.
         * @param mesh The mesh to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionMesh(const Mesh& mesh) const override;

        /**
         * @brief Check collision between the triangle and a circle.
         * @param circle The circle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionCircle(const Circle& circle) const override;

        /**
         * @brief Check collision between the triangle and an ellipse.
         * @param ellipse The ellipse to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionEllipse(const Ellipse& ellipse) const override;

        /**
         * @brief Check collision between the triangle and a polygon.
         * @param poly The polygon to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPolygon(const Polygon& poly) const override;

        /**
         * @brief Check collision between the triangle and another triangle.
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const Triangle& tri) const override;

        /**
         * @brief Check collision between the triangle and another triangle (with float coordinates).
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const TriangleF& tri) const override;

        /**
         * @brief Check collision between the triangle and a rectangle.
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const Rectangle& rect) const override;

        /**
         * @brief Check collision between the triangle and a rectangle (with float coordinates).
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const RectangleF& rect) const override;
    };

    /**
     * @brief A struct representing a 2D triangle.
     */
    struct NEXUS_API TriangleF : CollisionShape
    {
        math::Vec2 a, b, c;     ///< Vertices of the triangle.

        /**
         * @brief Default constructor initializing an empty triangle.
         */
        constexpr TriangleF() = default;

        /**
         * @brief Constructor initializing the triangle with three vertices.
         * @param a The first vertex of the triangle.
         * @param b The second vertex of the triangle.
         * @param c The third vertex of the triangle.
         */
        constexpr TriangleF(const math::Vec2& a, const math::Vec2& b, const math::Vec2& c)
        : a(a), b(b), c(c)
        { }

        /**
         * @brief Constructor initializing the triangle with three integer vertices.
         * @param tri The triangle with integer coordinates to initialize from.
         */
        constexpr TriangleF(const Triangle& tri)
        : a(tri.a), b(tri.b), c(tri.c)
        { }

        /**
         * @brief Equality comparison operator for triangles.
         * @param other Another triangle to compare against.
         * @return True if the two triangles are equal, false otherwise.
         */
        constexpr bool operator==(const Triangle& other) const
        {
            return a == other.a && b == other.b && c == other.c;
        }

        /**
         * @brief Inequality comparison operator for triangles.
         * @param other Another triangle to compare against.
         * @return True if the two triangles are not equal, false otherwise.
         */
        constexpr bool operator!=(const Triangle& other) const
        {
            return *this != other;
        }

        /**
         * @brief Accesses the vertices of the triangle by index.
         * @param index The index of the vertex (0, 1, or 2).
         * @return A reference to the vertex at the specified index.
         */
        math::Vec2& operator[](int index)
        {
            return *(reinterpret_cast<math::Vec2*>(this) + index);
        }

        /**
         * @brief Accesses the vertices of the triangle by index (const version).
         * @param index The index of the vertex (0, 1, or 2).
         * @return A reference to the vertex at the specified index.
         */
        const math::Vec2& operator[](int index) const
        {
            return *(reinterpret_cast<const math::Vec2*>(this) + index);
        }

        /**
         * @brief Computes the barycentric coordinates of a point within the triangle.
         * @param p The point for which to compute the barycentric coordinates.
         * @return The barycentric coordinates of the point.
         */
        math::Vec3 BarycentricCoordinates(const math::Vec2& p) const;

        /**
         * @brief Computes the signed area of the triangle.
         * @details If the signed area is positive, the vertices are ordered ClockWise (CW),
         *          if it is negative, they are ordered CounterClockWise (CCW), and if it is zero,
         *          the triangle is degenerate (flat).
         * @return The signed area of the triangle.
         */
        float GetSignedArea() const
        {
            return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
        }

        /**
         * @brief Computes the area of the triangle.
         * @details The area is always non-negative.
         * @return The area of the triangle.
         */
        float GetArea() const
        {
            return std::abs(GetSignedArea() * 0.5f);
        }

        /**
         * @brief Get the Axis-Aligned Bounding Box (AABB) of the triangle.
         * @return The AABB representing the bounding box of the triangle.
         */
        AABB GetAABB() const;

        /**
         * @brief Check collision between the triangle and an AABB.
         * @param aabb The AABB to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionAABB(const AABB& aabb) const override;

        /**
         * @brief Check collision between the triangle and a point.
         * @param point The point to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec2& point) const override;

        /**
         * @brief Check collision between the triangle and a line.
         * @param line The line to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionLine(const Line& line) const override;

        /**
         * @brief Check collision between the triangle and a mesh.
         * @param mesh The mesh to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionMesh(const Mesh& mesh) const override;

        /**
         * @brief Check collision between the triangle and a circle.
         * @param circle The circle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionCircle(const Circle& circle) const override;

        /**
         * @brief Check collision between the triangle and an ellipse.
         * @param ellipse The ellipse to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionEllipse(const Ellipse& ellipse) const override;

        /**
         * @brief Check collision between the triangle and a polygon.
         * @param poly The polygon to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPolygon(const Polygon& poly) const override;

        /**
         * @brief Check collision between the triangle and another triangle.
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const Triangle& tri) const override;

        /**
         * @brief Check collision between the triangle and another triangle (with float coordinates).
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const TriangleF& tri) const override;

        /**
         * @brief Check collision between the triangle and a rectangle.
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const Rectangle& rect) const override;

        /**
         * @brief Check collision between the triangle and a rectangle (with float coordinates).
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const RectangleF& rect) const override;
    };

}}

#endif // NEXUS_SHAPE_2D_TRIANGLE_HPP
