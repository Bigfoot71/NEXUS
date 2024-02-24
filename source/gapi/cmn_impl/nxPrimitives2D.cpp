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

#include <gapi/cmn_impl/nxPrimitives2D.hpp>

using namespace nexus;

void _gapi_primitives_2d::DrawPixel(gapi::Context& ctx, float x, float y, const gfx::Color& color)
{
    ctx.SetDefaultTexture();
    ctx.Begin(gapi::DrawMode::Quads);

        ctx.Color(color);

        ctx.TexCoord(0, 0);
        ctx.Vertex(x, y);

        ctx.TexCoord(0,1);
        ctx.Vertex(x, y + 1);

        ctx.TexCoord(1, 1);
        ctx.Vertex(x + 1, y + 1);

        ctx.TexCoord(1, 0);
        ctx.Vertex(x + 1, y);

    ctx.End();
    ctx.UnsetTexture();
}

void _gapi_primitives_2d::DrawLine(gapi::Context& ctx, float x1, float y1, float x2, float y2, const gfx::Color& color)
{
    ctx.Begin(gapi::DrawMode::Lines);
        ctx.Color(color);
        ctx.Vertex(x1, y1);
        ctx.Vertex(x2, y2);
    ctx.End();
}

void _gapi_primitives_2d::DrawLine(gapi::Context& ctx, const math::Vec2& a, const math::Vec2& b, float thick, const gfx::Color& color)
{
    const math::Vec2 delta = b - a;
    float length = delta.Magnitude();

    if (length > 0 && thick > 0)
    {
        float scale = thick/(2*length);

        const math::Vec2 radius(
            -scale*delta.y,
            scale*delta.x);

        DrawTriangleStrip(ctx, {
            { a.x - radius.x, a.y - radius.y },
            { a.x + radius.x, a.y + radius.y },
            { b.x - radius.x, b.y - radius.y },
            { b.x + radius.x, b.y + radius.y }
        }, color);
    }
}

void _gapi_primitives_2d::DrawLineStrip(gapi::Context& ctx, const math::Vec2* points, std::size_t count, const gfx::Color& color)
{
    if (count < 2) return;
    ctx.Begin(gapi::DrawMode::Lines);

        ctx.Color(color);

        for (std::size_t i = 0; i < count - 1; i++)
        {
            ctx.Vertex(points[i].x, points[i].y);
            ctx.Vertex(points[i + 1].x, points[i + 1].y);
        }

    ctx.End();
}

void _gapi_primitives_2d::DrawLineStrip(gapi::Context& ctx, const shape2D::Line* lines, std::size_t count, const gfx::Color& color)
{
    ctx.Begin(gapi::DrawMode::Lines);

        ctx.Color(color);

        for (std::size_t i = 0; i < count; i++)
        {
            const auto &line = lines[i];
            ctx.Vertex(line.start.x, line.start.y);
            ctx.Vertex(line.end.x, line.end.y);
        }

    ctx.End();
}

void _gapi_primitives_2d::DrawLineBezier(gapi::Context& ctx, const math::Vec2& start, const math::Vec2& end, float thick, const gfx::Color& color)
{
    math::Vec2 previous = start;
    math::Vec2 current;

    std::vector<math::Vec2> points(2 * SplineSegmentDivisions + 2);

    for (int i = 1; i <= SplineSegmentDivisions; i++)
    {
        const double t = static_cast<double>(i) / static_cast<double>(SplineSegmentDivisions);
        current.y = static_cast<float>(math::EaseCubicInOut(t) * (end.y - start.y) + start.y);
        current.x = previous.x + (end.x - start.x)/SplineSegmentDivisions;

        const math::Vec2 delta = current - previous;
        const float size = 0.5f*thick/delta.Magnitude();

        if (i == 1)
        {
            points[0].x = previous.x + delta.y * size;
            points[0].y = previous.y - delta.x * size;
            points[1].x = previous.x - delta.y * size;
            points[1].y = previous.y + delta.x * size;
        }

        points[2 * i + 1].x = current.x - delta.y * size;
        points[2 * i + 1].y = current.y + delta.x * size;
        points[2 * i].x = current.x + delta.y * size;
        points[2 * i].y = current.y - delta.x * size;

        previous = current;
    }

    DrawTriangleStrip(ctx, points, color);
}

void _gapi_primitives_2d::DrawCircle(gapi::Context& ctx, float x, float y, float radius, const gfx::Color& color)
{
    float angle = 0.0f;

    ctx.SetDefaultTexture();
    ctx.Begin(gapi::DrawMode::Quads);

        ctx.Color(color);

        for (int i = 0; i < CircleSegments; i++)
        {
            ctx.TexCoord(0, 0);
            ctx.Vertex(x, y);

            ctx.TexCoord(1.0f, 0);
            ctx.Vertex(x + std::cos(angle + 2 * CircleSegStepLen) * radius, y + std::sin(angle + 2 * CircleSegStepLen) * radius);

            ctx.TexCoord(1.0f, 1.0f);
            ctx.Vertex(x + std::cos(angle + CircleSegStepLen) * radius, y + std::sin(angle + CircleSegStepLen) * radius);

            ctx.TexCoord(0, 1.0f);
            ctx.Vertex(x + std::cos(angle) * radius, y + std::sin(angle) * radius);

            angle += CircleSegStepLen;
        }
    ctx.End();
    ctx.UnsetTexture();
}

void _gapi_primitives_2d::DrawCircleGradient(gapi::Context& ctx, float x, float y, float radius, const gfx::Color& color1, const gfx::Color& color2)
{
    float angle = 0.0f;

    ctx.Begin(gapi::DrawMode::Triangles);
        for (int i = 0; i < CircleSegments; i++)
        {
            ctx.Color(color1.r, color1.g, color1.b, color1.a);
            ctx.Vertex(x, y);

            ctx.Color(color2.r, color2.g, color2.b, color2.a);
            ctx.Vertex(x + std::cos(angle + CircleSegStepLen) * radius, y + std::sin(angle + CircleSegStepLen) * radius);
            ctx.Vertex(x + std::cos(angle) * radius, y + std::sin(angle) * radius);

            angle += CircleSegStepLen;
        }
    ctx.End();
}

void _gapi_primitives_2d::DrawCircleLines(gapi::Context& ctx, float x, float y, float radius, const gfx::Color& color)
{
    float angle = 0.0f;

    ctx.Begin(gapi::DrawMode::Lines);

        ctx.Color(color);

        // NOTE: Circle outline is drawn pixel by pixel every degree (0 to 360)
        for (int i = 0; i < CircleSegments; i++)
        {
            ctx.Vertex(x + std::cos(angle) * radius, y + std::sin(angle) * radius);
            ctx.Vertex(x + std::cos(angle+CircleSegStepLen) * radius, y + std::sin(angle+CircleSegStepLen) * radius);
            angle += CircleSegStepLen;
        }

    ctx.End();
}

