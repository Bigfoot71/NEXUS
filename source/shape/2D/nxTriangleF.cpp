#include "shape/2D/nxTriangle.hpp"

#include "shape/2D/nxRectangle.hpp"
#include "shape/2D/nxPolygon.hpp"
#include "shape/2D/nxEllipse.hpp"
#include "shape/2D/nxCircle.hpp"
#include "shape/2D/nxAABB.hpp"
#include "shape/2D/nxLine.hpp"
#include "shape/2D/nxMesh.hpp"

using namespace nexus;

math::Vec3 shape2D::TriangleF::BarycentricCoordinates(const math::Vec2& p) const
{
    // Calculate the side vectors of the triangle
    const math::Vec2 ab = b - a;
    const math::Vec2 ac = c - a;
    const math::Vec2 ap = p - a;

    // Calculate dot products and cross products
    const float d00 = ab.Dot(ab);
    const float d01 = ab.Dot(ac);
    const float d02 = ab.Dot(ap);
    const float d11 = ac.Dot(ac);
    const float d12 = ac.Dot(ap);

    // Calculate the inverse of the denominator
    const float invDenom = 1.0f / (d00 * d11 - d01 * d01);

    // Calculate barycentric coordinates
    const float u = (d11 * d02 - d01 * d12) * invDenom;
    const float v = (d00 * d12 - d01 * d02) * invDenom;
    const float w = 1.0f - u - v;

    return math::Vec3(u, v, w);
}

shape2D::AABB shape2D::TriangleF::GetAABB() const
{
    return shape2D::AABB {
        a.Min(b.Min(c)),
        a.Max(b.Max(c))
    };
}

bool shape2D::TriangleF::CollisionAABB(const AABB& aabb) const
{
    const auto& min = aabb.min;
    const auto& max = aabb.max;

    if (a.x < min.x && b.x < min.x && c.x < min.x) return false;
    if (a.y < min.y && b.y < min.y && c.y < min.y) return false;
    if (a.x > max.x && b.x > max.x && c.x > max.x) return false;
    if (a.y > max.y && b.y > max.y && c.y > max.y) return false;

    if (a.x >= min.x && a.x <= max.x && a.y >= min.y && a.y <= max.y) return true;
    if (b.x >= min.x && b.x <= max.x && b.y >= min.y && b.y <= max.y) return true;
    if (c.x >= min.x && c.x <= max.x && c.y >= min.y && c.y <= max.y) return true;

    if (Line(a, b).CollisionAABB(aabb)) return true;
    if (Line(b, c).CollisionAABB(aabb)) return true;
    if (Line(c, a).CollisionAABB(aabb)) return true;

    return false;
}

bool shape2D::TriangleF::CollisionPoint(const nexus::math::Vec2& point) const
{
    // Calculate the area of the triangle
    float a0 = std::abs((b.x-a.x) * (c.y-a.y) - (c.x-a.x) * (b.y-a.y));

    // Calculate the area of 3 triangles made between the point and the corners of the triangle
    float a1 = std::abs((a.x-point.x) * (b.y-point.y) - (b.x-point.x) * (a.y-point.y));
    float a2 = std::abs((b.x-point.x) * (c.y-point.y) - (c.x-point.x) * (b.y-point.y));
    float a3 = std::abs((c.x-point.x) * (a.y-point.y) - (a.x-point.x) * (c.y-point.y));

    // If the sum of the three areas equals the original, we're inside the triangle!
    return (a1 + a2 + a3 == a0);
}

bool shape2D::TriangleF::CollisionLine(const Line& line) const
{
    const auto& p = line.start;
    const auto& q = line.end;

    // Calculate the line vectors
    nexus::math::Vec2 dir = q - p;
    nexus::math::Vec2 e1 = b - a;
    nexus::math::Vec2 e2 = c - a;

    // Calculate the cross product
    float PxQy = dir.x * e2.y - dir.y * e2.x;

    // If the cross product is close to zero, the line is parallel to the triangle
    if (std::abs(PxQy) < 1e-5f) return false;

    float invPxQy = 1.0f / PxQy;

    // Calculate the t vector
    nexus::math::Vec2 t = p - a;

    // Calculate the first Möller-Trumbore coefficient
    float u = (t.x * e2.y - t.y * e2.x) * invPxQy;

    // Check if u is in the interval [0, 1]
    if (u < 0.0f || u > 1.0f) return false;

    // Calculate the second Möller-Trumbore coefficient
    float v = (dir.x * t.y - dir.y * t.x) * invPxQy;

    // Check if v is in the interval [0, 1] and if u + v is less than or equal to 1
    if (v < 0.0f || u + v > 1.0f) return false;

    // The line intersects the triangle
    return true;
}

