#include "shape/2D/nxRectangle.hpp"

#include "shape/2D/nxAABB.hpp"
#include "shape/2D/nxLine.hpp"
#include "shape/2D/nxMesh.hpp"
#include "shape/2D/nxCircle.hpp"
#include "shape/2D/nxEllipse.hpp"
#include "shape/2D/nxPolygon.hpp"
#include "shape/2D/nxTriangle.hpp"

using namespace nexus;

shape2D::AABB shape2D::RectangleF::GetAABB() const
{
    const math::Vec2 v1(x, y), v2(x + w, y + h);
    return AABB(v1.Min(v2), v1.Max(v2));
}

bool shape2D::RectangleF::CollisionAABB(const AABB& aabb) const
{
    return aabb.CollisionRectangle(*this);
}

bool shape2D::RectangleF::CollisionPoint(const math::Vec2& point) const
{
    return point.x >= x && point.x <= x+w && point.y >= y && point.y <= y+h;
}

bool shape2D::RectangleF::CollisionLine(const Line& line) const
{
    return CollisionPoint(line.start) || CollisionPoint(line.end)
        || Line(x, y, x + w, y).CollisionLine(line)
        || Line(x + w, y, x + w, y + h).CollisionLine(line)
        || Line(x + w, y + h, x, y + h).CollisionLine(line)
        || Line(x, y + h, x, y).CollisionLine(line);
}

bool shape2D::RectangleF::CollisionMesh(const Mesh& mesh) const
{
    return mesh.CollisionRectangle(*this);
}

bool shape2D::RectangleF::CollisionCircle(const Circle& circle) const
{
    const float adx = std::abs(circle.center.x - (x + w * 0.5f));
    const float ady = std::abs(circle.center.y - (y + h * 0.5f));

    const float hw = w * 0.5f, hh = h * 0.5f;

    if (adx > hw + circle.radius) return false;
    if (ady > hh + circle.radius) return false;

    if (adx <= hw) return true;
    if (ady <= hh) return true;

    const float cornerDistSqr = std::pow(adx - hw, 2) + std::pow(ady - hh, 2);

    return cornerDistSqr <= circle.radius * circle.radius;
}

bool shape2D::RectangleF::CollisionEllipse(const Ellipse& ellipse) const
{
    return ellipse.CollisionRectangle(*this);
}

bool shape2D::RectangleF::CollisionPolygon(const Polygon& poly) const
{
    return poly.CollisionRectangle(*this);
}

bool shape2D::RectangleF::CollisionTriangle(const Triangle& tri) const
{
    return tri.CollisionRectangle(*this);
}

bool shape2D::RectangleF::CollisionTriangle(const TriangleF& tri) const
{
    return tri.CollisionRectangle(*this);
}

bool shape2D::RectangleF::CollisionRectangle(const Rectangle& rect) const
{
    return (x + w >= rect.x) && (rect.x + rect.w >= x) && (y + h >= rect.y) && (rect.y + rect.h >= y);
}

bool shape2D::RectangleF::CollisionRectangle(const RectangleF& rect) const
{
    return (x + w >= rect.x) && (rect.x + rect.w >= x) && (y + h >= rect.y) && (rect.y + rect.h >= y);
}