void _gapi_primitives_2d::DrawCircleSector(gapi::Context& ctx, const math::Vec2& center, float radius, float startAngle, float endAngle, int segments, const gfx::Color& color)
{
    if (radius <= 0.0f) radius = 0.1f;  // Avoid div by zero

    // Function expects (endAngle > startAngle)
    if (endAngle < startAngle)
    {
        std::swap(startAngle, endAngle);
    }

    // Convert degrees to radians
    startAngle *= math::Deg2Rad;
    endAngle *= math::Deg2Rad;

    int minSegments = std::ceil((endAngle - startAngle) / math::DegToRad(90.0f));

    if (segments < minSegments)
    {
        // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
        float th = std::acos(2 * std::pow(1 - SmoothCircleErrorRate / radius, 2) - 1);
        segments = std::ceil(math::Tau / th) / 4.0f;

        if (segments <= 0) segments = minSegments;
    }

    float stepLength = (endAngle - startAngle) / static_cast<float>(segments);
    float angle = startAngle;

    ctx.SetDefaultTexture(); ;
    ctx.Begin(gapi::DrawMode::Quads);

        ctx.Color(color);

        // NOTE: Every QUAD actually represents two segments
        for (int i = 0; i < segments; i++)
        {
            ctx.TexCoord(0, 0);
            ctx.Vertex(center.x, center.y);

            ctx.TexCoord(1, 0);
            ctx.Vertex(center.x + std::cos(angle + stepLength) * radius, center.y + std::sin(angle + stepLength) * radius);

            ctx.TexCoord(1, 1);
            ctx.Vertex(center.x + std::cos(angle) * radius, center.y + std::sin(angle) * radius);

            ctx.TexCoord(0, 1);
            ctx.Vertex(center.x, center.y);

            angle += stepLength;
        }

    ctx.End();
    ctx.UnsetTexture();
}

void _gapi_primitives_2d::DrawCircleSectorLines(gapi::Context& ctx, const math::Vec2& center, float radius, float startAngle, float endAngle, int segments, const gfx::Color& color)
{
    if (radius <= 0.0f) radius = 0.1f;  // Avoid div by zero issue

    // Function expects (endAngle > startAngle)
    if (endAngle < startAngle)
    {
        std::swap(startAngle, endAngle);
    }

    // Convert degrees to radians
    startAngle *= math::Deg2Rad;
    endAngle *= math::Deg2Rad;

    int minSegments = std::ceil((endAngle - startAngle) / math::DegToRad(90.0f));

    if (segments < minSegments)
    {
        // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
        float th = std::acos(2 * std::pow(1 - SmoothCircleErrorRate / radius, 2) - 1);
        segments = std::ceil(math::Tau / th) / 4.0f;

        if (segments <= 0) segments = minSegments;
    }

    float stepLength = (endAngle - startAngle) / static_cast<float>(segments);
    float angle = startAngle;
    bool showCapLines = true;

    ctx.Begin(gapi::DrawMode::Lines);

        ctx.Color(color);

        if (showCapLines)
        {
            ctx.Vertex(center.x, center.y);
            ctx.Vertex(center.x + std::cos(angle) * radius, center.y + std::sin(angle) * radius);
        }

        for (int i = 0; i < segments; i++)
        {
            ctx.Vertex(center.x + std::cos(angle) * radius, center.y + std::sin(angle) * radius);
            ctx.Vertex(center.x + std::cos(angle + stepLength) * radius, center.y + std::sin(angle + stepLength) * radius);

            angle += stepLength;
        }

        if (showCapLines)
        {
            ctx.Vertex(center.x, center.y);
            ctx.Vertex(center.x + std::cos(angle) * radius, center.y + std::sin(angle) * radius);
        }

    ctx.End();
}

void _gapi_primitives_2d::DrawEllipse(gapi::Context& ctx, float x, float y, float rx, float ry, const gfx::Color& color)
{
    float angle = 0.0f;

    ctx.Begin(gapi::DrawMode::Triangles);

        ctx.Color(color);

        for (int i = 0; i < CircleSegments; i++)
        {
            ctx.Vertex(x, y);
            ctx.Vertex(x + std::cos(angle + CircleSegStepLen) * rx, y + std::sin(angle + CircleSegStepLen) * ry);
            ctx.Vertex(x + std::cos(angle) * rx, y + std::sin(angle) * ry);

            angle += CircleSegStepLen;
        }

    ctx.End();
}

void _gapi_primitives_2d::DrawEllipseLines(gapi::Context& ctx, int x, int y, float rx, float ry, const gfx::Color& color)
{
    float angle = 0.0f;

    ctx.Begin(gapi::DrawMode::Lines);

        ctx.Color(color);

        for (int i = 0; i < CircleSegments; i++)
        {
            ctx.Vertex(x + std::cos(angle + CircleSegStepLen) * rx, y + std::sin(angle + CircleSegStepLen) * ry);
            ctx.Vertex(x + std::cos(angle) * rx, y + std::sin(angle) * ry);

            angle += CircleSegStepLen;
        }

    ctx.End();
}

void _gapi_primitives_2d::DrawRing(gapi::Context& ctx, const math::Vec2& center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, const gfx::Color& color)
{
    if (startAngle == endAngle) return;

    // Function expects (outerRadius > innerRadius)
    if (outerRadius < innerRadius)
    {
        std::swap(innerRadius, outerRadius);
        if (outerRadius <= 0.0f) outerRadius = 0.1f;
    }

    // Not a ring
    if (innerRadius <= 0.0f)
    {
        DrawCircleSector(ctx, center, outerRadius, startAngle, endAngle, segments, color);
        return;
    }

    // Function expects (endAngle > startAngle)
    if (endAngle < startAngle)
    {
        std::swap(startAngle, endAngle);
    }

    int minSegments = std::ceil((endAngle - startAngle) / math::DegToRad(90.0f));

    if (segments < minSegments)
    {
        // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
        float th = std::acos(2 * std::pow(1 - SmoothCircleErrorRate / outerRadius, 2) - 1);
        segments = std::ceil(math::Tau / th) / 4.0f;

        if (segments <= 0) segments = minSegments;
    }

    float stepLength = (endAngle - startAngle)/segments;
    float angle = startAngle;

    ctx.SetDefaultTexture();
    ctx.Begin(gapi::DrawMode::Quads);

        ctx.Color(color);

        for (int i = 0; i < segments; i++)
        {
            ctx.TexCoord(0, 1);
            ctx.Vertex(center.x + std::cos(angle) * outerRadius, center.y + std::sin(angle) * outerRadius);

            ctx.TexCoord(0, 0);
            ctx.Vertex(center.x + std::cos(angle) * innerRadius, center.y + std::sin(angle) * innerRadius);

            ctx.TexCoord(1, 0);
            ctx.Vertex(center.x + std::cos(angle + stepLength) * innerRadius, center.y + std::sin(angle + stepLength) * innerRadius);

            ctx.TexCoord(1, 1);
            ctx.Vertex(center.x + std::cos(angle + stepLength) * outerRadius, center.y + std::sin(angle + stepLength) * outerRadius);

            angle += stepLength;
        }

    ctx.End();
    ctx.UnsetTexture();
}

