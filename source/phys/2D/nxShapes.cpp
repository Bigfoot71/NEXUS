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

#include "phys/2D/nxShapes.hpp"

using namespace nexus;

shape2D::Circle phys2D::GetCircle(const phys2D::Shape* shape)
{
    if (shape->GetType() != phys2D::Shape::e_circle)
    {
        throw core::NexusException("phys2D::GetCircle", "The shape given as a parameter is not a circle.");
    }

    auto circleShape = static_cast<const CircleShape*>(shape);
    return shape2D::Circle(circleShape->m_p, circleShape->m_radius);
}

shape2D::Polygon phys2D::GetChain(const phys2D::Shape* shape)
{
    if (shape->GetType() != phys2D::Shape::e_chain)
    {
        throw core::NexusException("phys2D::GetChain", "The shape given as a parameter is not a chain.");
    }

    const auto polygonShape = static_cast<const phys2D::PolygonShape*>(shape);
    const int vertexCount = polygonShape->m_count;

    shape2D::Polygon poly;
    poly.vertices.reserve(vertexCount);

    for (int i = 0; i < vertexCount; i++)
    {
        poly.vertices.push_back(polygonShape->m_vertices[i]);
    }

    return poly;
}

shape2D::Line phys2D::GetEdge(const phys2D::Shape* shape)
{
    if (shape->GetType() == phys2D::Shape::e_edge)
    {
        throw core::NexusException("phys2D::GetEdge", "The shape given as a parameter is not a edge.");
    }

    const auto edgeShape = static_cast<const phys2D::EdgeShape*>(shape);
    return shape2D::Line(edgeShape->m_vertex1, edgeShape->m_vertex2);
}

shape2D::Polygon phys2D::GetPolygon(const phys2D::Shape* shape)
{
    if (shape->GetType() != phys2D::Shape::e_polygon)
    {
        throw core::NexusException("phys2D::GetPolygon", "The shape given as a parameter is not a polygon.");
    }

    const auto polygonShape = static_cast<const phys2D::PolygonShape*>(shape);
    const int vertexCount = polygonShape->m_count;

    shape2D::Polygon poly;
    poly.vertices.reserve(vertexCount);

    for (int i = 0; i < vertexCount; i++)
    {
        poly.vertices.push_back(polygonShape->m_vertices[i]);
    }

    return poly;
}
