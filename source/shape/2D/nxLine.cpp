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

#include "shape/2D/nxLine.hpp"

#include "shape/2D/nxAABB.hpp"
#include "shape/2D/nxMesh.hpp"
#include "shape/2D/nxCircle.hpp"
#include "shape/2D/nxEllipse.hpp"
#include "shape/2D/nxPolygon.hpp"
#include "shape/2D/nxTriangle.hpp"
#include "shape/2D/nxRectangle.hpp"

using namespace nexus;

shape2D::AABB shape2D::Line::GetAABB() const
{
    return AABB(start.Min(end), start.Max(end));
}

bool shape2D::Line::CollisionAABB(const AABB& aabb) const
{
    return aabb.CollisionLine(*this);
}

bool shape2D::Line::CollisionPoint(const math::Vec2& point) const
{
    constexpr float lineEpsilon = 0.1f; // Added margin of error

    const float lineLen = start.Distance(end);
    const float d1 = point.Distance(start);
    const float d2 = point.Distance(end);

    return (d1 + d2 >= lineLen - lineEpsilon && d1 + d2 <= lineLen + lineEpsilon);
}

bool shape2D::Line::CollisionLine(const Line& line) const
{
    const float x1 = start.x, x2 = end.x, x3 = line.start.x, x4 = line.end.x;
    const float y1 = start.y, y2 = end.y, y3 = line.start.y, y4 = line.end.y;

    const float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (denom == 0) return false;

    const float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
    const float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;

    return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1);
}

bool shape2D::Line::CollisionMesh(const Mesh& mesh) const
{
    return mesh.CollisionLine(*this);
}

bool shape2D::Line::CollisionCircle(const Circle& circle) const
{
    return circle.CollisionLine(*this);
}

bool shape2D::Line::CollisionEllipse(const Ellipse& ellipse) const
{
    return ellipse.CollisionLine(*this);
}

bool shape2D::Line::CollisionPolygon(const Polygon& poly) const
{
    return poly.CollisionLine(*this);
}

bool shape2D::Line::CollisionTriangle(const Triangle& tri) const
{
    return tri.CollisionLine(*this);
}

bool shape2D::Line::CollisionTriangle(const TriangleF& tri) const
{
    return tri.CollisionLine(*this);
}

bool shape2D::Line::CollisionRectangle(const Rectangle& rect) const
{
    return rect.CollisionLine(*this);
}

bool shape2D::Line::CollisionRectangle(const RectangleF& rect) const
{
    return rect.CollisionLine(*this);
}