void _gapi_primitives_2d::DrawRingLines(gapi::Context& ctx, const math::Vec2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, const gfx::Color& color)
{
    if (startAngle == endAngle) return;

    // Function expects (outerRadius > innerRadius)
    if (outerRadius < innerRadius)
    {
        std::swap(innerRadius, outerRadius);
        if (outerRadius <= 0.0f) outerRadius = 0.1f;
    }

    if (innerRadius <= 0.0f)
    {
        DrawCircleSectorLines(ctx, center, outerRadius, startAngle, endAngle, segments, color);
        return;
    }

    // Function expects (endAngle > startAngle)
    if (endAngle < startAngle)
    {
        std::swap(startAngle, endAngle);
    }

    int minSegments = std::ceil((endAngle - startAngle) / math::DegToRad(90.0f));

    if (segments < minSegments)
    {
        // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
        float th = std::acos(2 * std::pow(1 - SmoothCircleErrorRate / outerRadius, 2) - 1);
        segments = std::ceil(math::Tau / th) / 4.0f;

        if (segments <= 0) segments = minSegments;
    }

    float stepLength = (endAngle - startAngle)/segments;
    float angle = startAngle;
    bool showCapLines = true;

    ctx.Begin(gapi::DrawMode::Lines);

        ctx.Color(color);

        if (showCapLines)
        {
            ctx.Vertex(center.x + std::cos(angle) * outerRadius, center.y + std::sin(angle) * outerRadius);
            ctx.Vertex(center.x + std::cos(angle) * innerRadius, center.y + std::sin(angle) * innerRadius);
        }

        for (int i = 0; i < segments; i++)
        {
            ctx.Vertex(center.x + std::cos(angle) * outerRadius, center.y + std::sin(angle) * outerRadius);
            ctx.Vertex(center.x + std::cos(angle + stepLength) * outerRadius, center.y + std::sin(angle + stepLength) * outerRadius);

            ctx.Vertex(center.x + std::cos(angle) * innerRadius, center.y + std::sin(angle) * innerRadius);
            ctx.Vertex(center.x + std::cos(angle + stepLength) * innerRadius, center.y + std::sin(angle + stepLength) * innerRadius);

            angle += stepLength;
        }

        if (showCapLines)
        {
            ctx.Vertex(center.x + std::cos(angle) * outerRadius, center.y + std::sin(angle) * outerRadius);
            ctx.Vertex(center.x + std::cos(angle) * innerRadius, center.y + std::sin(angle) * innerRadius);
        }

    ctx.End();
}

void _gapi_primitives_2d::DrawRectangle(gapi::Context& ctx, const shape2D::RectangleF& rec, const math::Vec2& origin, float rotation, const gfx::Color& color)
{
    math::Vec2 topLeft, topRight;
    math::Vec2 bottomLeft, bottomRight;

    if (rotation == 0.0f)
    {
        float x = rec.x - origin.x;
        float y = rec.y - origin.y;

        topLeft = { x, y };
        topRight = { x + rec.w, y };
        bottomLeft = { x, y + rec.h };
        bottomRight = { x + rec.w, y + rec.h };
    }
    else
    {
        rotation *= math::Deg2Rad;

        float s = std::sin(rotation);
        float c = std::cos(rotation);

        float x = rec.x, y = rec.y;
        float dx = -origin.x, dy = -origin.y;

        topLeft.x = x + dx * c - dy * s;
        topLeft.y = y + dx * s + dy * c;

        topRight.x = x + (dx + rec.w) * c - dy * s;
        topRight.y = y + (dx + rec.w) * s + dy * c;

        bottomLeft.x = x + dx * c - (dy + rec.h) * s;
        bottomLeft.y = y + dx * s + (dy + rec.h) * c;

        bottomRight.x = x + (dx + rec.w) * c - (dy + rec.h) * s;
        bottomRight.y = y + (dx + rec.w) * s + (dy + rec.h) * c;
    }

    ctx.SetDefaultTexture();
    ctx.Begin(gapi::DrawMode::Quads);

        ctx.Color(color);

        ctx.TexCoord(0, 0);
        ctx.Vertex(topLeft.x, topLeft.y);

        ctx.TexCoord(0, 1);
        ctx.Vertex(bottomLeft.x, bottomLeft.y);

        ctx.TexCoord(1, 1);
        ctx.Vertex(bottomRight.x, bottomRight.y);

        ctx.TexCoord(1, 0);
        ctx.Vertex(topRight.x, topRight.y);

    ctx.End();
    ctx.UnsetTexture();
}

void _gapi_primitives_2d::DrawRectangleGradient(gapi::Context& ctx, const shape2D::RectangleF& rec, const gfx::Color& c1, const gfx::Color& c2, const gfx::Color& c3, const gfx::Color& c4)
{
    ctx.SetDefaultTexture();
    ctx.Begin(gapi::DrawMode::Quads);

        ctx.Color(c1.r, c1.g, c1.b, c1.a);
        ctx.TexCoord(0, 0);
        ctx.Vertex(rec.x, rec.y);

        ctx.Color(c2.r, c2.g, c2.b, c2.a);
        ctx.TexCoord(0, 1);
        ctx.Vertex(rec.x, rec.y + rec.h);

        ctx.Color(c3.r, c3.g, c3.b, c3.a);
        ctx.TexCoord(1, 1);
        ctx.Vertex(rec.x + rec.w, rec.y + rec.h);

        ctx.Color(c4.r, c4.g, c4.b, c4.a);
        ctx.TexCoord(1, 0);
        ctx.Vertex(rec.x + rec.w, rec.y);

    ctx.End();
    ctx.UnsetTexture();
}

void _gapi_primitives_2d::DrawRectangleLines(gapi::Context& ctx, float x, float y, float w, float h, const gfx::Color& color)
{
    ctx.Begin(gapi::DrawMode::Lines);

        ctx.Color(color);

        ctx.Vertex(x + 1, y + 1);
        ctx.Vertex(x + w, y + 1);

        ctx.Vertex(x + w, y + 1);
        ctx.Vertex(x + w, y + h);

        ctx.Vertex(x + w, y + h);
        ctx.Vertex(x + 1, y + h);

        ctx.Vertex(x + 1, y + h);
        ctx.Vertex(x + 1, y + 1);

    ctx.End();
}

void _gapi_primitives_2d::DrawRectangleLines(gapi::Context& ctx, const shape2D::RectangleF& rec, float lineThick, const gfx::Color& color)
{
    if (lineThick > rec.w || lineThick > rec.h)
    {
        if (rec.w > rec.h) lineThick = rec.h/2;
        else if (rec.w < rec.h) lineThick = rec.w/2;
    }

    DrawRectangle(ctx, { rec.x, rec.y, rec.w, lineThick }, color);
    DrawRectangle(ctx, { rec.x, rec.y - lineThick + rec.h, rec.w, lineThick }, color);
    DrawRectangle(ctx, { rec.x, rec.y + lineThick, lineThick, rec.h - lineThick*2.0f }, color);
    DrawRectangle(ctx, { rec.x - lineThick + rec.w, rec.y + lineThick, lineThick, rec.h - lineThick*2.0f }, color);
}

