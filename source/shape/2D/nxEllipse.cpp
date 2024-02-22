#include "shape/2D/nxEllipse.hpp"

#include "shape/2D/nxAABB.hpp"
#include "shape/2D/nxLine.hpp"
#include "shape/2D/nxMesh.hpp"
#include "shape/2D/nxCircle.hpp"
#include "shape/2D/nxPolygon.hpp"
#include "shape/2D/nxTriangle.hpp"
#include "shape/2D/nxRectangle.hpp"

using namespace nexus;

namespace { class EllipseCollisionHelper
{
  private:
    static constexpr int maxIterations = 4;

  private:
    double innerPolygonCoef[maxIterations];
    double outerPolygonCoef[maxIterations];

    bool iterate(double x, double y, double c0x, double c0y, double c2x, double c2y, double rr) const
    {
        for (int t = 1; t <= maxIterations; t++)
        {
            double c1x = (c0x + c2x)*innerPolygonCoef[t];
            double c1y = (c0y + c2y)*innerPolygonCoef[t];
            double tx = x - c1x;
            double ty = y - c1y;

            if (tx*tx + ty*ty <= rr)
            {
                return true;
            }

            double t2x = c2x - c1x;
            double t2y = c2y - c1y;

            if (tx*t2x + ty*t2y >= 0 && tx*t2x + ty*t2y <= t2x*t2x + t2y*t2y
            && (ty*t2x - tx*t2y >= 0 || rr*(t2x*t2x + t2y*t2y) >= (ty*t2x - tx*t2y)*(ty*t2x - tx*t2y)))
            {
                return true;
            }

            double t0x = c0x - c1x;
            double t0y = c0y - c1y;

            if (tx*t0x + ty*t0y >= 0 && tx*t0x + ty*t0y <= t0x*t0x + t0y*t0y
            && (ty*t0x - tx*t0y <= 0 || rr*(t0x*t0x + t0y*t0y) >= (ty*t0x - tx*t0y)*(ty*t0x - tx*t0y)))
            {
                return true;
            }

            double c3x = (c0x + c1x)*outerPolygonCoef[t];
            double c3y = (c0y + c1y)*outerPolygonCoef[t];

            if ((c3x-x)*(c3x-x) + (c3y-y)*(c3y-y) < rr)
            {
                c2x = c1x;
                c2y = c1y;
                continue;
            }

            double c4x = c1x - c3x + c1x;
            double c4y = c1y - c3y + c1y;

            if ((c4x-x)*(c4x-x) + (c4y-y)*(c4y-y) < rr)
            {
                c0x = c1x;
                c0y = c1y;
                continue;
            }

            double t3x = c3x - c1x;
            double t3y = c3y - c1y;

            if (ty*t3x - tx*t3y <= 0 || rr*(t3x*t3x + t3y*t3y) > (ty*t3x - tx*t3y)*(ty*t3x - tx*t3y))
            {
                if (tx*t3x + ty*t3y > 0)
                {
                    if (fabs(tx*t3x + ty*t3y) <= t3x*t3x + t3y*t3y || (x-c3x)*(c0x-c3x) + (y-c3y)*(c0y-c3y) >= 0)
                    {
                        c2x = c1x;
                        c2y = c1y;
                        continue;
                    }
                }
                else if (-(tx*t3x + ty*t3y) <= t3x*t3x + t3y*t3y || (x-c4x)*(c2x-c4x) + (y-c4y)*(c2y-c4y) >= 0)
                {
                    c0x = c1x;
                    c0y = c1y;
                    continue;
                }
            }
            return false;
        }
        return false; // Out of iterations so it is unsure if there was a collision. But have to return something.
    }