bool shape2D::TriangleF::CollisionMesh(const Mesh& mesh) const
{
    for (int i = 0; i < mesh.vertices.size(); i += 3)
    {
        if (CollisionTriangle(TriangleF{ mesh.vertices[i].position, mesh.vertices[i + 1].position, mesh.vertices[i + 2].position }))
        {
            return true;
        }
    }

    return false;
}

bool shape2D::TriangleF::CollisionCircle(const Circle& circle) const
{
    if (CollisionPoint(circle.center)) return true;
    if (circle.CollisionLine({ a, b })) return true;
    if (circle.CollisionLine({ b, c })) return true;
    if (circle.CollisionLine({ c, a })) return true;
    return false;
}

bool shape2D::TriangleF::CollisionEllipse(const Ellipse& ellipse) const
{
    if (CollisionPoint(ellipse.center)) return true;
    if (ellipse.CollisionLine({ a, b })) return true;
    if (ellipse.CollisionLine({ b, c })) return true;
    if (ellipse.CollisionLine({ c, a })) return true;
    return false;
}

bool shape2D::TriangleF::CollisionPolygon(const Polygon& poly) const
{
    const float a0 = std::abs((b.x-a.x) * (c.y-a.y) - (c.x-a.x) * (b.y-a.y));

    for (auto point : poly.vertices)
    {
        const float a1 = std::abs((a.x-point.x) * (b.y-point.y) - (b.x-point.x) * (a.y-point.y));
        const float a2 = std::abs((b.x-point.x) * (c.y-point.y) - (c.x-point.x) * (b.y-point.y));
        const float a3 = std::abs((c.x-point.x) * (a.y-point.y) - (a.x-point.x) * (c.y-point.y));
        if (a1 + a2 + a3 == a0) return true;
    }

    if (poly.CollisionPoint(a)) return true;
    if (poly.CollisionPoint(b)) return true;
    if (poly.CollisionPoint(c)) return true;

    return false;
}

bool shape2D::TriangleF::CollisionTriangle(const Triangle& tri) const
{
    return this->CollisionTriangle(static_cast<TriangleF>(tri));
}

bool shape2D::TriangleF::CollisionTriangle(const TriangleF& tri) const
{
    const float a0 = std::abs((b.x-a.x) * (c.y-a.y) - (c.x-a.x) * (b.y-a.y));
    const float a00 = std::abs((tri.b.x-tri.a.x) * (tri.c.y-tri.a.y) - (tri.c.x-tri.a.x) * (tri.b.y-tri.a.y));

    for (Uint8 i = 0; i < 3; i++)
    {
        float a1 = std::abs((a.x-tri[i].x) * (b.y-tri[i].y) - (b.x-tri[i].x) * (a.y-tri[i].y));
        float a2 = std::abs((b.x-tri[i].x) * (c.y-tri[i].y) - (c.x-tri[i].x) * (b.y-tri[i].y));
        float a3 = std::abs((c.x-tri[i].x) * (a.y-tri[i].y) - (a.x-tri[i].x) * (c.y-tri[i].y));
        if (a1 + a2 + a3 == a0) return true;

        a1 = std::abs((tri.a.x-(*this)[i].x) * (tri.b.y-(*this)[i].y) - (tri.b.x-(*this)[i].x) * (tri.a.y-(*this)[i].y));
        a2 = std::abs((tri.b.x-(*this)[i].x) * (tri.c.y-(*this)[i].y) - (tri.c.x-(*this)[i].x) * (tri.b.y-(*this)[i].y));
        a3 = std::abs((tri.c.x-(*this)[i].x) * (tri.a.y-(*this)[i].y) - (tri.a.x-(*this)[i].x) * (tri.c.y-(*this)[i].y));
        if (a1 + a2 + a3 == a00) return true;

        const Line l1 = { (*this)[i], (*this)[(i + 1) % 3] };

        for (Uint8 j = 0; j < 3; i++)
        {
            if (l1.CollisionLine({ tri[i], tri[(i + 1) % 3] }))
            {
                return true;
            }
        }
    }

    return false;
}

bool shape2D::TriangleF::CollisionRectangle(const Rectangle& rect) const
{
    return CollisionAABB(rect.GetAABB());
}

bool shape2D::TriangleF::CollisionRectangle(const RectangleF& rect) const
{
    return CollisionAABB(rect.GetAABB());
}
