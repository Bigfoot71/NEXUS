#include "shape/2D/nxAABB.hpp"

#include "shape/2D/nxLine.hpp"
#include "shape/2D/nxMesh.hpp"
#include "shape/2D/nxCircle.hpp"
#include "shape/2D/nxEllipse.hpp"
#include "shape/2D/nxPolygon.hpp"
#include "shape/2D/nxTriangle.hpp"
#include "shape/2D/nxRectangle.hpp"

using namespace nexus;

shape2D::RectangleF shape2D::AABB::GetRectangle() const
{
    return { min.x, min.y, max.x - min.x, max.y - min.y };
}

bool shape2D::AABB::CollisionAABB(const AABB& aabb) const
{
    bool overlapX = max.x >= aabb.min.x && min.x <= aabb.max.x;
    bool overlapY = max.y >= aabb.min.y && min.y <= aabb.max.y;
    return overlapX && overlapY;
}

bool shape2D::AABB::CollisionPoint(const math::Vec2& point) const
{
    return (point.x >= min.x && point.x <= max.x &&
            point.y >= min.y && point.y <= max.y);
}

bool shape2D::AABB::CollisionLine(const Line& line) const
{
    if (line.CollisionLine({ min, math::Vec2(max.x, min.y) })) return true;
    if (line.CollisionLine({ math::Vec2(min.x, max.y), max })) return true;
    if (line.CollisionLine({ min, math::Vec2(min.x, max.y) })) return true;
    if (line.CollisionLine({ math::Vec2(max.x, min.y), max })) return true;

    return false;
}

bool shape2D::AABB::CollisionMesh(const Mesh& mesh) const
{
    return mesh.CollisionAABB(*this);
}

bool shape2D::AABB::CollisionCircle(const Circle& circle) const
{
    // Calculer le point le plus proche du centre du cercle à l'intérieur de l'AABB
    float closestX = std::clamp(circle.center.x, min.x, max.x);
    float closestY = std::clamp(circle.center.y, min.y, max.y);

    // Calculer la distance entre le point le plus proche et le centre du cercle
    float distanceX = circle.center.x - closestX;
    float distanceY = circle.center.y - closestY;
    float distanceSquared = distanceX * distanceX + distanceY * distanceY;

    // Collision si la distance au carré est inférieure ou égale au carré du rayon du cercle
    return distanceSquared <= (circle.radius * circle.radius);
}

bool shape2D::AABB::CollisionEllipse(const Ellipse& ellipse) const
{
    return ellipse.CollisionAABB(*this);
}

bool shape2D::AABB::CollisionPolygon(const Polygon& poly) const
{
    return poly.CollisionAABB(*this);
}

bool shape2D::AABB::CollisionTriangle(const Triangle& tri) const
{
    return tri.CollisionAABB(*this);
}

bool shape2D::AABB::CollisionTriangle(const TriangleF& tri) const
{
    return tri.CollisionAABB(*this);
}

bool shape2D::AABB::CollisionRectangle(const Rectangle& rect) const
{
    bool overlapX = max.x >= rect.x && min.x <= rect.x + rect.w;
    bool overlapY = max.y >= rect.y && min.y <= rect.y + rect.h;
    return overlapX && overlapY;
}

bool shape2D::AABB::CollisionRectangle(const RectangleF& rect) const
{
    bool overlapX = max.x >= rect.x && min.x <= rect.x + rect.w;
    bool overlapY = max.y >= rect.y && min.y <= rect.y + rect.h;
    return overlapX && overlapY;
}