  public:
    // Test for collision between two ellipses, "0" and "1". Ellipse is at (x, y) with major or minor radius 
    // vector (wx, wy) and the other major or minor radius perpendicular to that vector and hw times as long.
    bool collide(double x0, double y0, double wx0, double wy0, double hw0, double x1, double y1, double wx1, double wy1, double hw1) const
    {
        float rr = hw1*hw1*(wx1*wx1 + wy1*wy1)*(wx1*wx1 + wy1*wy1)*(wx1*wx1 + wy1*wy1);
        float x = hw1*wx1*(wy1*(y1 - y0) + wx1*(x1 - x0)) - wy1*(wx1*(y1 - y0) - wy1*(x1 - x0));
        float y = hw1*wy1*(wy1*(y1 - y0) + wx1*(x1 - x0)) + wx1*(wx1*(y1 - y0) - wy1*(x1 - x0));

        float temp = wx0;
        wx0 = hw1*wx1*(wy1*wy0 + wx1*wx0) - wy1*(wx1*wy0 - wy1*wx0);

        float temp2 = wy0;
        wy0 = hw1*wy1*(wy1*wy0 + wx1*temp) + wx1*(wx1*wy0 - wy1*temp);

        float hx0 = hw1*wx1*(wy1*(temp*hw0)-wx1*temp2*hw0)-wy1*(wx1*(temp*hw0)+wy1*temp2*hw0);
        float hy0 = hw1*wy1*(wy1*(temp*hw0)-wx1*temp2*hw0)+wx1*(wx1*(temp*hw0)+wy1*temp2*hw0);

        if (wx0*y - wy0*x < 0)
        {
            x = -x;
            y = -y;
        }
                    
        if ((wx0 - x)*(wx0 - x) + (wy0 - y)*(wy0 - y) <= rr)
        {
            return true;
        }
        else if ((wx0 + x)*(wx0 + x) + (wy0 + y)*(wy0 + y) <= rr)
        {
            return true;
        }
        else if ((hx0 - x)*(hx0 - x) + (hy0 - y)*(hy0 - y) <= rr)
        {
            return true;
        }
        else if ((hx0 + x)*(hx0 + x) + (hy0 + y)*(hy0 + y) <= rr)
        {
            return true;
        }
        else if (x*(hy0 - wy0) + y*(wx0 - hx0) <= hy0*wx0 - hx0*wy0 && y*(wx0 + hx0) - x*(wy0 + hy0) <= hy0*wx0 - hx0*wy0)
        {
            return true;
        }
        else if (x*(wx0-hx0) - y*(hy0-wy0) > hx0*(wx0-hx0) - hy0*(hy0-wy0)     
              && x*(wx0-hx0) - y*(hy0-wy0) < wx0*(wx0-hx0) - wy0*(hy0-wy0)
              && (x*(hy0-wy0) + y*(wx0-hx0) - hy0*wx0 + hx0*wy0)*(x*(hy0-wy0) + y*(wx0-hx0) - hy0*wx0 + hx0*wy0)
              <= rr*((wx0-hx0)*(wx0-hx0) + (wy0-hy0)*(wy0-hy0)))
        {
            return true;
        }
        else if (x*(wx0+hx0) + y*(wy0+hy0) > -wx0*(wx0+hx0) - wy0*(wy0+hy0)
              && x*(wx0+hx0) + y*(wy0+hy0) < hx0*(wx0+hx0) + hy0*(wy0+hy0)
              && (y*(wx0+hx0) - x*(wy0+hy0) - hy0*wx0 + hx0*wy0)*(y*(wx0+hx0) - x*(wy0+hy0) - hy0*wx0 + hx0*wy0)
              <= rr*((wx0+hx0)*(wx0+hx0) + (wy0+hy0)*(wy0+hy0)))
        {
            return true;
        }
        else
        {
            if ((hx0-wx0 - x)*(hx0-wx0 - x) + (hy0-wy0 - y)*(hy0-wy0 - y) <= rr)
            {
                return iterate(x, y, hx0, hy0, -wx0, -wy0, rr);
            }
            else if ((hx0+wx0 - x)*(hx0+wx0 - x) + (hy0+wy0 - y)*(hy0+wy0 - y) <= rr)
            {
                return iterate(x, y, wx0, wy0, hx0, hy0, rr);
            }
            else if ((wx0-hx0 - x)*(wx0-hx0 - x) + (wy0-hy0 - y)*(wy0-hy0 - y) <= rr)
            {
                return iterate(x, y, -hx0, -hy0, wx0, wy0, rr);
            }
            else if ((-wx0-hx0 - x)*(-wx0-hx0 - x) + (-wy0-hy0 - y)*(-wy0-hy0 - y) <= rr)
            {
                return iterate(x, y, -wx0, -wy0, -hx0, -hy0, rr);
            }
            else if (wx0*y - wy0*x < wx0*hy0 - wy0*hx0 && fabs(hx0*y - hy0*x) < hy0*wx0 - hx0*wy0)
            {
                if (hx0*y - hy0*x > 0)
                {
                    return iterate(x, y, hx0, hy0, -wx0, -wy0, rr);
                }
                return iterate(x, y, wx0, wy0, hx0, hy0, rr);
            }
            else if (wx0*x + wy0*y > wx0*(hx0-wx0) + wy0*(hy0-wy0) && wx0*x + wy0*y < wx0*(hx0+wx0) + wy0*(hy0+wy0)
                 && (wx0*y - wy0*x - hy0*wx0 + hx0*wy0)*(wx0*y - wy0*x - hy0*wx0 + hx0*wy0) < rr*(wx0*wx0 + wy0*wy0))
            {
                if (wx0*x + wy0*y > wx0*hx0 + wy0*hy0)
                {
                    return iterate(x, y, wx0, wy0, hx0, hy0, rr);
                }
                return iterate(x, y, hx0, hy0, -wx0, -wy0, rr);
            }
            else
            {
                if (hx0*y - hy0*x < 0)
                {
                    x = -x;
                    y = -y;
                }

                if (hx0*x + hy0*y > -hx0*(wx0+hx0) - hy0*(wy0+hy0) && hx0*x + hy0*y < hx0*(hx0-wx0) + hy0*(hy0-wy0)
                && (hx0*y - hy0*x - hy0*wx0 + hx0*wy0)*(hx0*y - hy0*x - hy0*wx0 + hx0*wy0) < rr*(hx0*hx0 + hy0*hy0))
                {
                    if (hx0*x + hy0*y > -hx0*wx0 - hy0*wy0)
                    {      
                        return iterate(x, y, hx0, hy0, -wx0, -wy0, rr);
                    } 
                    return iterate(x, y, -wx0, -wy0, -hx0, -hy0, rr);
                }
                return false;
            }
        }
    }