void _gapi_primitives_2d::DrawRectangleRounded(gapi::Context& ctx, const shape2D::RectangleF& rec, float roundness, int segments, const gfx::Color& color)
{
    // Not a rounded rectangle
    if ((roundness <= 0.0f) || (rec.w < 1) || (rec.h < 1 ))
    {
        DrawRectangle(ctx, rec, color);
        return;
    }

    if (roundness >= 1.0f) roundness = 1.0f;

    // Calculate corner radius
    float radius = (rec.w > rec.h)? (rec.h*roundness)/2 : (rec.w*roundness)/2;
    if (radius <= 0.0f) return;

    // Calculate number of segments to use for the corners
    if (segments < 4)
    {
        // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
        float th = std::acos(2 * std::pow(1 - SmoothCircleErrorRate / radius, 2) - 1);
        segments = std::ceil(math::Tau / th) / 4.0f;

        if (segments <= 0) segments = 4;
    }

    float stepLength = math::DegToRad(90.0f)/segments;

    // Coordinates of the 12 points that define the rounded rect
    const math::Vec2 point[12] = {
        {rec.x + radius, rec.y}, {(rec.x + rec.w) - radius, rec.y}, { rec.x + rec.w, rec.y + radius },      // PO, P1, P2
        {rec.x + rec.w, (rec.y + rec.h) - radius}, {(rec.x + rec.w) - radius, rec.y + rec.h},                   // P3, P4
        {rec.x + radius, rec.y + rec.h}, { rec.x, (rec.y + rec.h) - radius}, {rec.x, rec.y + radius},       // P5, P6, P7
        {rec.x + radius, rec.y + radius}, {(rec.x + rec.w) - radius, rec.y + radius},                           // P8, P9
        {(rec.x + rec.w) - radius, (rec.y + rec.h) - radius}, {rec.x + radius, (rec.y + rec.h) - radius}        // P10, P11
    };

    // [1] Upper Left Corner, [3] Upper Right Corner, [5] Lower Right Corner, [7] Lower Left Corner
    const math::Vec2 centers[4] = { point[8], point[9], point[10], point[11] };
    constexpr float angles[4] = { math::DegToRad(180.0f), math::DegToRad(270.0f), 0.0f, math::DegToRad(90.0f) };

    ctx.SetDefaultTexture();
    ctx.Begin(gapi::DrawMode::Quads);

        ctx.Color(color);

        for (int k = 0; k < 4; ++k)
        {
            float angle = angles[k];
            const math::Vec2 &center = centers[k];

            // NOTE: Every QUAD actually represents two segments
            for (int i = 0; i < segments/2; i++)
            {
                ctx.TexCoord(0, 0);
                ctx.Vertex(center.x, center.y);

                ctx.TexCoord(1, 0);
                ctx.Vertex(center.x + std::cos(angle + 2 * stepLength) * radius, center.y + std::sin(angle + 2 * stepLength) * radius);

                ctx.TexCoord(1, 1);
                ctx.Vertex(center.x + std::cos(angle + stepLength) * radius, center.y + std::sin(angle + stepLength) * radius);

                ctx.TexCoord(0, 1);
                ctx.Vertex(center.x + std::cos(angle) * radius, center.y + std::sin(angle) * radius);

                angle += (stepLength*2);
            }

            // NOTE: In case number of segments is odd, we add one last piece to the cake
            if (segments%2)
            {
                ctx.TexCoord(0, 0);
                ctx.Vertex(center.x, center.y);

                ctx.TexCoord(1, 1);
                ctx.Vertex(center.x + std::cos(angle + stepLength) * radius, center.y + std::sin(angle + stepLength) * radius);

                ctx.TexCoord(0, 1);
                ctx.Vertex(center.x + std::cos(angle) * radius, center.y + std::sin(angle) * radius);

                ctx.TexCoord(1, 0);
                ctx.Vertex(center.x, center.y);
            }
        }

        // [2] Upper Rectangle
        ctx.TexCoord(0, 0);
        ctx.Vertex(point[0].x, point[0].y);
        ctx.TexCoord(0, 1);
        ctx.Vertex(point[8].x, point[8].y);
        ctx.TexCoord(1, 1);
        ctx.Vertex(point[9].x, point[9].y);
        ctx.TexCoord(1, 0);
        ctx.Vertex(point[1].x, point[1].y);

        // [4] Right Rectangle
        ctx.TexCoord(0, 0);
        ctx.Vertex(point[2].x, point[2].y);
        ctx.TexCoord(0, 1);
        ctx.Vertex(point[9].x, point[9].y);
        ctx.TexCoord(1, 1);
        ctx.Vertex(point[10].x, point[10].y);
        ctx.TexCoord(1, 0);
        ctx.Vertex(point[3].x, point[3].y);

        // [6] Bottom Rectangle
        ctx.TexCoord(0, 0);
        ctx.Vertex(point[11].x, point[11].y);
        ctx.TexCoord(0, 1);
        ctx.Vertex(point[5].x, point[5].y);
        ctx.TexCoord(1, 1);
        ctx.Vertex(point[4].x, point[4].y);
        ctx.TexCoord(1, 0);
        ctx.Vertex(point[10].x, point[10].y);

        // [8] Left Rectangle
        ctx.TexCoord(0, 0);
        ctx.Vertex(point[7].x, point[7].y);
        ctx.TexCoord(0, 1);
        ctx.Vertex(point[6].x, point[6].y);
        ctx.TexCoord(1, 1);
        ctx.Vertex(point[11].x, point[11].y);
        ctx.TexCoord(1, 0);
        ctx.Vertex(point[8].x, point[8].y);

        // [9] Middle Rectangle
        ctx.TexCoord(0, 0);
        ctx.Vertex(point[8].x, point[8].y);
        ctx.TexCoord(0, 1);
        ctx.Vertex(point[11].x, point[11].y);
        ctx.TexCoord(1, 1);
        ctx.Vertex(point[10].x, point[10].y);
        ctx.TexCoord(1, 0);
        ctx.Vertex(point[9].x, point[9].y);

    ctx.End();
    ctx.UnsetTexture();
}

