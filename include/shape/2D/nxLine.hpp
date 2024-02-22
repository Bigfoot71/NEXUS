#ifndef NEXUS_SHAPE_2D_LINE_HPP
#define NEXUS_SHAPE_2D_LINE_HPP

#include "./nxCollisionShape.hpp"

namespace nexus { namespace shape2D {

    /**
     * @brief 2D Line collision shape.
     */
    struct NEXUS_API Line : CollisionShape
    {
        math::Vec2 start;   ///< Start point of the line.
        math::Vec2 end;     ///< End point of the line.

        /**
         * @brief Default constructor initializing start and end points to (0,0).
         */
        constexpr Line() : start(), end() { }

        /**
         * @brief Constructor initializing the line with specified start and end coordinates.
         * @param x1 X-coordinate of the start point.
         * @param y1 Y-coordinate of the start point.
         * @param x2 X-coordinate of the end point.
         * @param y2 Y-coordinate of the end point.
         */
        constexpr Line(float x1, float y1, float x2, float y2)
        : start(x1, y1), end(x2, y2)
        { }

        /**
         * @brief Constructor initializing the line with specified start and end vectors.
         * @param a Start point of the line.
         * @param b End point of the line.
         */
        constexpr Line(const math::Vec2& a, const math::Vec2& b)
        : start(a), end(b)
        { }

        /**
         * @brief Equality comparison operator for lines.
         * @param other Another line to compare against.
         * @return True if the two lines are equal, false otherwise.
         */
        constexpr bool operator==(const Line& other) const
        {
            return start == other.start && end == other.end;
        }

        /**
         * @brief Inequality comparison operator for lines.
         * @param other Another line to compare against.
         * @return True if the two lines are not equal, false otherwise.
         */
        constexpr bool operator!=(const Line& other) const
        {
            return !(*this == other);
        }

        /**
         * @brief Get the Axis-Aligned Bounding Box (AABB) of the line.
         * @return The AABB representing the line's bounding box.
         */
        AABB GetAABB() const;

        /**
         * @brief Check collision between the line and an AABB.
         * @param aabb The AABB to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionAABB(const AABB& aabb) const override;

        /**
         * @brief Check collision between the line and a point.
         * @param point The point to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec2& point) const override;

        /**
         * @brief Check collision between the line and another line.
         * @param line The line to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionLine(const Line& line) const override;

        /**
         * @brief Check collision between the line and a mesh.
         * @param mesh The mesh to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionMesh(const Mesh& mesh) const override;

        /**
         * @brief Check collision between the line and a circle.
         * @param circle The circle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionCircle(const Circle& circle) const override;

        /**
         * @brief Check collision between the line and an ellipse.
         * @param ellipse The ellipse to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionEllipse(const Ellipse& ellipse) const override;

        /**
         * @brief Check collision between the line and a polygon.
         * @param poly The polygon to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPolygon(const Polygon& poly) const override;

        /**
         * @brief Check collision between the line and a triangle.
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const Triangle& tri) const override;

        /**
         * @brief Check collision between the line and a triangle (with float coordinates).
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const TriangleF& tri) const override;

        /**
         * @brief Check collision between the line and a rectangle.
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const Rectangle& rect) const override;

        /**
         * @brief Check collision between the line and a rectangle (with float coordinates).
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const RectangleF& rect) const override;
    };

}}

#endif //NEXUS_SHAPE_2D_LINE_HPP
