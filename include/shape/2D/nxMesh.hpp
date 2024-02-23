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

#ifndef NEXUS_SHAPE_2D_MESH_HPP
#define NEXUS_SHAPE_2D_MESH_HPP

#include "./nxCollisionShape.hpp"
#include "../../gfx/nxColor.hpp"
#include "../../math/nxVec2.hpp"
#include <SDL_render.h>
#include <vector>

namespace nexus { namespace shape2D {

    /**
     * @brief Represents a 2D vertex, primarily intended for use with gfx::Renderer::DrawGeometry.
     * @note: The order of declaration of members is important because it corresponds to that of SDL_Vertex.
     */
    struct NEXUS_API Vertex
    {
        math::Vec2 position{};  ///< The position of the vertex.
        gfx::Color color{};     ///< The color of the vertex.
        math::Vec2 texcoord{};  ///< The texture coordinates of the vertex.

        Vertex() = default; ///< Default constructor, fill everithing with 0

        /**
         * @brief Constructs a Vertex object with specified position, color, and texture coordinates.
         * @param position The position of the vertex.
         * @param color The color of the vertex.
         * @param texcoord The texture coordinates of the vertex.
         */
        constexpr Vertex(const math::Vec2 position, const gfx::Color color = gfx::White, const math::Vec2 texcoord = {})
        : position(position), color(color), texcoord(texcoord)
        { }

        /**
         * @brief Constructs a Vertex object with specified coordinates, color, and texture coordinates.
         * @param x The x-coordinate of the vertex.
         * @param y The y-coordinate of the vertex.
         * @param r The red component of the vertex color.
         * @param g The green component of the vertex color.
         * @param b The blue component of the vertex color.
         * @param a The alpha component of the vertex color.
         * @param u The u texture coordinate of the vertex.
         * @param v The v texture coordinate of the vertex.
         */
        constexpr Vertex(float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, float u = 0, float v = 0)
        : position(x, y), color(r, g, b, a), texcoord(u, v)
        { }

        /**
         * @brief Constructs a Vertex object from an SDL_Vertex object.
         * @param vertex The SDL_Vertex object to construct from.
         */
        constexpr Vertex(const ::SDL_Vertex& vertex)
        : position(vertex.position)
        , color(vertex.color)
        , texcoord(vertex.tex_coord)
        { }

        /**
         * @brief Conversion operator to convert Vertex to mutable SDL_Vertex reference.
         * @return A mutable reference to the SDL_Vertex representation of this Vertex.
         */
        operator ::SDL_Vertex&()
        {
            return *reinterpret_cast<SDL_Vertex*>(this);
        }

        /**
         * @brief Conversion operator to convert Vertex to constant SDL_Vertex reference.
         * @return A constant reference to the SDL_Vertex representation of this Vertex.
         */
        operator const ::SDL_Vertex&() const
        {
            return *reinterpret_cast<const SDL_Vertex*>(this);
        }
    };

    /**
     * @brief Represents a mesh composed of triangles.
     *
     * This structure represents a mesh consisting of vertices and indices forming triangles.
     */
    struct NEXUS_API Mesh : CollisionShape
    {
        std::vector<Vertex> vertices;   ///< The vertices of the mesh.
        std::vector<int> indices;       ///< The indices defining the triangles of the mesh.

        /**
         * @brief Default constructor for Mesh.
         */
        Mesh() = default;

        /**
         * @brief Constructs a mesh with a specified number of vertices.
         *
         * @param numVertices The number of vertices to initialize the mesh with.
         * @param vertex The default vertex to use for initialization.
         */
        Mesh(Uint32 numVertices, const Vertex& vertex = { 0, 0, 0, 0, 0, 0, 0, 0 })
        : vertices(numVertices, vertex)
        { }

        /**
         * @brief Constructs a mesh from a vector of vertices.
         *
         * @param vertices The vector of vertices to initialize the mesh with.
         */
        Mesh(const std::vector<Vertex>& vertices)
        : vertices(vertices)
        { }

        /**
         * @brief Get the Axis-Aligned Bounding Box (AABB) of the mesh.
         * @return The AABB representing the bounding box of the mesh.
         */
        shape2D::AABB GetAABB() const;

        /**
         * @brief Check collision between the mesh and an AABB.
         * @param aabb The AABB to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionAABB(const AABB& aabb) const override;

        /**
         * @brief Check collision between the mesh and a point.
         * @param point The point to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPoint(const nexus::math::Vec2& point) const override;

        /**
         * @brief Check collision between the mesh and a line.
         * @param line The line to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionLine(const Line& line) const override;

        /**
         * @brief Check collision between the mesh and another mesh.
         * @param mesh The mesh to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionMesh(const Mesh& mesh) const override;

        /**
         * @brief Check collision between the mesh and a circle.
         * @param circle The circle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionCircle(const Circle& circle) const override;

        /**
         * @brief Check collision between the mesh and an ellipse.
         * @param ellipse The ellipse to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionEllipse(const Ellipse& ellipse) const override;

        /**
         * @brief Check collision between the mesh and a polygon.
         * @param poly The polygon to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionPolygon(const Polygon& poly) const override;

        /**
         * @brief Check collision between the mesh and a triangle.
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const Triangle& tri) const override;

        /**
         * @brief Check collision between the mesh and a triangle (with float coordinates).
         * @param tri The triangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionTriangle(const TriangleF& tri) const override;

        /**
         * @brief Check collision between the mesh and a rectangle.
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const Rectangle& rect) const override;

        /**
         * @brief Check collision between the mesh and a rectangle (with float coordinates).
         * @param rect The rectangle to check collision with.
         * @return True if collision occurs, false otherwise.
         */
        bool CollisionRectangle(const RectangleF& rect) const override;
    };

}}

#endif //NEXUS_SHAPE_2D_MESH_HPP
