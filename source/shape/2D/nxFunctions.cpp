#include "shape/2D/nxFunctions.hpp"

using namespace nexus;

// Get spline point for a given t [0.0f .. 1.0f], Linear
math::Vec2 shape2D::GetSplinePointLinear(math::Vec2 startPos, math::Vec2 endPos, float t)
{
    return startPos * (1.0f - t) + endPos * t;
}

// Get spline point for a given t [0.0f .. 1.0f], B-Spline
math::Vec2 shape2D::GetSplinePointBasis(math::Vec2 p1, math::Vec2 p2, math::Vec2 p3, math::Vec2 p4, float t)
{
    const float a[4] = {
        (-p1.x + 3 * p2.x - 3 * p3.x + p4.x) / 6.0f,
        (3 * p1.x - 6 * p2.x + 3 * p3.x) / 6.0f,
        (-3 * p1.x + 3 * p3.x) / 6.0f,
        (p1.x + 4 * p2.x + p3.x) / 6.0f
    };

    const float b[4] = {
        (-p1.y + 3 * p2.y - 3 * p3.y + p4.y) / 6.0f,
        (3 * p1.y - 6 * p2.y + 3 * p3.y) / 6.0f,
        (-3 * p1.y + 3 * p3.y) / 6.0f,
        (p1.y + 4 * p2.y + p3.y) / 6.0f
    };

    return {
        a[3] + t * (a[2] + t * (a[1] + t * a[0])),
        b[3] + t * (b[2] + t * (b[1] + t * b[0]))
    };
}

// Get spline point for a given t [0.0f .. 1.0f], Catmull-Rom
math::Vec2 shape2D::GetSplinePointCatmullRom(math::Vec2 p1, math::Vec2 p2, math::Vec2 p3, math::Vec2 p4, float t)
{
    float q0 = (-1*t*t*t) + (2*t*t) + (-1*t);
    float q1 = (3*t*t*t) + (-5*t*t) + 2;
    float q2 = (-3*t*t*t) + (4*t*t) + t;
    float q3 = t*t*t - t*t;

    return {
        0.5f * ((p1.x * q0) + (p2.x * q1) + (p3.x * q2) + (p4.x * q3)),
        0.5f * ((p1.y * q0) + (p2.y * q1) + (p3.y * q2) + (p4.y * q3))
    };
}

// Get spline point for a given t [0.0f .. 1.0f], Quadratic Bezier
math::Vec2 shape2D::GetSplinePointBezierQuad(math::Vec2 startPos, math::Vec2 controlPos, math::Vec2 endPos, float t)
{
    float a = std::pow(1.0f - t, 2);
    float b = 2.0f * (1.0f - t) * t;
    float c = std::pow(t, 2);

    return {
        a * startPos.y + b * controlPos.y + c * endPos.y,
        a * startPos.x + b * controlPos.x + c * endPos.x
    };
}

// Get spline point for a given t [0.0f .. 1.0f], Cubic Bezier
math::Vec2 shape2D::GetSplinePointBezierCubic(math::Vec2 startPos, math::Vec2 startControlPos, math::Vec2 endControlPos, math::Vec2 endPos, float t)
{
    float a = std::pow(1.0f - t, 3);
    float b = 3.0f * std::pow(1.0f - t, 2) * t;
    float c = 3.0f * (1.0f - t)*std::pow(t, 2);
    float d = std::pow(t, 3);

    return {
        a * startPos.y + b * startControlPos.y + c * endControlPos.y + d * endPos.y,
        a * startPos.x + b * startControlPos.x + c * endControlPos.x + d * endPos.x
    };
}
