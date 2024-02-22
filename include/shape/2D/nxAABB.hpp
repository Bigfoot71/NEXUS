#ifndef NEXUS_SHAPE_2D_AABB_HPP
#define NEXUS_SHAPE_2D_AABB_HPP

#include "./nxCollisionShape.hpp"

namespace nexus { namespace shape2D {

    /**
     * @brief Axis-Aligned Bounding Box (AABB) 2D collision shape.
     */
    struct NEXUS_API AABB : CollisionShape
    {
        math::Vec2 min, max;    ///< Minimum and maximum corner points of the AABB.

        /**
         * @brief Default constructor.
         */
        constexpr AABB() = default;

        /**
         * @brief Constructor initializing the AABB with specified minimum and maximum coordinates.
         * @param minX Minimum x-coordinate.
         * @param minY Minimum y-coordinate.
         * @param maxX Maximum x-coordinate.
         * @param maxY Maximum y-coordinate.
         */
        constexpr AABB(float minX, float minY, float maxX, float maxY)
        : min(minX, minY), max(maxX, maxY)
        { }

        /**
         * @brief Constructor initializing the AABB with specified minimum and maximum vectors.
         * @param min Minimum corner point.
         * @param max Maximum corner point.
         */
        constexpr AABB(const math::Vec2 min, const math::Vec2 max)
        : min(min), max(max)
        { }

        /**
         * @brief Equality comparison operator for AABBs.
         * @param other Another AABB to compare against.
         * @return True if the two AABBs are equal, false otherwise.
         */
        constexpr bool operator==(const AABB& other) const
        {
            return min == other.min && max == other.max;
        }

        /**
         * @brief Inequality comparison operator for AABBs.
         * @param other Another AABB to compare against.
         * @return True if the two AABBs are not equal, false otherwise.
         */
        constexpr bool operator!=(const AABB& other) const
        {
            return !(*this == other);
        }

        /**
         * @brief Get the rectangle representation of the AABB.
         * @return A rectangle representing the AABB.
         */
        RectangleF GetRectangle() const;

        /**
         * @brief Check collision between two AABBs.
         * @param aabb Another AABB to check against.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionAABB(const AABB& aabb) const override;

        /**
         * @brief Check collision between a point and the AABB.
         * @param point The point to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec2& point) const override;

        /**
         * @brief Check collision between a line and the AABB.
         * @param line The line to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionLine(const Line& line) const override;

        /**
         * @brief Check collision between a mesh and the AABB.
         * @param mesh The mesh to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionMesh(const Mesh& mesh) const override;

        /**
         * @brief Check collision between a circle and the AABB.
         * @param circle The circle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionCircle(const Circle& circle) const override;

        /**
         * @brief Check collision between an ellipse and the AABB.
         * @param ellipse The ellipse to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionEllipse(const Ellipse& ellipse) const override;

        /**
         * @brief Check collision between a polygon and the AABB.
         * @param poly The polygon to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPolygon(const Polygon& poly) const override;

        /**
         * @brief Check collision between a triangle and the AABB.
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const Triangle& tri) const override;

        /**
         * @brief Check collision between a triangle (with float coordinates) and the AABB.
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const TriangleF& tri) const override;

        /**
         * @brief Check collision between a rectangle and the AABB.
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const Rectangle& rect) const override;

        /**
         * @brief Check collision between a rectangle (with float coordinates) and the AABB.
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const RectangleF& rect) const override;
    };

}}

#endif //NEXUS_SHAPE_2D_AABB_HPP