void _gapi_primitives_2d::DrawRectangleRoundedLines(gapi::Context& ctx, const shape2D::RectangleF& rec, float roundness, int segments, float lineThick, const gfx::Color& color)
{
    if (lineThick < 0) lineThick = 0;

    // Not a rounded rectangle
    if (roundness <= 0.0f)
    {
        DrawRectangleLines(ctx, {rec.x-lineThick, rec.y-lineThick, rec.w+2*lineThick, rec.h+2*lineThick}, lineThick, color);
        return;
    }

    if (roundness >= 1.0f) roundness = 1.0f;

    // Calculate corner radius
    float radius = (rec.w > rec.h)? (rec.h*roundness)/2 : (rec.w*roundness)/2;
    if (radius <= 0.0f) return;

    // Calculate number of segments to use for the corners
    if (segments < 4)
    {
        // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
        float th = std::acos(2 * std::pow(1 - SmoothCircleErrorRate / radius, 2) - 1);
        segments = std::ceil(math::Tau / th) / 4.0f;

        if (segments <= 0) segments = 4;
    }

    float stepLength = math::DegToRad(90.0f)/segments;
    const float outerRadius = radius + lineThick, innerRadius = radius;

    const math::Vec2 point[16] = {
        {rec.x + innerRadius, rec.y - lineThick}, {(rec.x + rec.w) - innerRadius, rec.y - lineThick}, { rec.x + rec.w + lineThick, rec.y + innerRadius },   // PO, P1, P2
        {rec.x + rec.w + lineThick, (rec.y + rec.h) - innerRadius}, {(rec.x + rec.w) - innerRadius, rec.y + rec.h + lineThick},                                 // P3, P4
        {rec.x + innerRadius, rec.y + rec.h + lineThick}, { rec.x - lineThick, (rec.y + rec.h) - innerRadius}, {rec.x - lineThick, rec.y + innerRadius},    // P5, P6, P7
        {rec.x + innerRadius, rec.y}, {(rec.x + rec.w) - innerRadius, rec.y},                                                                                   // P8, P9
        { rec.x + rec.w, rec.y + innerRadius }, {rec.x + rec.w, (rec.y + rec.h) - innerRadius},                                                                 // P10, P11
        {(rec.x + rec.w) - innerRadius, rec.y + rec.h}, {rec.x + innerRadius, rec.y + rec.h},                                                                   // P12, P13
        { rec.x, (rec.y + rec.h) - innerRadius}, {rec.x, rec.y + innerRadius}                                                                                   // P14, P15
    };

    const math::Vec2 centers[4] = {
        {rec.x + innerRadius, rec.y + innerRadius}, {(rec.x + rec.w) - innerRadius, rec.y + innerRadius},                                                       // P16, P17
        {(rec.x + rec.w) - innerRadius, (rec.y + rec.h) - innerRadius}, {rec.x + innerRadius, (rec.y + rec.h) - innerRadius}                                    // P18, P19
    };

    constexpr float angles[4] = {
        math::DegToRad(180.0f),
        math::DegToRad(270.0f),
        0.0f,
        math::DegToRad(90.0f)
    };

    if (lineThick > 1)
    {
        ctx.SetDefaultTexture();
        ctx.Begin(gapi::DrawMode::Quads);

            ctx.Color(color);

            for (int k = 0; k < 4; ++k)
            {
                float angle = angles[k];
                const math::Vec2 &center = centers[k];

                for (int i = 0; i < segments; i++)
                {
                    ctx.Color(color);

                    ctx.TexCoord(0, 0);
                    ctx.Vertex(center.x + std::cos(angle) * innerRadius, center.y + std::sin(angle) * innerRadius);

                    ctx.TexCoord(1, 0);
                    ctx.Vertex(center.x + std::cos(angle + stepLength) * innerRadius, center.y + std::sin(angle + stepLength) * innerRadius);

                    ctx.TexCoord(1, 1);
                    ctx.Vertex(center.x + std::cos(angle + stepLength) * outerRadius, center.y + std::sin(angle + stepLength) * outerRadius);

                    ctx.TexCoord(0, 1);
                    ctx.Vertex(center.x + std::cos(angle) * outerRadius, center.y + std::sin(angle) * outerRadius);

                    angle += stepLength;
                }
            }

            // Upper rectangle
            ctx.TexCoord(0, 0);
            ctx.Vertex(point[0].x, point[0].y);
            ctx.TexCoord(0, 1);
            ctx.Vertex(point[8].x, point[8].y);
            ctx.TexCoord(1, 1);
            ctx.Vertex(point[9].x, point[9].y);
            ctx.TexCoord(1, 0);
            ctx.Vertex(point[1].x, point[1].y);

            // Right rectangle
            ctx.TexCoord(0, 0);
            ctx.Vertex(point[2].x, point[2].y);
            ctx.TexCoord(0, 1);
            ctx.Vertex(point[10].x, point[10].y);
            ctx.TexCoord(1, 1);
            ctx.Vertex(point[11].x, point[11].y);
            ctx.TexCoord(1, 0);
            ctx.Vertex(point[3].x, point[3].y);

            // Lower rectangle
            ctx.TexCoord(0, 0);
            ctx.Vertex(point[13].x, point[13].y);
            ctx.TexCoord(0, 1);
            ctx.Vertex(point[5].x, point[5].y);
            ctx.TexCoord(1, 1);
            ctx.Vertex(point[4].x, point[4].y);
            ctx.TexCoord(1, 0);
            ctx.Vertex(point[12].x, point[12].y);

            // Left rectangle
            ctx.TexCoord(0, 0);
            ctx.Vertex(point[15].x, point[15].y);
            ctx.TexCoord(0, 1);
            ctx.Vertex(point[7].x, point[7].y);
            ctx.TexCoord(1, 1);
            ctx.Vertex(point[6].x, point[6].y);
            ctx.TexCoord(1, 0);
            ctx.Vertex(point[14].x, point[14].y);

        ctx.End();
        ctx.UnsetTexture();
    }
    else
    {
        // Use lines to draw the outline
        ctx.Begin(gapi::DrawMode::Lines);

            // Draw all the 4 corners
            for (int k = 0; k < 4; ++k)
            {
                float angle = angles[k];
                const math::Vec2 &center = centers[k];

                for (int i = 0; i < segments; i++)
                {
                    ctx.Color(color);
                    ctx.Vertex(center.x + std::cos(angle) * outerRadius, center.y + std::sin(angle) * outerRadius);
                    ctx.Vertex(center.x + std::cos(angle + stepLength) * outerRadius, center.y + std::sin(angle + stepLength) * outerRadius);
                    angle += stepLength;
                }
            }

            // And the remaining 4 lines
            for (int i = 0; i < 8; i += 2)
            {
                ctx.Color(color);
                ctx.Vertex(point[i].x, point[i].y);
                ctx.Vertex(point[i + 1].x, point[i + 1].y);
            }

        ctx.End();
    }
}

void _gapi_primitives_2d::DrawTriangle(gapi::Context& ctx, const math::Vec2& p1, const math::Vec2& p2, const math::Vec2& p3, const gfx::Color& color)
{
#   if SUPPORT_SOFTWARE_RASTERIZER

    IF_GAPI_IS(sr)
    {
        ctx.Begin(gapi::DrawMode::Triangles);

            ctx.Color(color);

            ctx.Vertex(p1.x, p1.y);
            ctx.Vertex(p2.x, p2.y);
            ctx.Vertex(p3.x, p3.y);

        ctx.End();
    }
    else
    {
        ctx.SetDefaultTexture();
        ctx.Begin(gapi::DrawMode::Quads);

            ctx.Color(color);

            ctx.TexCoord(0, 0);
            ctx.Vertex(p1.x, p1.y);

            ctx.TexCoord(0, 1);
            ctx.Vertex(p2.x, p2.y);

            ctx.TexCoord(1, 1);
            ctx.Vertex(p2.x, p2.y);

            ctx.TexCoord(1, 0);
            ctx.Vertex(p3.x, p3.y);

        ctx.End();
        ctx.UnsetTexture();
    }

#   else

    ctx.SetDefaultTexture();
    ctx.Begin(gapi::DrawMode::Quads);

        ctx.Color(color);

        ctx.TexCoord(0, 0);
        ctx.Vertex(p1.x, p1.y);

        ctx.TexCoord(0, 1);
        ctx.Vertex(p2.x, p2.y);

        ctx.TexCoord(1, 1);
        ctx.Vertex(p2.x, p2.y);

        ctx.TexCoord(1, 0);
        ctx.Vertex(p3.x, p3.y);

    ctx.End();
    ctx.UnsetTexture();

#   endif
}

void _gapi_primitives_2d::DrawTriangleLines(gapi::Context& ctx, const math::Vec2& p1, const math::Vec2& p2, const math::Vec2& p3, const gfx::Color& color)
{
    ctx.Begin(gapi::DrawMode::Lines);

        ctx.Color(color);
        ctx.Vertex(p1.x, p1.y);
        ctx.Vertex(p2.x, p2.y);

        ctx.Vertex(p2.x, p2.y);
        ctx.Vertex(p3.x, p3.y);

        ctx.Vertex(p3.x, p3.y);
        ctx.Vertex(p1.x, p1.y);

    ctx.End();
}

