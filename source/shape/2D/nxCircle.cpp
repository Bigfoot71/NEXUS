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

#include "shape/2D/nxCircle.hpp"

#include "shape/2D/nxAABB.hpp"
#include "shape/2D/nxLine.hpp"
#include "shape/2D/nxMesh.hpp"
#include "shape/2D/nxEllipse.hpp"
#include "shape/2D/nxPolygon.hpp"
#include "shape/2D/nxTriangle.hpp"
#include "shape/2D/nxRectangle.hpp"

using namespace nexus;

shape2D::AABB shape2D::Circle::GetAABB() const
{
    return AABB(
        center.x - radius, center.y - radius,
        center.x + radius, center.y + radius);
}

bool shape2D::Circle::CollisionAABB(const AABB& aabb) const
{
    return aabb.CollisionCircle(*this);
}

bool shape2D::Circle::CollisionPoint(const math::Vec2& point) const
{
    return point.Distance(center) <= radius;
}

bool shape2D::Circle::CollisionLine(const Line& line) const
{
    const float dot = ((center.x - line.start.x) * (line.end.x - line.start.x) + (center.y - line.start.y) * (line.end.y - line.start.y)) / std::pow(line.start.Distance(line.end), 2);
    math::Vec2 closestPoint(line.start.x + dot * (line.end.x - line.start.x), line.start.y + dot * (line.end.y - line.start.y));
    if (dot < 0 || dot > 1) closestPoint = (center.Distance(line.start) < center.Distance(line.end)) ? line.start : line.end;
    return closestPoint.Distance(center) <= radius;
}

bool shape2D::Circle::CollisionMesh(const Mesh& mesh) const
{
    return mesh.CollisionCircle(*this);
}

bool shape2D::Circle::CollisionCircle(const Circle& circle) const
{
    return center.Distance(circle.center) < radius + circle.radius;
}

bool shape2D::Circle::CollisionEllipse(const Ellipse& ellipse) const
{
    return ellipse.CollisionCircle(*this);
}

bool shape2D::Circle::CollisionPolygon(const Polygon& poly) const
{
    return poly.CollisionCircle(*this);
}

bool shape2D::Circle::CollisionTriangle(const Triangle& tri) const
{
    return tri.CollisionCircle(*this);
}

bool shape2D::Circle::CollisionTriangle(const TriangleF& tri) const
{
    return tri.CollisionCircle(*this);
}

bool shape2D::Circle::CollisionRectangle(const Rectangle& rect) const
{
    return rect.CollisionCircle(*this);
}

bool shape2D::Circle::CollisionRectangle(const RectangleF& rect) const
{
    return rect.CollisionCircle(*this);
}
