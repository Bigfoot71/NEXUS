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

#include "shape/2D/nxMesh.hpp"

#include "shape/2D/nxAABB.hpp"
#include "shape/2D/nxTriangle.hpp"

using namespace nexus;

shape2D::AABB shape2D::Mesh::GetAABB() const
{
    math::Vec2 minVertex(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    math::Vec2 maxVertex(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

    for (const auto &vertex : vertices)
    {
        minVertex = minVertex.Min(vertex.position);
        maxVertex = maxVertex.Max(vertex.position);
    }

    return shape2D::AABB(minVertex, maxVertex);
}

bool shape2D::Mesh::CollisionAABB(const AABB& aabb) const
{
    for (int i = 0; i < vertices.size(); i += 3)
    {
        if (Triangle(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position).CollisionAABB(aabb))
        {
            return true;
        }
    }

    return false;
}

bool shape2D::Mesh::CollisionPoint(const nexus::math::Vec2& point) const
{
    for (int i = 0; i < vertices.size(); i += 3)
    {
        if (Triangle(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position).CollisionPoint(point))
        {
            return true;
        }
    }

    return false;
}

bool shape2D::Mesh::CollisionLine(const Line& line) const
{
    for (int i = 0; i < vertices.size(); i += 3)
    {
        if (Triangle(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position).CollisionLine(line))
        {
            return true;
        }
    }

    return false;
}

bool shape2D::Mesh::CollisionMesh(const Mesh& mesh) const
{
    for (int i = 0; i < vertices.size(); i += 3)
    {
        if (Triangle(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position).CollisionMesh(mesh))
        {
            return true;
        }
    }

    return false;
}

bool shape2D::Mesh::CollisionCircle(const Circle& circle) const
{
    for (int i = 0; i < vertices.size(); i += 3)
    {
        if (Triangle(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position).CollisionCircle(circle))
        {
            return true;
        }
    }

    return false;
}

bool shape2D::Mesh::CollisionEllipse(const Ellipse& ellipse) const
{
    for (int i = 0; i < vertices.size(); i += 3)
    {
        if (Triangle(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position).CollisionEllipse(ellipse))
        {
            return true;
        }
    }

    return false;
}

bool shape2D::Mesh::CollisionPolygon(const Polygon& poly) const
{
    for (int i = 0; i < vertices.size(); i += 3)
    {
        if (Triangle(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position).CollisionPolygon(poly))
        {
            return true;
        }
    }

    return false;
}

bool shape2D::Mesh::CollisionTriangle(const Triangle& tri) const
{
    return tri.CollisionMesh(*this);
}

bool shape2D::Mesh::CollisionTriangle(const TriangleF& tri) const
{
    return tri.CollisionMesh(*this);
}

bool shape2D::Mesh::CollisionRectangle(const Rectangle& rect) const
{
    for (int i = 0; i < vertices.size(); i += 3)
    {
        if (Triangle(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position).CollisionRectangle(rect))
        {
            return true;
        }
    }

    return false;
}

bool shape2D::Mesh::CollisionRectangle(const RectangleF& rect) const
{
    for (int i = 0; i < vertices.size(); i += 3)
    {
        if (Triangle(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position).CollisionRectangle(rect))
        {
            return true;
        }
    }

    return false;
}