void _gapi_primitives_2d::DrawTriangleFan(gapi::Context& ctx, const math::Vec2* points, std::size_t count, const gfx::Color& color)
{
    if (count < 3) return;

#   if SUPPORT_SOFTWARE_RASTERIZER

    IF_GAPI_IS(sr)
    {
        ctx.Begin(gapi::DrawMode::Triangles);

            ctx.Color(color);

            for (int i = 1; i < count - 1; i++)
            {
                ctx.TexCoord(0, 0);
                ctx.Vertex(points[0]);

                ctx.TexCoord(0, 1);
                ctx.Vertex(points[i]);

                ctx.TexCoord(1, 1);
                ctx.Vertex(points[i + 1]);
            }

        ctx.End();
    }
    else
    {
        ctx.SetDefaultTexture();
        ctx.Begin(gapi::DrawMode::Quads);

            ctx.Color(color);

            for (int i = 1; i < count - 1; i++)
            {
                ctx.TexCoord(0, 0);
                ctx.Vertex(points[0]);

                ctx.TexCoord(0, 1);
                ctx.Vertex(points[i]);

                ctx.TexCoord(1, 1);
                ctx.Vertex(points[i + 1]);

                ctx.TexCoord(1, 0);
                ctx.Vertex(points[i + 1]);
            }

        ctx.End();
        ctx.UnsetTexture();
    }

#   else

    ctx.SetDefaultTexture();
    ctx.Begin(gapi::DrawMode::Quads);

        ctx.Color(color);

        for (int i = 1; i < count - 1; i++)
        {
            ctx.TexCoord(0, 0);
            ctx.Vertex(points[0]);

            ctx.TexCoord(0, 1);
            ctx.Vertex(points[i]);

            ctx.TexCoord(1, 1);
            ctx.Vertex(points[i + 1]);

            ctx.TexCoord(1, 0);
            ctx.Vertex(points[i + 1]);
        }

    ctx.End();
    ctx.UnsetTexture();

#   endif
}

void _gapi_primitives_2d::DrawTriangleStrip(gapi::Context& ctx, const math::Vec2* points, std::size_t count, const gfx::Color& color)
{
    if (count < 3) return;

    ctx.Begin(gapi::DrawMode::Triangles);

        ctx.Color(color);

        for (int i = 2; i < count; i++)
        {
            if (i % 2 == 0)
            {
                ctx.Vertex(points[i]);
                ctx.Vertex(points[i - 2]);
                ctx.Vertex(points[i - 1]);
            }
            else
            {
                ctx.Vertex(points[i]);
                ctx.Vertex(points[i - 1]);
                ctx.Vertex(points[i - 2]);
            }
        }

    ctx.End();
}

void _gapi_primitives_2d::DrawPolygon(gapi::Context& ctx, const math::Vec2& center, int sides, float radius, float rotation, const gfx::Color& color)
{
    if (sides < 3) sides = 3;

    rotation *= math::Deg2Rad;
    float angleStep = math::Tau / sides;

    ctx.SetDefaultTexture();
    ctx.Begin(gapi::DrawMode::Quads);

        ctx.Color(color);

        for (int i = 0; i < sides; i++)
        {
            float nextAngle = rotation + angleStep;

            ctx.TexCoord(0, 0);
            ctx.Vertex(center.x, center.y);

            ctx.TexCoord(0, 1);
            ctx.Vertex(center.x + std::cos(rotation) * radius, center.y + std::sin(rotation) * radius);

            ctx.TexCoord(1, 0);
            ctx.Vertex(center.x + std::cos(nextAngle) * radius, center.y + std::sin(nextAngle) * radius);

            ctx.TexCoord(1, 1);
            ctx.Vertex(center.x + std::cos(rotation) * radius, center.y + std::sin(rotation) * radius);

            rotation = nextAngle;
        }

    ctx.End();
    ctx.UnsetTexture();
}

void _gapi_primitives_2d::DrawPolygonLines(gapi::Context& ctx, const math::Vec2& center, int sides, float radius, float rotation, const gfx::Color& color)
{
    if (sides < 3) sides = 3;

    rotation *= math::Deg2Rad;
    float angleStep = math::Tau / sides;

    ctx.Begin(gapi::DrawMode::Lines);

        ctx.Color(color);

        for (int i = 0; i < sides; i++)
        {
            ctx.Vertex(center.x + std::cos(rotation) * radius, center.y + std::sin(rotation) * radius);
            ctx.Vertex(center.x + std::cos(rotation + angleStep) * radius, center.y + std::sin(rotation + angleStep) * radius);

            rotation += angleStep;
        }

    ctx.End();
}

void _gapi_primitives_2d::DrawPolygonLines(gapi::Context& ctx, const math::Vec2& center, int sides, float radius, float rotation, float lineThick, const gfx::Color& color)
{
    if (sides < 3) sides = 3;

    float centralAngle = rotation * math::Deg2Rad;
    float exteriorAngle = math::Tau / sides;

    float innerRadius = radius - lineThick * std::cos(exteriorAngle * 0.5f);

    ctx.SetDefaultTexture();
    ctx.Begin(gapi::DrawMode::Quads);

        ctx.Color(color);

        for (int i = 0; i < sides; i++)
        {
            float nextAngle = centralAngle + exteriorAngle;

            ctx.TexCoord(0, 1);
            ctx.Vertex(center.x + std::cos(centralAngle) * radius, center.y + std::sin(centralAngle) * radius);

            ctx.TexCoord(0, 0);
            ctx.Vertex(center.x + std::cos(centralAngle) * innerRadius, center.y + std::sin(centralAngle) * innerRadius);

            ctx.TexCoord(1, 1);
            ctx.Vertex(center.x + std::cos(nextAngle) * innerRadius, center.y + std::sin(nextAngle) * innerRadius);

            ctx.TexCoord(1, 0);
            ctx.Vertex(center.x + std::cos(nextAngle) * radius, center.y + std::sin(nextAngle) * radius);

            centralAngle = nextAngle;
        }

    ctx.End();
    ctx.UnsetTexture();
}

void _gapi_primitives_2d::DrawSplineLinear(gapi::Context& ctx, const math::Vec2* points, std::size_t count, float thick, const gfx::Color& color)
{
    math::Vec2 delta{};
    float length = 0.0f;
    float scale = 0.0f;
    
    for (int i = 0; i < count - 1; i++)
    {
        delta = points[i + 1] - points[i];
        length = delta.Length();

        if (length > 0) scale = thick / (2 * length);

        math::Vec2 radius = {
            -scale*delta.y, scale*delta.x
        };

        math::Vec2 strip[4] = {
            { points[i].x - radius.x, points[i].y - radius.y },
            { points[i].x + radius.x, points[i].y + radius.y },
            { points[i + 1].x - radius.x, points[i + 1].y - radius.y },
            { points[i + 1].x + radius.x, points[i + 1].y + radius.y }
        };

        DrawTriangleStrip(ctx, strip, 4, color);
    }
}

