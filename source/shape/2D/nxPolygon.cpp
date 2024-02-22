#include "shape/2D/nxPolygon.hpp"

#include "shape/2D/nxAABB.hpp"
#include "shape/2D/nxLine.hpp"
#include "shape/2D/nxMesh.hpp"
#include "shape/2D/nxCircle.hpp"
#include "shape/2D/nxEllipse.hpp"
#include "shape/2D/nxTriangle.hpp"
#include "shape/2D/nxRectangle.hpp"

#include "math/nxMath.hpp"
#include "core/nxRandom.hpp"

#include <limits>

using namespace nexus;

bool shape2D::Polygon::operator==(const Polygon& other) const
{
    if (vertices.size() != other.size()) return false;

    for (std::size_t i = 0; i < vertices.size(); i++)
    {
        if (vertices[i] != other.vertices[i]) return false;
    }

    return true;
}

bool shape2D::Polygon::operator!=(const Polygon& other) const
{
    return *this != other;
}

shape2D::Polygon& shape2D::Polygon::operator=(std::initializer_list<math::Vec2> initList)
{
    vertices = initList;
    return *this;
}

shape2D::Polygon shape2D::Polygon::NewTriangleEquilateral(float cx, float cy, float sideLength)
{
    const float halfSide = sideLength * 0.5f;
    const float height = halfSide * std::pow(3, 0.5f);

    return {
        cx, cy - height / 2,
        cx + halfSide, cy + height / 2,
        cx - halfSide, cy + height / 2
    };
}

shape2D::Polygon shape2D::Polygon::NewTriangleIsosceles(float cx, float cy, float sideLength)
{
    const float height = sideLength * 0.5f;

    return {
        cx - height, cy + height,
        cx + height, cy + height,
        cx, cy - height
    };
}

shape2D::Polygon shape2D::Polygon::NewTriangleRectangle(float cx, float cy, float sideLength)
{
    const float halfSide = sideLength * 0.5f;
    const float quatSide = halfSide * 0.5f;

    return {
        (cx - halfSide) + quatSide, (cy - halfSide) - quatSide,
        (cx - halfSide) + quatSide, (cy + halfSide) - quatSide,
        (cx + halfSide) + quatSide, (cy + halfSide) - quatSide
    };
}

shape2D::Polygon shape2D::Polygon::NewRectangle(float cx, float cy, float width, float height)
{
    return {
        cx - width / 2, cy - height / 2,
        cx - width / 2, cy + height / 2,
        cx + width / 2, cy + height / 2,
        cx + width / 2, cy - height / 2
    };
}

shape2D::Polygon shape2D::Polygon::NewEllipse(float cx, float cy, float rx, float ry, int segments)
{
    Polygon poly = { cx, cy };
    const float invSeg = 1.0f / segments;

    for (int i = 0; i < segments; i++)
    {
        float angle = (i * invSeg) * math::Tau;
        float x = cx + std::cos(angle) * rx;
        float y = cy + std::sin(angle) * ry;
        poly.vertices.emplace_back(x, y);
    }

    return poly;
}

shape2D::Polygon shape2D::Polygon::NewCircle(float cx, float cy, float r, int segments)
{
    return NewEllipse(cx, cy, r, r, segments);
}

shape2D::Polygon shape2D::Polygon::NewDonut(float cx, float cy, float outerRx, float outerRy, float innerRx, float innerRy, int outerSeg, int innerSeg)
{
    Polygon poly;

    const float invOS = 1.0f / outerSeg;

    for (int i = 0; i < outerSeg; i++)
    {
        float angle = (i * invOS) * math::Tau;
        float x = cx + std::cos(angle) * outerRx;
        float y = cy + std::sin(angle) * outerRy;
        poly.vertices.emplace_back(x, y);
    }

    const float invIS = 1.0f / innerSeg;

    for (int i = 0; i < innerSeg; i++)
    {
        float angle = (i * invIS) * math::Tau;
        float x = cx + std::cos(angle) * innerRx;
        float y = cy + std::sin(angle) * innerRy;
        poly.vertices.emplace_back(x, y);
    }

    return poly;
}

