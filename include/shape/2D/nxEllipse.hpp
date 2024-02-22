#ifndef NEXUS_SHAPE_2D_ELLIPSE_HPP
#define NEXUS_SHAPE_2D_ELLIPSE_HPP

#include "./nxCollisionShape.hpp"

namespace nexus { namespace shape2D {

    /**
     * @brief 2D Ellipse collision shape.
     */
    struct NEXUS_API Ellipse : CollisionShape
    {
        math::Vec2 center;  ///< Center point of the ellipse.
        math::Vec2 radius;  ///< Radius (half-lengths) of the ellipse along x and y axes.

        /**
         * @brief Default constructor initializing center and radius to (0,0).
         */
        constexpr Ellipse() : center(), radius() { }

        /**
         * @brief Constructor initializing the ellipse with specified center coordinates and radii.
         * @param x X-coordinate of the center.
         * @param y Y-coordinate of the center.
         * @param rx Radius along the x-axis.
         * @param ry Radius along the y-axis.
         */
        constexpr Ellipse(float x, float y, float rx, float ry)
        : center(x, y), radius(rx, ry)
        { }

        /**
         * @brief Constructor initializing the ellipse with specified center vector and radii.
         * @param _center Center point of the ellipse.
         * @param _radius Radii along x and y axes.
         */
        constexpr Ellipse(const math::Vec2& center, const math::Vec2& radius)
        : center(center), radius(radius)
        { }

        /**
         * @brief Equality comparison operator for ellipses.
         * @param other Another ellipse to compare against.
         * @return True if the two ellipses are equal, false otherwise.
         */
        constexpr bool operator==(const Ellipse& other) const
        {
            return center == other.center && radius == other.radius;
        }

        /**
         * @brief Inequality comparison operator for ellipses.
         * @param other Another ellipse to compare against.
         * @return True if the two ellipses are not equal, false otherwise.
         */
        constexpr bool operator!=(const Ellipse& other) const
        {
            return !(*this == other);
        }

        /**
         * @brief Get the Axis-Aligned Bounding Box (AABB) of the ellipse.
         * @return The AABB representing the ellipse's bounding box.
         */
        AABB GetAABB() const;

        /**
         * @brief Check collision between the ellipse and an AABB.
         * @param aabb The AABB to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionAABB(const AABB& aabb) const override;

        /**
         * @brief Check collision between the ellipse and a point.
         * @param point The point to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec2& point) const override;

        /**
         * @brief Check collision between the ellipse and a line.
         * @param line The line to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionLine(const Line& line) const override;

        /**
         * @brief Check collision between the ellipse and a mesh.
         * @param mesh The mesh to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionMesh(const Mesh& mesh) const override;

        /**
         * @brief Check collision between the ellipse and a circle.
         * @param circle The circle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionCircle(const Circle& circle) const override;

        /**
         * @brief Check collision between the ellipse and another ellipse.
         * @param ellipse The ellipse to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionEllipse(const Ellipse& ellipse) const override;

        /**
         * @brief Check collision between the ellipse and a polygon.
         * @param poly The polygon to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPolygon(const Polygon& poly) const override;

        /**
         * @brief Check collision between the ellipse and a triangle.
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const Triangle& tri) const override;

        /**
         * @brief Check collision between the ellipse and a triangle (with float coordinates).
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const TriangleF& tri) const override;

        /**
         * @brief Check collision between the ellipse and a rectangle.
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const Rectangle& rect) const override;

        /**
         * @brief Check collision between the ellipse and a rectangle (with float coordinates).
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const RectangleF& rect) const override;
    };

}}

#endif //NEXUS_SHAPE_2D_ELLIPSE_HPP
