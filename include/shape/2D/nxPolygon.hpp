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

#ifndef NEXUS_SHAPE_2D_POLYGON_HPP
#define NEXUS_SHAPE_2D_POLYGON_HPP

#include "./nxCollisionShape.hpp"
#include <algorithm>
#include <limits>
#include <vector>

namespace nexus { namespace shape2D {

    /**
     * @brief 2D Polygon collision shape.
     */
    struct NEXUS_API Polygon : CollisionShape
    {
        std::vector<math::Vec2> vertices; ///< Vertices of the polygon.

        /**
         * @brief Default constructor.
         */
        Polygon() = default;

        /**
         * @brief Constructor initializing the polygon with specified vertices.
         * @param verts List of vertices defining the polygon.
         */
        Polygon(const std::vector<math::Vec2>& verts) : vertices(verts) { }

        /**
         * @brief Constructor initializing the polygon with specified vertices using initializer list.
         * @param initList Initializer list of vertices.
         */
        Polygon(std::initializer_list<math::Vec2> initList) : vertices(initList) { }

        /**
         * @brief Constructor initializing the polygon with specified vertices using initializer list of floats.
         * @details This constructor takes an initializer list of floats, interprets them as pairs of (x, y) coordinates,
         *          and constructs Vec2 objects from them to populate the vertices of the polygon.
         * @param initList Initializer list of floats representing (x, y) coordinates of vertices.
         */
        Polygon(std::initializer_list<float> initList)
        {
            vertices.reserve(initList.size() / 2);
            std::copy(initList.begin(), initList.end(),
                reinterpret_cast<float*>(vertices.data()));
        }

        /**
         * @brief Assignment operator initializing the polygon with specified vertices using initializer list.
         * @param initList Initializer list of vertices.
         * @return Reference to the modified polygon.
         */
        Polygon& operator=(std::initializer_list<math::Vec2> initList);

        /**
         * @brief Equality comparison operator for polygons.
         * @param other Another polygon to compare against.
         * @return True if the two polygons are equal, false otherwise.
         */
        bool operator==(const Polygon& other) const;

        /**
         * @brief Inequality comparison operator for polygons.
         * @param other Another polygon to compare against.
         * @return True if the two polygons are not equal, false otherwise.
         */
        bool operator!=(const Polygon& other) const;

        /**
         * @brief Create an equilateral triangle polygon with a specified center and side length.
         * @param cx X-coordinate of the center.
         * @param cy Y-coordinate of the center.
         * @param sideLength Length of each side of the equilateral triangle.
         * @return Equilateral triangle polygon.
         */
        static Polygon NewTriangleEquilateral(float cx, float cy, float sideLength);

        /**
         * @brief Create an isosceles triangle polygon with a specified center and side length.
         * @param cx X-coordinate of the center.
         * @param cy Y-coordinate of the center.
         * @param sideLength Length of the equal sides of the isosceles triangle.
         * @return Isosceles triangle polygon.
         */
        static Polygon NewTriangleIsosceles(float cx, float cy, float sideLength);

        /**
         * @brief Create a right-angled triangle polygon with a specified center and side length.
         * @param cx X-coordinate of the center.
         * @param cy Y-coordinate of the center.
         * @param sideLength Length of the sides adjacent to the right angle.
         * @return Right-angled triangle polygon.
         */
        static Polygon NewTriangleRectangle(float cx, float cy, float sideLength);

        /**
         * @brief Create a rectangle polygon with a specified center, width, and height.
         * @param cx X-coordinate of the center.
         * @param cy Y-coordinate of the center.
         * @param width Width of the rectangle.
         * @param height Height of the rectangle.
         * @return Rectangle polygon.
         */
        static Polygon NewRectangle(float cx, float cy, float width, float height);

        /**
         * @brief Create an ellipse polygon with a specified center, radii, and number of segments.
         * @param cx X-coordinate of the center.
         * @param cy Y-coordinate of the center.
         * @param rx Radius along the x-axis.
         * @param ry Radius along the y-axis.
         * @param segments Number of segments used to approximate the ellipse.
         * @return Ellipse polygon.
         */
        static Polygon NewEllipse(float cx, float cy, float rx, float ry, int segments = 36);

        /**
         * @brief Create a circle polygon with a specified center, radius, and number of segments.
         * @param cx X-coordinate of the center.
         * @param cy Y-coordinate of the center.
         * @param r Radius of the circle.
         * @param segments Number of segments used to approximate the circle.
         * @return Circle polygon.
         */
        static Polygon NewCircle(float cx, float cy, float r, int segments = 36);

        /**
         * @brief Create a donut-shaped polygon with specified parameters.
         * @param cx X-coordinate of the center.
         * @param cy Y-coordinate of the center.
         * @param outerRx Outer radius along the x-axis.
         * @param outerRy Outer radius along the y-axis.
         * @param innerRx Inner radius along the x-axis.
         * @param innerRy Inner radius along the y-axis.
         * @param outerSeg Number of segments used to approximate the outer circle.
         * @param innerSeg Number of segments used to approximate the inner circle.
         * @return Donut-shaped polygon.
         */
        static Polygon NewDonut(float cx, float cy, float outerRx, float outerRy, float innerRx, float innerRy, int outerSeg, int innerSeg);

