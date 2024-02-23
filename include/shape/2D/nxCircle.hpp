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

#ifndef NEXUS_SHAPE_2D_CIRCLE_HPP
#define NEXUS_SHAPE_2D_CIRCLE_HPP

#include "./nxCollisionShape.hpp"

namespace nexus { namespace shape2D {

    /**
     * @brief 2D Circle collision shape.
     */
    struct NEXUS_API Circle : CollisionShape
    {
        math::Vec2 center;      ///< Center point of the circle.
        float radius;           ///< Radius of the circle.

        /**
         * @brief Default constructor initializing center at (0,0) and radius to 0.
         */
        constexpr Circle() : center(), radius(0) { }

        /**
         * @brief Constructor initializing the circle with specified center coordinates and radius.
         * @param x X-coordinate of the center.
         * @param y Y-coordinate of the center.
         * @param radius Radius of the circle.
         */
        constexpr Circle(float x, float y, float radius)
        : center(x, y), radius(radius)
        { }

        /**
         * @brief Constructor initializing the circle with specified center vector and radius.
         * @param center Center point of the circle.
         * @param radius Radius of the circle.
         */
        constexpr Circle(const math::Vec2& center, float radius)
        : center(center), radius(radius)
        { }

        /**
         * @brief Equality comparison operator for circles.
         * @param other Another circle to compare against.
         * @return True if the two circles are equal, false otherwise.
         */
        constexpr bool operator==(const Circle& other) const
        {
            return center == other.center && radius == other.radius;
        }

        /**
         * @brief Inequality comparison operator for circles.
         * @param other Another circle to compare against.
         * @return True if the two circles are not equal, false otherwise.
         */
        constexpr bool operator!=(const Circle& other) const
        {
            return !(*this == other);
        }

        /**
         * @brief Get the Axis-Aligned Bounding Box (AABB) of the circle.
         * @return The AABB representing the circle's bounding box.
         */
        AABB GetAABB() const;

        /**
         * @brief Check collision between the circle and an AABB.
         * @param aabb The AABB to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionAABB(const AABB& aabb) const override;

        /**
         * @brief Check collision between the circle and a point.
         * @param point The point to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec2& point) const override;

        /**
         * @brief Check collision between the circle and a line.
         * @param line The line to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionLine(const Line& line) const override;

        /**
         * @brief Check collision between the circle and a mesh.
         * @param mesh The mesh to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionMesh(const Mesh& mesh) const override;

        /**
         * @brief Check collision between the circle and another circle.
         * @param circle The circle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionCircle(const Circle& circle) const override;

        /**
         * @brief Check collision between the circle and an ellipse.
         * @param ellipse The ellipse to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionEllipse(const Ellipse& ellipse) const override;

        /**
         * @brief Check collision between the circle and a polygon.
         * @param poly The polygon to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPolygon(const Polygon& poly) const override;

        /**
         * @brief Check collision between the circle and a triangle.
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const Triangle& tri) const override;

        /**
         * @brief Check collision between the circle and a triangle (with float coordinates).
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const TriangleF& tri) const override;

        /**
         * @brief Check collision between the circle and a rectangle.
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const Rectangle& rect) const override;

        /**
         * @brief Check collision between the circle and a rectangle (with float coordinates).
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const RectangleF& rect) const override;
    };

}}

#endif //NEXUS_SHAPE_2D_CIRCLE_HPP