    // Test for collision between an ellipse of horizontal radius w0 and vertical radius h0 at (x0, y0) and
    // an ellipse of horizontal radius w1 and vertical radius h1 at (x1, y1)
    bool collide(double x0, double y0, double w0, double h0, double x1, double y1, double w1, double h1) const
    {
        double x = fabs(x1 - x0)*h1;
        double y = fabs(y1 - y0)*w1;

        w0 *= h1;
        h0 *= w1;

        double r = w1*h1;
        
        if (x*x + (h0 - y)*(h0 - y) <= r*r || (w0 - x)*(w0 - x) + y*y <= r*r || x*h0 + y*w0 <= w0*h0
        || ((x*h0 + y*w0 - w0*h0)*(x*h0 + y*w0 - w0*h0) <= r*r*(w0*w0 + h0*h0) && x*w0 - y*h0 >= -h0*h0 && x*w0 - y*h0 <= w0*w0))
        {
            return true;
        }
        else
        {
            if ((x-w0)*(x-w0) + (y-h0)*(y-h0) <= r*r || (x <= w0 && y - r <= h0) || (y <= h0 && x - r <= w0))
            {
                return iterate(x, y, w0, 0, 0, h0, r*r);
            }
            return false;
        }
    }

    // Test for collision between an ellipse of horizontal radius w and vertical radius h at (x0, y0) and
    // a circle of radius r at (x1, y1)
    bool collide(double x0, double y0, double w, double h, double x1, double y1, double r) const
    {
        double x = fabs(x1 - x0);
        double y = fabs(y1 - y0);

        if (x*x + (h - y)*(h - y) <= r*r || (w - x)*(w - x) + y*y <= r*r || x*h + y*w <= w*h
        || ((x*h + y*w - w*h)*(x*h + y*w - w*h) <= r*r*(w*w + h*h) && x*w - y*h >= -h*h && x*w - y*h <= w*w))
        {
            return true;
        }
        else
        {
            if ((x-w)*(x-w) + (y-h)*(y-h) <= r*r || (x <= w && y - r <= h) || (y <= h && x - r <= w))
            {
                return iterate(x, y, w, 0, 0, h, r*r);
            }
            return false;
        }
    }