shape2D::Polygon shape2D::Polygon::NewRandom(float cx, float cy, float maxRadius, int numSides, Uint64 seed)
{
    core::RandomGenerator gen(seed);
    Polygon poly;

    const float invNumSides = 1.0f / numSides;

    for (int i = 0; i < numSides; i++)
    {
        float angle = (i * invNumSides) * math::Tau;
        float radius = maxRadius * gen.Random(0.0f, 0.1f);
        float x = cx + radius * std::cos(angle);
        float y = cy + radius * std::sin(angle);
        poly.vertices.emplace_back(x, y);
    }

    return poly;
}

void shape2D::Polygon::Translate(const math::Vec2& displacment)
{
    for (auto& vertex : vertices)
    {
        vertex += displacment;
    }
}

void shape2D::Polygon::SetPosition(const math::Vec2& position, const math::Vec2& from)
{
    this->Translate(position - from);
}

void shape2D::Polygon::Rotate(float angle, const math::Vec2& origin)
{
    angle *= math::Deg2Rad;

    const float c = std::cos(angle);
    const float s = std::sin(angle);

    for (auto& v : vertices)
    {
        v = (v - origin).Rotated(c, s) + origin;
    }
}

void shape2D::Polygon::Scale(const math::Vec2& scale)
{
    const math::Vec2 center = this->GetCenter();

    for (auto& v : vertices)
    {
        v = center + v * scale;
    }
}

void shape2D::Polygon::Revert()
{
    const int n = vertices.size();
    for (int i = 0; i < n / 2; i++)
    {
        std::swap(vertices[i], vertices[n - i - 1]);
    }
}

float shape2D::Polygon::GetSignedArea() const
{
    float sum = 0;
    math::Vec2 v0 = vertices.back();
    for (auto& v1 : vertices)
    {
        sum = sum + (v0.x + v1.x) * (v0.y - v1.y);
        v0 = v1;
    }
    return sum;
}

float shape2D::Polygon::GetArea() const
{
    float sum = this->GetSignedArea();
    return std::abs(sum * 0.5f);
}

float shape2D::Polygon::GetPerimeter() const
{
    float sum = 0;
    for (int i = 0; i < vertices.size(); i++)
    {
        sum += vertices[i].Distance(vertices[(i + 1) % vertices.size()]);
    }
    return sum;
}

math::Vec2 shape2D::Polygon::GetCenter() const
{
    math::Vec2 sum(0, 0);
    for (const auto& v : vertices) sum += v;
    return sum / vertices.size();
}

math::Vec2 shape2D::Polygon::GetCentroid() const
{
    math::Vec2 centeroid;
    float area = 0;

    for (int i = 0; i < vertices.size(); i++)
    {
        const int j = (i + 2 - 1) % vertices.size();

        const math::Vec2& v0 = vertices[i];
        const math::Vec2& v1 = vertices[j];

        const float cp = v0.x * v1.y - v1.y * v0.y;

        centeroid += (v0 + v1) * cp;
        area += cp;
    }

    return centeroid / (3 * area);
}

math::Vec2 shape2D::Polygon::GetClosestVertice(const math::Vec2& point, int* index) const
{
    float minDist = std::numeric_limits<float>::max();
    math::Vec2 nearest;

    if (index)
    {
        for (int i = 0; i < vertices.size(); i++)
        {
            const math::Vec2& v = vertices[i];
            float dist = v.DistanceSqr(point);
            if (dist < minDist) minDist = dist, nearest = v, *index = i;
        }
    }
    else
    {
        for (const auto& v : vertices)
        {
            float dist = v.DistanceSqr(point);
            if (dist < minDist) minDist = dist, nearest = v;
        }
    }

    return nearest;
}

const math::Vec2& shape2D::Polygon::GetVertice(Uint32 index) const
{
    return vertices[index % vertices.size()];
}

math::Vec2& shape2D::Polygon::GetVertice(Uint32 index)
{
    return vertices[index % vertices.size()];
}

shape2D::Line shape2D::Polygon::GetEdge(Uint32 index) const
{
    return Line(vertices[index % vertices.size()],
                vertices[(index+1) % vertices.size()]);
}

shape2D::AABB shape2D::Polygon::GetAABB() const
{
    math::Vec2 min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    math::Vec2 max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

    for (const auto& vertex : vertices)
    {
        min = min.Min(vertex);
        max = max.Max(vertex);
    }

    return { min, max };
}

/* Collision implmentation */