        /**
         * @brief Create a random polygon with specified parameters.
         * @param cx X-coordinate of the center.
         * @param cy Y-coordinate of the center.
         * @param maxRadius Maximum radius of the vertices from the center.
         * @param numSides Number of sides of the polygon.
         * @param seed Seed value for random number generation (optional).
         * @return Random polygon.
         */
        static Polygon NewRandom(float cx, float cy, float maxRadius, int numSides, Uint64 seed = 0);

        /**
         * @brief Get the number of vertices in the polygon.
         * @return The number of vertices in the polygon.
         */
        std::size_t size() const { return vertices.size(); }

        /**
         * @brief Get an iterator to the beginning of the vertices.
         * @return An iterator to the beginning of the vertices.
         */
        auto begin() { return vertices.begin(); }

        /**
         * @brief Get an iterator to the end of the vertices.
         * @return An iterator to the end of the vertices.
         */
        auto end() { return vertices.end(); }

        /**
         * @brief Translate the polygon by a displacement vector.
         * @param displacement The displacement vector to translate the polygon by.
         */
        void Translate(const math::Vec2& displacement);

        /**
         * @brief Set the position of the polygon by applying a translation from the current position to a new position.
         * @param position The new position of the polygon.
         * @param from The current position of the polygon (optional).
         */
        void SetPosition(const math::Vec2& position, const math::Vec2& from);

        /**
         * @brief Rotate the polygon by a specified angle around a given origin point.
         * @param angle The angle of rotation in degrees.
         * @param origin The origin point of rotation.
         */
        void Rotate(float angle, const math::Vec2& origin);

        /**
         * @brief Scale the polygon by a scaling factor along both axes.
         * @param scale The scaling factor along both axes.
         */
        void Scale(const math::Vec2& scale);

        /**
         * @brief Reverse the order of vertices in the polygon.
         * This function reverses the order of vertices in the polygon, effectively reversing the direction of its edges.
         */
        void Revert();

        /**
         * @brief Get the signed area of the polygon.
         * The signed area of a polygon is a measure of the area it encloses, taking into account the orientation (clockwise or counterclockwise) of its vertices.
         * @return The signed area of the polygon.
         */
        float GetSignedArea() const;

        /**
         * @brief Get the unsigned area of the polygon.
         * The unsigned area of a polygon is the absolute value of its signed area, representing the magnitude of the area it encloses.
         * @return The unsigned area of the polygon.
         */
        float GetArea() const;

        /**
         * @brief Get the perimeter (length) of the polygon.
         * @return The perimeter (length) of the polygon.
         */
        float GetPerimeter() const;

        /**
         * @brief Get the center of the bounding box of the polygon.
         * @return The center of the bounding box of the polygon.
         */
        math::Vec2 GetCenter() const;

        /**
         * @brief Get the centroid (geometric center) of the polygon.
         * The centroid of a polygon is the arithmetic mean position of all the points in the shape.
         * @return The centroid of the polygon.
         */
        math::Vec2 GetCentroid() const;

        /**
         * @brief Get the closest vertex to a specified point in the polygon.
         * Optionally returns the index of the closest vertex.
         * @param point The point to which the closest vertex is sought.
         * @param index Pointer to store the index of the closest vertex (optional).
         * @return The closest vertex to the specified point.
         */
        math::Vec2 GetClosestVertice(const math::Vec2& point, int* index = nullptr) const;

        /**
         * @brief Get the vertex at the specified index (const version).
         * @param index Index of the vertex to retrieve.
         * @return The vertex at the specified index.
         */
        const math::Vec2& GetVertice(Uint32 index) const;

        /**
         * @brief Get the vertex at the specified index (mutable version).
         * @param index Index of the vertex to retrieve.
         * @return Reference to the vertex at the specified index.
         */
        math::Vec2& GetVertice(Uint32 index);

        /**
         * @brief Get the edge of the polygon at the specified index.
         * @param index Index of the edge to retrieve.
         * @return The edge of the polygon at the specified index.
         */
        Line GetEdge(Uint32 index) const;

        /**
         * @brief Get the Axis-Aligned Bounding Box (AABB) of the polygon.
         * @return The AABB representing the bounding box of the polygon.
         */
        AABB GetAABB() const;

        /**
         * @brief Check collision between the polygon and an AABB.
         * @param aabb The AABB to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionAABB(const AABB& aabb) const override;

        /**
         * @brief Check collision between the polygon and a point.
         * @param point The point to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec2& point) const override;

        /**
         * @brief Check collision between the polygon and a line.
         * @param line The line to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionLine(const Line& line) const override;

        /**
         * @brief Check collision between the polygon and a mesh.
         * @param mesh The mesh to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionMesh(const Mesh& mesh) const override;

        /**
         * @brief Check collision between the polygon and a circle.
         * @param circle The circle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionCircle(const Circle& circle) const override;

        /**
         * @brief Check collision between the polygon and an ellipse.
         * @param ellipse The ellipse to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionEllipse(const Ellipse& ellipse) const override;

        /**
         * @brief Check collision between the polygon and another polygon.
         * @param poly The polygon to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPolygon(const Polygon& poly) const override;

        /**
         * @brief Check collision between the polygon and a triangle.
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const Triangle& tri) const override;

        /**
         * @brief Check collision between the polygon and a triangle (with float coordinates).
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const TriangleF& tri) const override;

        /**
         * @brief Check collision between the polygon and a rectangle.
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const Rectangle& rect) const override;

        /**
         * @brief Check collision between the polygon and a rectangle (with float coordinates).
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const RectangleF& rect) const override;
    };

}}

#endif // NEXUS_SHAPE_2D_POLYGON_HPP