void _gapi_primitives_2d::DrawSplineBasis(gapi::Context& ctx, const math::Vec2* points, std::size_t count, float thick, const gfx::Color& color)
{
    if (count < 4) return;

    float a[4]{};
    float b[4]{};
    float dy = 0.0f;
    float dx = 0.0f;
    float size = 0.0f;

    math::Vec2 currentPoint{};
    math::Vec2 nextPoint{};
    math::Vec2 vertices[2 * SplineSegmentDivisions + 2]{};

    for (int i = 0; i < (count - 3); i++)
    {
        float t = 0.0f;
        math::Vec2 p1 = points[i], p2 = points[i + 1], p3 = points[i + 2], p4 = points[i + 3];

        a[0] = (-p1.x + 3.0f * p2.x - 3.0f * p3.x + p4.x) / 6.0f;
        a[1] = (3.0f * p1.x - 6.0f * p2.x + 3.0f * p3.x) / 6.0f;
        a[2] = (-3.0f * p1.x + 3.0f * p3.x) / 6.0f;
        a[3] = (p1.x + 4.0f * p2.x + p3.x) / 6.0f;

        b[0] = (-p1.y + 3.0f * p2.y - 3.0f * p3.y + p4.y) / 6.0f;
        b[1] = (3.0f * p1.y - 6.0f * p2.y + 3.0f * p3.y) / 6.0f;
        b[2] = (-3.0f * p1.y + 3.0f * p3.y) / 6.0f;
        b[3] = (p1.y + 4.0f * p2.y + p3.y) / 6.0f;

        currentPoint.x = a[3];
        currentPoint.y = b[3];

        if (i == 0) DrawCircle(ctx, currentPoint, thick * 0.5f, color);   // Draw init line circle-cap

        if (i > 0)
        {
            vertices[0].x = currentPoint.x + dy * size;
            vertices[0].y = currentPoint.y - dx * size;
            vertices[1].x = currentPoint.x - dy * size;
            vertices[1].y = currentPoint.y + dx * size;
        }

        for (int j = 1; j <= SplineSegmentDivisions; j++)
        {
            t = static_cast<float>(j) / (SplineSegmentDivisions);

            nextPoint.x = a[3] + t * (a[2] + t * (a[1] + t * a[0]));
            nextPoint.y = b[3] + t * (b[2] + t * (b[1] + t * b[0]));

            dy = nextPoint.y - currentPoint.y;
            dx = nextPoint.x - currentPoint.x;
            size = (0.5f * thick) / std::sqrt(dx*dx+dy*dy);

            if (i == 0 && j == 1)
            {
                vertices[0].x = currentPoint.x + dy * size;
                vertices[0].y = currentPoint.y - dx * size;
                vertices[1].x = currentPoint.x - dy * size;
                vertices[1].y = currentPoint.y + dx * size;
            }

            vertices[2 * j + 1].x = nextPoint.x - dy * size;
            vertices[2 * j + 1].y = nextPoint.y + dx * size;
            vertices[2 * j].x = nextPoint.x + dy * size;
            vertices[2 * j].y = nextPoint.y - dx * size;

            currentPoint = nextPoint;
        }

        DrawTriangleStrip(ctx, vertices, 2 * SplineSegmentDivisions + 2, color);
    }

    DrawCircle(ctx, currentPoint, thick * 0.5f, color);   // Draw end line circle-cap
}

void _gapi_primitives_2d::DrawSplineCatmullRom(gapi::Context& ctx, const math::Vec2* points, std::size_t count, float thick, const gfx::Color& color)
{
    if (count < 4) return;

    float dy = 0.0f;
    float dx = 0.0f;
    float size = 0.0f;

    math::Vec2 currentPoint = points[1];
    math::Vec2 nextPoint{};
    math::Vec2 vertices[2 * SplineSegmentDivisions + 2]{};

    DrawCircle(ctx, currentPoint, thick/2.0f, color);   // Draw init line circle-cap

    for (int i = 0; i < (count - 3); i++)
    {
        float t = 0.0f;
        math::Vec2 p1 = points[i], p2 = points[i + 1], p3 = points[i + 2], p4 = points[i + 3];

        if (i > 0)
        {
            vertices[0].x = currentPoint.x + dy * size;
            vertices[0].y = currentPoint.y - dx * size;
            vertices[1].x = currentPoint.x - dy * size;
            vertices[1].y = currentPoint.y + dx * size;
        }

        for (int j = 1; j <= SplineSegmentDivisions; j++)
        {
            t = static_cast<float>(j)/(SplineSegmentDivisions);

            float q0 = (-1.0f * t * t * t) + (2.0f * t * t) + (-1.0f * t);
            float q1 = (3.0f * t * t * t) + (-5.0f * t * t) + 2.0f;
            float q2 = (-3.0f * t * t * t) + (4.0f * t * t) + t;
            float q3 = t * t * t - t * t;

            nextPoint.x = 0.5f * ((p1.x * q0) + (p2.x * q1) + (p3.x * q2) + (p4.x * q3));
            nextPoint.y = 0.5f * ((p1.y * q0) + (p2.y * q1) + (p3.y * q2) + (p4.y * q3));

            dy = nextPoint.y - currentPoint.y;
            dx = nextPoint.x - currentPoint.x;
            size = (0.5f * thick) / std::sqrt(dx*dx+dy*dy);

            if (i == 0 && j == 1)
            {
                vertices[0].x = currentPoint.x + dy * size;
                vertices[0].y = currentPoint.y - dx * size;
                vertices[1].x = currentPoint.x - dy * size;
                vertices[1].y = currentPoint.y + dx * size;
            }

            vertices[2 * j + 1].x = nextPoint.x - dy * size;
            vertices[2 * j + 1].y = nextPoint.y + dx * size;
            vertices[2 * j].x = nextPoint.x + dy * size;
            vertices[2 * j].y = nextPoint.y - dx * size;

            currentPoint = nextPoint;
        }

        DrawTriangleStrip(ctx, vertices, 2 * SplineSegmentDivisions + 2, color);
    }

    DrawCircle(ctx, currentPoint, thick/2.0f, color);   // Draw end line circle-cap
}

void _gapi_primitives_2d::DrawSplineBezierQuadratic(gapi::Context& ctx, const math::Vec2* points, std::size_t count, float thick, const gfx::Color& color)
{
    if (count < 3) return;
    
    for (int i = 0; i < count - 2; i++)
    {
        DrawSplineSegmentBezierQuadratic(ctx, points[i], points[i + 1], points[i + 2], thick, color);
    }
}

void _gapi_primitives_2d::DrawSplineBezierCubic(gapi::Context& ctx, const math::Vec2* points, std::size_t count, float thick, const gfx::Color& color)
{
    if (count < 4) return;
    
    for (int i = 0; i < count - 3; i++)
    {
        DrawSplineSegmentBezierCubic(ctx, points[i], points[i + 1], points[i + 2], points[i + 3], thick, color);
    }
}

void _gapi_primitives_2d::DrawSplineSegmentLinear(gapi::Context& ctx, const math::Vec2& p1, const math::Vec2& p2, float thick, const gfx::Color& color)
{
    // NOTE: For the linear spline we don't use subdivisions, just a single quad
    
    math::Vec2 delta = p2 - p1;
    float length = delta.Length();

    if (length > 0 && thick > 0)
    {
        float scale = thick/(2*length);

        math::Vec2 radius = {
            -scale * delta.y, scale * delta.x
        };

        math::Vec2 strip[4] = {
            { p1.x - radius.x, p1.y - radius.y },
            { p1.x + radius.x, p1.y + radius.y },
            { p2.x - radius.x, p2.y - radius.y },
            { p2.x + radius.x, p2.y + radius.y }
        };

        DrawTriangleStrip(ctx, strip, 4, color);
    }
}