bool shape2D::Polygon::CollisionAABB(const AABB& aabb) const
{
    // If the center of the bounding box is inside the polygon
    if (CollisionPoint({ (aabb.min.x + aabb.max.x) * 0.5f, (aabb.min.x + aabb.max.y) * 0.5f }))
    {
        return true;
    }

    // If any of the polygon's edges intersect with an edge of the bounding box
    for (Uint32 i = 0, j = 1; i < vertices.size(); i++, j++)
    {
        if (aabb.CollisionLine(Line(vertices[i], vertices[j == vertices.size() ? 0 : j])))
        {
            return true;
        }
    }

    return false;
}


bool shape2D::Polygon::CollisionPoint(const math::Vec2& point) const
{
    bool oddNodes = false;

    for (Uint32 i = 0, j = vertices.size()-1; i < vertices.size(); i++)
    {
        const math::Vec2& p1 = vertices[i];
        const math::Vec2& p2 = vertices[j];

        if (p1.y < point.y and p2.y >= point.y or p2.y < point.y and p1.y >= point.y)
        {
            if (p1.x + ( point.y - p1.y ) / (p2.y - p1.y) * (p2.x - p1.x) < point.x)
            {
                oddNodes = !oddNodes;
            }
        }

        j = i;
    }

    return oddNodes;
}

bool shape2D::Polygon::CollisionLine(const Line& line) const
{
    for (Uint32 i = 0, j = 1; i < vertices.size(); i++, j++)
    {
        if (line.CollisionLine(Line(vertices[i], vertices[j == vertices.size() ? 0 : j])))
        {
            return true;
        }
    }
    return false;
}

bool shape2D::Polygon::CollisionMesh(const Mesh& mesh) const
{
    return mesh.CollisionPolygon(*this);
}

bool shape2D::Polygon::CollisionCircle(const Circle& circle) const
{
    // If the center of the circle is inside the polygon
    if (CollisionPoint(circle.center))
    {
        return true;
    }

    // If any of the polygon's edges intersect the circle
    for (Uint32 i = 0, j = 1; i < vertices.size(); i++, j++)
    {
        if (circle.CollisionLine(Line(vertices[i], vertices[j == vertices.size() ? 0 : j])))
        {
            return true;
        }
    }

    return false;
}

bool shape2D::Polygon::CollisionEllipse(const Ellipse& ellipse) const
{
    // If the center of the circle is inside the polygon
    if (CollisionPoint(ellipse.center))
    {
        return true;
    }

    // If any of the polygon's edges intersect the circle
    for (Uint32 i = 0, j = 1; i < vertices.size(); i++, j++)
    {
        if (ellipse.CollisionLine(Line(vertices[i], vertices[j == vertices.size() ? 0 : j])))
        {
            return true;
        }
    }

    return false;
}

bool shape2D::Polygon::CollisionPolygon(const Polygon& poly) const
{
    for (Uint32 i = 0, j = 1; i < vertices.size(); i++, j++)
    {
        const math::Vec2& p1 = vertices[i];

        // Check if any line segment of this polygon intersects with the other polygon
        if (poly.CollisionLine(Line(p1, vertices[j == vertices.size() ? 0 : j])))
        {
            return true;
        }

        // Check if any vertex of this polygon is inside the other polygon
        if (poly.CollisionPoint(p1))
        {
            return true;
        }
    }

    return false;
}

bool shape2D::Polygon::CollisionTriangle(const Triangle& tri) const
{
    return tri.CollisionPolygon(*this);
}

bool shape2D::Polygon::CollisionTriangle(const TriangleF& tri) const
{
    return tri.CollisionPolygon(*this);
}

bool shape2D::Polygon::CollisionRectangle(const Rectangle& rect) const
{
    // If the center of the square is inside the polygon
    if (CollisionPoint({ rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f }))
    {
        return true;
    }

    // If any of the polygon's edges intersect with an edge of the rectangle
    for (Uint32 i = 0, j = 1; i < vertices.size(); i++, j++)
    {
        if (rect.CollisionLine(Line(vertices[i], vertices[j == vertices.size() ? 0 : j])))
        {
            return true;
        }
    }

    return false;
}

bool shape2D::Polygon::CollisionRectangle(const RectangleF& rect) const
{
    // If the center of the square is inside the polygon
    if (CollisionPoint({ rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f }))
    {
        return true;
    }

    // If any of the polygon's edges intersect with an edge of the rectangle
    for (Uint32 i = 0, j = 1; i < vertices.size(); i++, j++)
    {
        if (rect.CollisionLine(Line(vertices[i], vertices[j == vertices.size() ? 0 : j])))
        {
            return true;
        }
    }

    return false;
}