    EllipseCollisionHelper()
    {
        for (int t = 0; t <= maxIterations; t++)
        {
            int numNodes = 4 << t;
            innerPolygonCoef[t] = 0.5/std::cos(4*std::acos(0.0)/numNodes);
            outerPolygonCoef[t] = 0.5/(std::cos(2*std::acos(0.0)/numNodes)*std::cos(2*std::acos(0.0)/numNodes));
        }
    }
};}

shape2D::AABB shape2D::Ellipse::GetAABB() const
{
    return AABB(
        center.x - radius.x, center.y - radius.y,
        center.x + radius.x, center.y + radius.y);
}

bool shape2D::Ellipse::CollisionAABB(const AABB& aabb) const
{
    return CollisionPoint({
        std::max(aabb.min.x, std::min(center.x, aabb.max.x)),
        std::max(aabb.min.y, std::min(center.y, aabb.max.y))
    });
}

bool shape2D::Ellipse::CollisionPoint(const math::Vec2& point) const
{
    return ((point - center) / radius).Magnitude() <= 1.0;
}

bool shape2D::Ellipse::CollisionLine(const Line& line) const
{
    const math::Vec2 sqRadius = radius * radius;
    const math::Vec2 p1 = line.start - center;
    const math::Vec2 p2 = line.end - center;

    // Calculate the quadratic parameters
    float A = (p2.x - p1.x) * (p2.x - p1.x) / sqRadius.x + (p2.y - p1.y) * (p2.y - p1.y) / sqRadius.y;
    float B = 2 * p1.x * (p2.x - p1.x) / sqRadius.x + 2 * p1.y * (p2.y - p1.y) / sqRadius.y;
    float C = p1.x * p1.x / sqRadius.x + p1.y * p1.y / sqRadius.y - 1;

    // Calculate the discriminant
    float discriminant = B * B - 4 * A * C;

    // Check if there are solutions
    if (discriminant >= 0)
    {
        discriminant = std::sqrt(discriminant);

        float t1 = (-B + discriminant) / (2 * A);
        if (t1 >= 0.0f && t1 <= 1.0f) return true;

        float t2 = (-B - discriminant) / (2 * A);
        if (t2 >= 0.0f && t2 <= 1.0f) return true;
    }

    return false; // No intersection points
}

bool shape2D::Ellipse::CollisionMesh(const Mesh& mesh) const
{
    return mesh.CollisionEllipse(*this);
}

bool shape2D::Ellipse::CollisionCircle(const Circle& circle) const
{
    return EllipseCollisionHelper().collide(center.x, center.y, radius.x, radius.y,
        circle.center.x, circle.center.y, circle.radius);
}

bool shape2D::Ellipse::CollisionEllipse(const Ellipse& ellipse) const
{
    return EllipseCollisionHelper().collide(center.x, center.y, radius.x, radius.y,
        ellipse.center.x, ellipse.center.y, ellipse.radius.x, ellipse.radius.y);
}

bool shape2D::Ellipse::CollisionPolygon(const Polygon& poly) const
{
    return poly.CollisionEllipse(*this);
}

bool shape2D::Ellipse::CollisionTriangle(const Triangle& tri) const
{
    return tri.CollisionEllipse(*this);
}

bool shape2D::Ellipse::CollisionTriangle(const TriangleF& tri) const
{
    return tri.CollisionEllipse(*this);
}

bool shape2D::Ellipse::CollisionRectangle(const Rectangle& rect) const
{
    return CollisionPoint({
        std::max(static_cast<float>(rect.x), std::min(center.x, static_cast<float>(rect.x + rect.w))),
        std::max(static_cast<float>(rect.y), std::min(center.y, static_cast<float>(rect.y + rect.h)))
    });
}

bool shape2D::Ellipse::CollisionRectangle(const RectangleF& rect) const
{
    return CollisionPoint({
        std::max(rect.x, std::min(center.x, rect.x + rect.w)),
        std::max(rect.y, std::min(center.y, rect.y + rect.h))
    });
}