void _gapi_primitives_2d::DrawSplineSegmentBasis(gapi::Context& ctx, const math::Vec2& p1, const math::Vec2& p2, const math::Vec2& p3, const math::Vec2& p4, float thick, const gfx::Color& color)
{
    constexpr float step = 1.0f / SplineSegmentDivisions;

    math::Vec2 currentPoint{};
    math::Vec2 nextPoint{};
    float t = 0.0f;
    
    math::Vec2 points[2 * SplineSegmentDivisions + 2]{};
    
    float a[4]{}, b[4]{};

    a[0] = (-p1.x + 3 * p2.x - 3 * p3.x + p4.x) / 6.0f;
    a[1] = (3 * p1.x - 6 * p2.x + 3 * p3.x) / 6.0f;
    a[2] = (-3 * p1.x + 3 * p3.x) / 6.0f;
    a[3] = (p1.x + 4 * p2.x + p3.x) / 6.0f;

    b[0] = (-p1.y + 3 * p2.y - 3 * p3.y + p4.y) / 6.0f;
    b[1] = (3 * p1.y - 6 * p2.y + 3 * p3.y) / 6.0f;
    b[2] = (-3 * p1.y + 3 * p3.y) / 6.0f;
    b[3] = (p1.y + 4*p2.y + p3.y) / 6.0f;

    currentPoint.x = a[3];
    currentPoint.y = b[3];

    for (int i = 0; i <= SplineSegmentDivisions; i++)
    {
        t = step * i;

        nextPoint.x = a[3] + t * (a[2] + t * (a[1] + t * a[0]));
        nextPoint.y = b[3] + t * (b[2] + t * (b[1] + t * b[0]));

        float dy = nextPoint.y - currentPoint.y;
        float dx = nextPoint.x - currentPoint.x;
        float size = (0.5f * thick) / std::sqrt(dx*dx+dy*dy);

        if (i == 1)
        {
            points[0].x = currentPoint.x + dy * size;
            points[0].y = currentPoint.y - dx * size;
            points[1].x = currentPoint.x - dy * size;
            points[1].y = currentPoint.y + dx * size;
        }

        points[2 * i + 1].x = nextPoint.x - dy * size;
        points[2 * i + 1].y = nextPoint.y + dx * size;
        points[2 * i].x = nextPoint.x + dy * size;
        points[2 * i].y = nextPoint.y - dx * size;

        currentPoint = nextPoint;
    }

    DrawTriangleStrip(ctx, points, 2 * SplineSegmentDivisions + 2, color);
}

void _gapi_primitives_2d::DrawSplineSegmentCatmullRom(gapi::Context& ctx, const math::Vec2& p1, const math::Vec2& p2, const math::Vec2& p3, const math::Vec2& p4, float thick, const gfx::Color& color)
{
    constexpr float step = 1.0f / SplineSegmentDivisions;

    math::Vec2 currentPoint = p1;
    math::Vec2 nextPoint{};
    float t = 0.0f;
    
    math::Vec2 points[2 * SplineSegmentDivisions + 2]{};

    for (int i = 0; i <= SplineSegmentDivisions; i++)
    {
        t = step * i;

        float q0 = (-1 * t * t * t) + (2 * t * t) + (-1 * t);
        float q1 = (3 * t * t * t) + (-5 * t * t) + 2;
        float q2 = (-3 * t * t * t) + (4 * t * t) + t;
        float q3 = t * t * t - t * t;

        nextPoint.x = 0.5f * ((p1.x * q0) + (p2.x * q1) + (p3.x * q2) + (p4.x * q3));
        nextPoint.y = 0.5f * ((p1.y * q0) + (p2.y * q1) + (p3.y * q2) + (p4.y * q3));

        float dy = nextPoint.y - currentPoint.y;
        float dx = nextPoint.x - currentPoint.x;
        float size = (0.5f * thick) / std::sqrt(dx*dx+dy*dy);

        if (i == 1)
        {
            points[0].x = currentPoint.x + dy * size;
            points[0].y = currentPoint.y - dx * size;
            points[1].x = currentPoint.x - dy * size;
            points[1].y = currentPoint.y + dx * size;
        }

        points[2 * i + 1].x = nextPoint.x - dy * size;
        points[2 * i + 1].y = nextPoint.y + dx * size;
        points[2 * i].x = nextPoint.x + dy * size;
        points[2 * i].y = nextPoint.y - dx * size;

        currentPoint = nextPoint;
    }

    DrawTriangleStrip(ctx, points, 2 * SplineSegmentDivisions + 2, color);
}

void _gapi_primitives_2d::DrawSplineSegmentBezierQuadratic(gapi::Context& ctx, const math::Vec2& p1, const math::Vec2& c2, const math::Vec2& p3, float thick, const gfx::Color& color)
{
    constexpr float step = 1.0f / SplineSegmentDivisions;

    math::Vec2 previous = p1;
    math::Vec2 current{};
    float t = 0.0f;

    math::Vec2 points[2 * SplineSegmentDivisions + 2]{};

    for (int i = 1; i <= SplineSegmentDivisions; i++)
    {
        t = step * i;

        float a = std::pow(1.0f - t, 2);
        float b = 2.0f * (1.0f - t) * t;
        float c = std::pow(t, 2);

        // NOTE: The easing functions aren't suitable here because they don't take a control point
        current.y = a * p1.y + b * c2.y + c * p3.y;
        current.x = a * p1.x + b * c2.x + c * p3.x;

        float dy = current.y - previous.y;
        float dx = current.x - previous.x;
        float size = (0.5f * thick) / std::sqrt(dx*dx+dy*dy);

        if (i == 1)
        {
            points[0].x = previous.x + dy * size;
            points[0].y = previous.y - dx * size;
            points[1].x = previous.x - dy * size;
            points[1].y = previous.y + dx * size;
        }

        points[2 * i + 1].x = current.x - dy * size;
        points[2 * i + 1].y = current.y + dx * size;
        points[2 * i].x = current.x + dy * size;
        points[2 * i].y = current.y - dx * size;

        previous = current;
    }

    DrawTriangleStrip(ctx, points, 2 * SplineSegmentDivisions + 2, color);
}

void _gapi_primitives_2d::DrawSplineSegmentBezierCubic(gapi::Context& ctx, const math::Vec2& p1, const math::Vec2& c2, const math::Vec2& c3, const math::Vec2& p4, float thick, const gfx::Color& color)
{
    constexpr float step = 1.0f / SplineSegmentDivisions;

    math::Vec2 previous = p1;
    math::Vec2 current{};
    float t = 0.0f;

    math::Vec2 points[2 * SplineSegmentDivisions + 2]{};

    for (int i = 1; i <= SplineSegmentDivisions; i++)
    {
        t = step*i;

        float a = std::pow(1.0f - t, 3);
        float b = 3.0f * std::pow(1.0f - t, 2) * t;
        float c = 3.0f * (1.0f - t) * std::pow(t, 2);
        float d = std::pow(t, 3);

        current.y = a * p1.y + b * c2.y + c * c3.y + d * p4.y;
        current.x = a * p1.x + b * c2.x + c * c3.x + d * p4.x;

        float dy = current.y - previous.y;
        float dx = current.x - previous.x;
        float size = (0.5f * thick) / std::sqrt(dx*dx+dy*dy);

        if (i == 1)
        {
            points[0].x = previous.x + dy * size;
            points[0].y = previous.y - dx * size;
            points[1].x = previous.x - dy * size;
            points[1].y = previous.y + dx * size;
        }

        points[2 * i + 1].x = current.x - dy * size;
        points[2 * i + 1].y = current.y + dx * size;
        points[2 * i].x = current.x + dy * size;
        points[2 * i].y = current.y - dx * size;

        previous = current;
    }

    DrawTriangleStrip(ctx, points, 2 * SplineSegmentDivisions + 2, color);
}
