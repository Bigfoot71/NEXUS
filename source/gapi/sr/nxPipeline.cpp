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

#include "gapi/sr/nxPipeline.hpp"

using namespace nexus;


/* Private Implementation Pipeline (Projection and Clipping) */

void sr::Pipeline::HomogeneousToScreen(math::Vec4& homogeneous, const nexus::shape2D::Rectangle& viewport)
{
    homogeneous.x = (homogeneous.x + 1.0f) * 0.5f * viewport.w;
    homogeneous.y = (1.0f - homogeneous.y) * 0.5f * viewport.h;
}

_sr_impl::Vertex sr::Pipeline::VertexInterpolation(const _sr_impl::Vertex& start, const _sr_impl::Vertex& end, float t)
{
    return {
        math::Lerp(start.position, end.position, t),
        math::Lerp(start.normal, end.normal, t),
        math::Lerp(start.texcoord, end.texcoord, t),
        math::Lerp(start.color.Normalized(), end.color.Normalized(), t)
    };
}

bool sr::Pipeline::ClipLine2D(_sr_impl::Vertex& v0, _sr_impl::Vertex& v1, const shape2D::Rectangle& viewport)
{
    constexpr Uint8 INSIDE    = 0x00;    // 0000
    constexpr Uint8 LEFT      = 0x01;    // 0001
    constexpr Uint8 RIGHT     = 0x02;    // 0010
    constexpr Uint8 BOTTOM    = 0x04;    // 0100
    constexpr Uint8 TOP       = 0x08;    // 1000

    constexpr auto swap = [](_sr_impl::Vertex& v0, _sr_impl::Vertex& v1) -> void
    {
        _sr_impl::Vertex t = v0;
        v0 = v1, v1 = t;
    };

    constexpr auto encode = [](const math::Vec4& v, const shape2D::Rectangle& viewport) -> Uint8
    {
        Uint8 code = INSIDE;
        if (v.x < viewport.x) code |= LEFT;
        if (v.x > viewport.w) code |= RIGHT;
        if (v.y < viewport.y) code |= BOTTOM;
        if (v.y > viewport.h) code |= TOP;
        return code;
    };

    bool accept = false;
    Uint8 code0, code1;
    float m = 0;

    if (v0.position.x != v1.position.x)
    {
        m = (v1.position.y - v0.position.y) / (v1.position.x - v0.position.x);
    }

    for (;;)
    {
        code0 = encode(v0.position, viewport);
        code1 = encode(v1.position, viewport);

        // Accepted if both endpoints lie within rectangle
        if ((code0 | code1) == 0)
        {
            accept = true;
            break;
        }

        // Rejected if both endpoints are outside rectangle, in same region
        if (code0 & code1) break;

        if (code0 == INSIDE)
        {
            std::swap(code0, code1);
            swap(v0, v1);
        }

        if (code0 & LEFT)
        {
            v0.position.y += (viewport.x - v0.position.x) * m;
            v0.position.x = viewport.x;
        }
        else if (code0 & RIGHT)
        {
            v0.position.y += (viewport.w - v0.position.x) * m;
            v0.position.x = viewport.w;
        }
        else if (code0 & BOTTOM)
        {
            if (m) v0.position.x += (viewport.y - v0.position.y) / m;
            v0.position.y = viewport.y;
        }
        else if (code0 & TOP)
        {
            if (m) v0.position.x += (viewport.h - v0.position.y) / m;
            v0.position.y = viewport.h;
        }
    }

    return accept;
}

bool sr::Pipeline::ClipLine3D(_sr_impl::Vertex& v0, _sr_impl::Vertex& v1)
{
    float t0 = 0, t1 = 1;

    constexpr auto clip = [](float q, float p, float& t0, float& t1) -> bool
    {
        if (std::abs(p) < 1e-5f && q < 0)
        {
            return false;
        }

        const float r = q / p;

        if (p < 0)
        {
            if (r > t1) return false;
            if (r > t0) t0 = r;
        }
        else
        {
            if (r < t0) return false;
            if (r < t1) t1 = r;
        }

        return true;
    };

    const math::Vec4 delta = v1.position - v0.position;

    if (!clip(v0.position.w - v0.position.x, -delta.w + delta.x, t0, t1)) return false;
    if (!clip(v0.position.w + v0.position.x, -delta.w - delta.x, t0, t1)) return false;

    if (!clip(v0.position.w - v0.position.y, -delta.w + delta.y, t0, t1)) return false;
    if (!clip(v0.position.w + v0.position.y, -delta.w - delta.y, t0, t1)) return false;

    if (!clip(v0.position.w - v0.position.z, -delta.w + delta.z, t0, t1)) return false;
    if (!clip(v0.position.w + v0.position.z, -delta.w - delta.z, t0, t1)) return false;

    if (t1 < 1) v1.position = v0.position + delta * t1;
    if (t0 > 0) v0.position = v0.position + delta * t0;

    return true;
}

bool sr::Pipeline::ClipPolygonW(std::array<_sr_impl::Vertex, 12>& polygon, Uint8& vertexCounter)
{
    constexpr float ClipEpsilon = 1e-5f;

    std::array<_sr_impl::Vertex, 12> input = polygon;
    Uint8 inputCounter = vertexCounter;
    vertexCounter = 0;

    const _sr_impl::Vertex *prevVt = &input[inputCounter-1];
    Sint8 currDot, prevDot = (prevVt->position.w < ClipEpsilon) ? -1 : 1;

    for (Uint8 i = 0; i < inputCounter; i++)
    {
        const auto &inVertex = input[i];

        currDot = (inVertex.position.w < ClipEpsilon) ? -1 : 1;

        if (prevDot * currDot < 0)
        {
            polygon[vertexCounter++] = VertexInterpolation(*prevVt, inVertex, 
                (ClipEpsilon - prevVt->position.w) / (prevVt->position.w - inVertex.position.w));
        }

        if (currDot > 0)
        {
            polygon[vertexCounter++] = inVertex;
        }

        prevDot = currDot;
        prevVt = &inVertex;
    }

    return vertexCounter > 0;
}

bool sr::Pipeline::ClipPolygonXYZ(std::array<_sr_impl::Vertex, 12>& polygon, Uint8& vertexCounter)
{
    for (Uint8 iAxis = 0; iAxis < 3; iAxis++)
    {
        if (vertexCounter == 0) return false;

        std::array<_sr_impl::Vertex, 12> input = polygon;
        Uint8 inputCounter = vertexCounter;
        vertexCounter = 0;
        
        const _sr_impl::Vertex *prevVt = &input[inputCounter-1];
        Sint8 prevDotHigh = (prevVt->position[iAxis] <= prevVt->position.w) ? 1 : -1;
        Sint8 prevDotLow = (-prevVt->position[iAxis] <= prevVt->position.w) ? 1 : -1;

        for (Uint8 i = 0; i < inputCounter; i++)
        {
            const auto &inVertex = input[i];

            const Sint8 currDotHigh = (inVertex.position[iAxis] <= inVertex.position.w) ? 1 : -1;
            const Sint8 currDotLow = (-inVertex.position[iAxis] <= inVertex.position.w) ? 1 : -1;

            if (prevDotHigh * currDotHigh < 0)
            {
                polygon[vertexCounter++] = VertexInterpolation(*prevVt, inVertex, 
                    (prevVt->position.w - prevVt->position[iAxis]) / ((prevVt->position.w - prevVt->position[iAxis]) - (inVertex.position.w - inVertex.position[iAxis])));
            }

            if (prevDotLow * currDotLow < 0)
            {
                polygon[vertexCounter++] = VertexInterpolation(*prevVt, inVertex, 
                    (prevVt->position.w + prevVt->position[iAxis]) / ((prevVt->position.w + prevVt->position[iAxis]) - (inVertex.position.w + inVertex.position[iAxis])));
            }

            if (currDotHigh > 0 && currDotLow > 0)
            {
                polygon[vertexCounter++] = inVertex;
            }

            prevDotHigh = currDotHigh;
            prevDotLow = currDotLow;
            prevVt = &inVertex;
        }
    }

    return vertexCounter > 0;
}

void sr::Pipeline::ProjectAndClipLine(std::array<_sr_impl::Vertex, 2>& line, Uint8& vertexCounter, const math::Mat4& mvp, const shape2D::Rectangle& viewport, Shader* shader)
{
    line[0].position = shader->Vertex(mvp, line[0].position);
    line[1].position = shader->Vertex(mvp, line[1].position);

    if (line[0].position.w == 1.0f && line[1].position.w == 1.0f)
    {
        HomogeneousToScreen(line[0].position, viewport);
        HomogeneousToScreen(line[1].position, viewport);

        if (!ClipLine2D(line[0], line[1], viewport))
        {
            vertexCounter = 0;
            return;
        }
    }
    else
    {
        if (!ClipLine3D(line[0], line[1]))
        {
            vertexCounter = 0;
            return;
        }

        line[0].position /= line[0].position.w;
        line[1].position /= line[1].position.w;

        HomogeneousToScreen(line[0].position, viewport);
        HomogeneousToScreen(line[1].position, viewport);
    }
}

void sr::Pipeline::ProjectAndClipTriangle(std::array<_sr_impl::Vertex, 12>& polygon, Uint8& vertexCounter, const math::Mat4& mvp, const shape2D::Rectangle& viewport, nexus::sr::Shader* shader, bool& is2D)
{
    for (int i = 0; i < vertexCounter; i++)
    {
        polygon[i].position = shader->Vertex(mvp, polygon[i].position);
    }

    if (polygon[0].position.w == 1.0f && polygon[1].position.w == 1.0f && polygon[2].position.w == 1.0f)
    {
        for (int i = 0; i < vertexCounter; i++)
        {
            HomogeneousToScreen(polygon[i].position, viewport);
        }
        is2D = true;
    }
    else
    {
        if (ClipPolygonW(polygon, vertexCounter) && ClipPolygonXYZ(polygon, vertexCounter))
        {
            for (int i = 0; i < vertexCounter; i++)
            {
                polygon[i].position /= polygon[i].position.w;
                HomogeneousToScreen(polygon[i].position, viewport);
            }
        }
        is2D = false;
    }
}


/* Private Implementation Pipeline (Rasterization) */

void sr::Pipeline::RasterizeLine(Framebuffer& framebuffer, const _sr_impl::Vertex& v0, const _sr_impl::Vertex& v1, bool depthTest)
{
    const float dx = v1.position.x - v0.position.x;
    const float dy = v1.position.y - v0.position.y;

    if (dx == 0 && dy == 0)
    {
        framebuffer.SetPixelDepthUnsafe(v0.position.x, v0.position.y, v0.position.z, v0.color, depthTest);
        return;
    }

    const float adx = std::fabs(dx);
    const float ady = std::fabs(dy);

    if (adx > ady)
    {
        const float invAdx = 1.0f / adx;
        const float slope = dy / dx;

        int xMin, xMax;
        float zMin, zMax;
        if (v0.position.x < v1.position.x)
        {
            xMin = v0.position.x, xMax = v1.position.x;
            zMin = v0.position.z, zMax = v1.position.z;
        }
        else
        {
            xMin = v1.position.x, xMax = v0.position.x;
            zMin = v1.position.z, zMax = v0.position.z;
        }

        for (int x = xMin; x <= xMax; x++)
        {
            const float t = (x - xMin) * invAdx;
            const int y = v0.position.y + (x - v0.position.x) * slope;

            if (!depthTest || framebuffer.SetDepthUnsafe(x, y, zMin + t * (zMax - zMin)))
            {
                framebuffer.SetPixelUnsafe(x, y, math::Lerp(v0.color, v1.color, t));
            }
        }
    }
    else
    {
        const float invAdy = 1.0f / ady;
        const float slope = dx / dy;

        int yMin, yMax;
        float zMin, zMax;
        if (v0.position.y < v1.position.y)
        {
            yMin = v0.position.y, yMax = v1.position.y;
            zMin = v0.position.z, zMax = v1.position.z;
        }
        else
        {
            yMin = v1.position.y, yMax = v0.position.y;
            zMin = v1.position.z, zMax = v0.position.z;
        }

        for (int y = yMin; y <= yMax; y++)
        {
            const float t = (y - yMin) * invAdy;
            const int x = v0.position.x + (y - v0.position.y) * slope;

            if (!depthTest || framebuffer.SetDepthUnsafe(x, y, zMin + t * (zMax - zMin)))
            {
                framebuffer.SetPixelUnsafe(x, y, math::Lerp(v0.color, v1.color, t));
            }
        }
    }
}

void sr::Pipeline::RasterizeTriangleColor2D(Framebuffer& framebuffer, const _sr_impl::Vertex& v0, const _sr_impl::Vertex& v1, const _sr_impl::Vertex& v2, sr::Shader* shader, bool depthTest, const shape2D::Rectangle& viewport)
{
    // Get integer 2D position coordinates
    const math::Vector2<int> iV0(v0.position.x, v0.position.y);
    const math::Vector2<int> iV1(v1.position.x, v1.position.y);
    const math::Vector2<int> iV2(v2.position.x, v2.position.y);

    // Check if vertices are in clockwise order or degenerate, in which case the triangle cannot be rendered
    if ((iV1.x - iV0.x) * (iV2.y - iV0.y) - (iV2.x - iV0.x) * (iV1.y - iV0.y) >= 0.0f) return;

    // Calculate the 2D bounding box of the triangle clamped to the viewport dimensions
    const math::Vector2<Uint16> min = iV0.Min(iV1.Min(iV2)).Clamp(
        viewport.GetPosition(), viewport.GetSize());

    const math::Vector2<Uint16> max = iV0.Max(iV1.Max(iV2)).Clamp(
        viewport.GetPosition(), viewport.GetSize());

    // If triangle is entirely outside the viewport we can stop now
    if (min == max) return;

    // Calculate original edge weights relative to bounds.min
    // Will be used to obtain barycentric coordinates by incrementing then averaging them
    int w0Row = (min.x - iV1.x) * (iV2.y - iV1.y) - (iV2.x - iV1.x) * (min.y - iV1.y);
    int w1Row = (min.x - iV2.x) * (iV0.y - iV2.y) - (iV0.x - iV2.x) * (min.y - iV2.y);
    int w2Row = (min.x - iV0.x) * (iV1.y - iV0.y) - (iV1.x - iV0.x) * (min.y - iV0.y);

    // Calculate weight increment steps for each edge
    const math::IVec2 sW0(iV2.y - iV1.y, iV1.x - iV2.x);
    const math::IVec2 sW1(iV0.y - iV2.y, iV2.x - iV0.x);
    const math::IVec2 sW2(iV1.y - iV0.y, iV0.x - iV1.x);

    // Normalize colors for barycentric interpolation
    const math::Vec4 nColV0 = v0.color.Normalized();
    const math::Vec4 nColV1 = v1.color.Normalized();
    const math::Vec4 nColV2 = v2.color.Normalized();

    // Fill the triangle with either color or image based on the provided parameters
    for (Uint16 y = min.y; y <= max.y; y++)
    {
        const Uint32 yOffset = y * framebuffer.GetWidth();
        int w0 = w0Row, w1 = w1Row, w2 = w2Row;

        for (Uint16 x = min.x; x <= max.x; x++)
        {
            if ((w0 | w1 | w2) >= 0)
            {
                const Uint32 xyOffset = yOffset + x;
                const float invSum = 1.0f / (w0 + w1 + w2);
                const float aW0 = w0 * invSum, aW1 = w1 * invSum, aW2 = w2 * invSum;

                if (!depthTest || framebuffer.SetDepthUnsafe(xyOffset, v0.position.z * aW0 + v1.position.z * aW1 + v2.position.z * aW2))
                {
                    const Uint32 byteOffset = xyOffset * framebuffer.GetBytesPerPixel();

                    gfx::Color out = shader->Fragment(math::IVec2(x, y),
                        { 0, 0, 1 }, nColV0 * aW0 + nColV1 * aW1 + nColV2 * aW2);

                    if (out.a && out.a != 255)
                    {
                        const gfx::Color dst = framebuffer.GetPixelUnsafe(byteOffset);
                        const Uint16 alpha = static_cast<Uint16>(out.a) + 1;
                        const Uint16 invAlpha = 256 - alpha;

                        out.a = static_cast<Uint8>((alpha * 256 + dst.a * invAlpha) >> 8);
                        out.r = static_cast<Uint8>((out.r * alpha + dst.r * invAlpha) >> 8);
                        out.g = static_cast<Uint8>((out.g * alpha + dst.g * invAlpha) >> 8);
                        out.b = static_cast<Uint8>((out.b * alpha + dst.b * invAlpha) >> 8);
                    }

                    if (out.a)
                    {
                        framebuffer.SetPixelUnsafe(byteOffset, out);
                    }
                }
            }

            w0 += sW0.x, w1 += sW1.x, w2 += sW2.x;
        }

        w0Row += sW0.y, w1Row += sW1.y, w2Row += sW2.y;
    }
}

void sr::Pipeline::RasterizeTriangleImage2D(Framebuffer& framebuffer, const _sr_impl::Vertex& v0, const _sr_impl::Vertex& v1, const _sr_impl::Vertex& v2, sr::Shader* shader, const gfx::Surface* image, bool depthTest, const shape2D::Rectangle& viewport)
{
    // Get integer 2D position coordinates
    const math::Vector2<int> iV0(v0.position.x, v0.position.y);
    const math::Vector2<int> iV1(v1.position.x, v1.position.y);
    const math::Vector2<int> iV2(v2.position.x, v2.position.y);

    // Check if vertices are in clockwise order or degenerate, in which case the triangle cannot be rendered
    if ((iV1.x - iV0.x) * (iV2.y - iV0.y) - (iV2.x - iV0.x) * (iV1.y - iV0.y) >= 0.0f) return;

    // Calculate the 2D bounding box of the triangle clamped to the viewport dimensions
    const math::Vector2<Uint16> min = iV0.Min(iV1.Min(iV2)).Clamp(
        viewport.GetPosition(), viewport.GetSize());

    const math::Vector2<Uint16> max = iV0.Max(iV1.Max(iV2)).Clamp(
        viewport.GetPosition(), viewport.GetSize());

    // If triangle is entirely outside the viewport we can stop now
    if (min == max) return;

    // Calculate original edge weights relative to bounds.min
    // Will be used to obtain barycentric coordinates by incrementing then averaging them
    int w0Row = (min.x - iV1.x) * (iV2.y - iV1.y) - (iV2.x - iV1.x) * (min.y - iV1.y);
    int w1Row = (min.x - iV2.x) * (iV0.y - iV2.y) - (iV0.x - iV2.x) * (min.y - iV2.y);
    int w2Row = (min.x - iV0.x) * (iV1.y - iV0.y) - (iV1.x - iV0.x) * (min.y - iV0.y);

    // Calculate weight increment steps for each edge
    const math::IVec2 sW0(iV2.y - iV1.y, iV1.x - iV2.x);
    const math::IVec2 sW1(iV0.y - iV2.y, iV2.x - iV0.x);
    const math::IVec2 sW2(iV1.y - iV0.y, iV0.x - iV1.x);

    // Normalize colors for barycentric interpolation
    const math::Vec4 nColV0 = v0.color.Normalized();
    const math::Vec4 nColV1 = v1.color.Normalized();
    const math::Vec4 nColV2 = v2.color.Normalized();

    // Fill the triangle with either color or image based on the provided parameters
    for (Uint16 y = min.y; y <= max.y; y++)
    {
        const Uint32 yOffset = y * framebuffer.GetWidth();
        int w0 = w0Row, w1 = w1Row, w2 = w2Row;

        for (Uint16 x = min.x; x <= max.x; x++)
        {
            if ((w0 | w1 | w2) >= 0)
            {
                const Uint32 xyOffset = yOffset + x;
                const float invSum = 1.0f / (w0 + w1 + w2);
                const float aW0 = w0 * invSum, aW1 = w1 * invSum, aW2 = w2 * invSum;

                if (!depthTest || framebuffer.SetDepthUnsafe(xyOffset, v0.position.z * aW0 + v1.position.z * aW1 + v2.position.z * aW2))
                {
                    const Uint32 byteOffset = xyOffset * framebuffer.GetBytesPerPixel();

                    gfx::Color out = shader->Fragment(image, math::IVec2(x, y),
                        v0.texcoord * aW0 + v1.texcoord * aW1 + v2.texcoord * aW2,
                        { 0, 0, 1 }, nColV0 * aW0 + nColV1 * aW1 + nColV2 * aW2);

                    if (out.a && out.a != 255)
                    {
                        const gfx::Color dst = framebuffer.GetPixelUnsafe(byteOffset);
                        const Uint16 alpha = static_cast<Uint16>(out.a) + 1;
                        const Uint16 invAlpha = 256 - alpha;

                        out.a = static_cast<Uint8>((alpha * 256 + dst.a * invAlpha) >> 8);
                        out.r = static_cast<Uint8>((out.r * alpha + dst.r * invAlpha) >> 8);
                        out.g = static_cast<Uint8>((out.g * alpha + dst.g * invAlpha) >> 8);
                        out.b = static_cast<Uint8>((out.b * alpha + dst.b * invAlpha) >> 8);
                    }

                    if (out.a)
                    {
                        framebuffer.SetPixelUnsafe(byteOffset, out);
                    }
                }
            }

            w0 += sW0.x, w1 += sW1.x, w2 += sW2.x;
        }

        w0Row += sW0.y, w1Row += sW1.y, w2Row += sW2.y;
    }
}

void sr::Pipeline::RasterizeTriangleColor3D(Framebuffer& framebuffer, const _sr_impl::Vertex& v0, const _sr_impl::Vertex& v1, const _sr_impl::Vertex& v2, sr::Shader* shader, bool depthTest)
{
    // Get integer 2D position coordinates
    const math::Vector2<Uint16> iV0(v0.position.x, v0.position.y);
    const math::Vector2<Uint16> iV1(v1.position.x, v1.position.y);
    const math::Vector2<Uint16> iV2(v2.position.x, v2.position.y);

    // Check if vertices are in clockwise order or degenerate, in which case the triangle cannot be rendered
    if ((iV1.x - iV0.x) * (iV2.y - iV0.y) - (iV2.x - iV0.x) * (iV1.y - iV0.y) >= 0.0f) return;

    // Calculate the 2D bounding box of the triangle
    const math::Vector2<Uint16> min = iV0.Min(iV1.Min(iV2));
    const math::Vector2<Uint16> max = iV0.Max(iV1.Max(iV2));

    // Calculate original edge weights relative to bounds.min
    // Will be used to obtain barycentric coordinates by incrementing then averaging them
    int w0Row = (min.x - iV1.x) * (iV2.y - iV1.y) - (iV2.x - iV1.x) * (min.y - iV1.y);
    int w1Row = (min.x - iV2.x) * (iV0.y - iV2.y) - (iV0.x - iV2.x) * (min.y - iV2.y);
    int w2Row = (min.x - iV0.x) * (iV1.y - iV0.y) - (iV1.x - iV0.x) * (min.y - iV0.y);

    // Calculate weight increment steps for each edge
    const math::IVec2 sW0(iV2.y - iV1.y, iV1.x - iV2.x);
    const math::IVec2 sW1(iV0.y - iV2.y, iV2.x - iV0.x);
    const math::IVec2 sW2(iV1.y - iV0.y, iV0.x - iV1.x);

    // Normalize colors for barycentric interpolation
    const math::Vec4 nColV0 = v0.color.Normalized();
    const math::Vec4 nColV1 = v1.color.Normalized();
    const math::Vec4 nColV2 = v2.color.Normalized();

    // Fill the triangle with either color or image based on the provided parameters
    for (Uint16 y = min.y; y <= max.y; y++)
    {
        const Uint32 yOffset = y * framebuffer.GetWidth();
        int w0 = w0Row, w1 = w1Row, w2 = w2Row;

        for (Uint16 x = min.x; x <= max.x; x++)
        {
            if ((w0 | w1 | w2) >= 0)
            {
                const Uint32 xyOffset = yOffset + x;
                const float invSum = 1.0f / (w0 + w1 + w2);
                const float aW0 = w0 * invSum, aW1 = w1 * invSum, aW2 = w2 * invSum;

                if (!depthTest || framebuffer.SetDepthUnsafe(xyOffset, v0.position.z * aW0 + v1.position.z * aW1 + v2.position.z * aW2))
                {
                    const Uint32 byteOffset = xyOffset * framebuffer.GetBytesPerPixel();

                    gfx::Color out = shader->Fragment(math::IVec2(x, y),
                        v0.normal * aW0 + v1.normal * aW1 + v2.normal * aW2,
                        nColV0 * aW0 + nColV1 * aW1 + nColV2 * aW2);

                    if (out.a && out.a != 255)
                    {
                        const gfx::Color dst = framebuffer.GetPixelUnsafe(byteOffset);
                        const Uint16 alpha = static_cast<Uint16>(out.a) + 1;
                        const Uint16 invAlpha = 256 - alpha;

                        out.a = static_cast<Uint8>((alpha * 256 + dst.a * invAlpha) >> 8);
                        out.r = static_cast<Uint8>((out.r * alpha + dst.r * invAlpha) >> 8);
                        out.g = static_cast<Uint8>((out.g * alpha + dst.g * invAlpha) >> 8);
                        out.b = static_cast<Uint8>((out.b * alpha + dst.b * invAlpha) >> 8);
                    }

                    if (out.a)
                    {
                        framebuffer.SetPixelUnsafe(byteOffset, out);
                    }
                }
            }

            w0 += sW0.x, w1 += sW1.x, w2 += sW2.x;
        }

        w0Row += sW0.y, w1Row += sW1.y, w2Row += sW2.y;
    }
}

void sr::Pipeline::RasterizeTriangleImage3D(Framebuffer& framebuffer, const _sr_impl::Vertex& v0, const _sr_impl::Vertex& v1, const _sr_impl::Vertex& v2, sr::Shader* shader, const gfx::Surface* image, bool depthTest)
{
    // Get integer 2D position coordinates
    const math::Vector2<Uint16> iV0(v0.position.x, v0.position.y);
    const math::Vector2<Uint16> iV1(v1.position.x, v1.position.y);
    const math::Vector2<Uint16> iV2(v2.position.x, v2.position.y);

    // Check if vertices are in clockwise order or degenerate, in which case the triangle cannot be rendered
    if ((iV1.x - iV0.x) * (iV2.y - iV0.y) - (iV2.x - iV0.x) * (iV1.y - iV0.y) >= 0.0f) return;

    // Calculate the 2D bounding box of the triangle
    const math::Vector2<Uint16> min = iV0.Min(iV1.Min(iV2));
    const math::Vector2<Uint16> max = iV0.Max(iV1.Max(iV2));

    // Calculate original edge weights relative to bounds.min
    // Will be used to obtain barycentric coordinates by incrementing then averaging them
    int w0Row = (min.x - iV1.x) * (iV2.y - iV1.y) - (iV2.x - iV1.x) * (min.y - iV1.y);
    int w1Row = (min.x - iV2.x) * (iV0.y - iV2.y) - (iV0.x - iV2.x) * (min.y - iV2.y);
    int w2Row = (min.x - iV0.x) * (iV1.y - iV0.y) - (iV1.x - iV0.x) * (min.y - iV0.y);

    // Calculate weight increment steps for each edge
    const math::IVec2 sW0(iV2.y - iV1.y, iV1.x - iV2.x);
    const math::IVec2 sW1(iV0.y - iV2.y, iV2.x - iV0.x);
    const math::IVec2 sW2(iV1.y - iV0.y, iV0.x - iV1.x);

    // Normalize colors for barycentric interpolation
    const math::Vec4 nColV0 = v0.color.Normalized();
    const math::Vec4 nColV1 = v1.color.Normalized();
    const math::Vec4 nColV2 = v2.color.Normalized();

    // Fill the triangle with either color or image based on the provided parameters
    for (Uint16 y = min.y; y <= max.y; y++)
    {
        const Uint32 yOffset = y * framebuffer.GetWidth();
        int w0 = w0Row, w1 = w1Row, w2 = w2Row;

        for (Uint16 x = min.x; x <= max.x; x++)
        {
            if ((w0 | w1 | w2) >= 0)
            {
                const Uint32 xyOffset = yOffset + x;
                const float invSum = 1.0f / (w0 + w1 + w2);
                const float aW0 = w0 * invSum, aW1 = w1 * invSum, aW2 = w2 * invSum;
                const float z = v0.position.z * aW0 + v1.position.z * aW1 + v2.position.z * aW2;

                if (!depthTest || framebuffer.SetDepthUnsafe(xyOffset, z))
                {
                    const Uint32 byteOffset = xyOffset * framebuffer.GetBytesPerPixel();

                    const math::Vec2 correctPerspectiveUV = (
                        v0.texcoord / v0.position.z * aW0 +
                        v1.texcoord / v1.position.z * aW1 +
                        v2.texcoord / v0.position.z * aW2
                    ) * (1.0f / z);

                    gfx::Color out = shader->Fragment(image,
                        math::IVec2(x, y), correctPerspectiveUV,
                        v0.normal * aW0 + v1.normal * aW1 + v2.normal * aW2,
                        nColV0 * aW0 + nColV1 * aW1 + nColV2 * aW2);

                    if (out.a && out.a != 255)
                    {
                        const gfx::Color dst = framebuffer.GetPixelUnsafe(byteOffset);
                        const Uint16 alpha = static_cast<Uint16>(out.a) + 1;
                        const Uint16 invAlpha = 256 - alpha;

                        out.a = static_cast<Uint8>((alpha * 256 + dst.a * invAlpha) >> 8);
                        out.r = static_cast<Uint8>((out.r * alpha + dst.r * invAlpha) >> 8);
                        out.g = static_cast<Uint8>((out.g * alpha + dst.g * invAlpha) >> 8);
                        out.b = static_cast<Uint8>((out.b * alpha + dst.b * invAlpha) >> 8);
                    }

                    if (out.a)
                    {
                        framebuffer.SetPixelUnsafe(byteOffset, out);
                    }
                }
            }

            w0 += sW0.x, w1 += sW1.x, w2 += sW2.x;
        }

        w0Row += sW0.y, w1Row += sW1.y, w2Row += sW2.y;
    }
}

/* Public Implementation Pipeline */

void sr::Pipeline::Reset()
{
    vertexCounter = 0;
}

bool sr::Pipeline::AddVertex(DrawMode mode, const math::Vec3& position, const math::Vec3& normal, const math::Vec2& texcoord, const gfx::Color& color)
{
    if (mode != this->mode)
    {
        this->mode = mode;
        this->Reset();
    }

    vertices[vertexCounter++] = {
        position, normal, texcoord, color
    };

    return vertexCounter == static_cast<int>(mode);
}

void sr::Pipeline::ProcessAndRender(Framebuffer& framebuffer, const math::Mat4& mvp, const shape2D::Rectangle& viewport, Shader* shader, const gfx::Surface* image, bool depthTest)
{
    switch (mode)
    {
        case DrawMode::Lines:
        {
            Uint8 processedCounter = 2;
            std::array<_sr_impl::Vertex, 2> processed = { vertices[0], vertices[1] };
            ProjectAndClipLine(processed, processedCounter, mvp, viewport, shader);

            if (processedCounter == 2)
            {
                RasterizeLine(framebuffer, processed[0], processed[1], depthTest);
            }
        }
        break;

        case DrawMode::Triangles:
        {
            bool is2D = false;
            Uint8 processedCounter = 3;
            std::array<_sr_impl::Vertex, 12> processed = { vertices[0], vertices[1], vertices[2] };
            ProjectAndClipTriangle(processed, processedCounter, mvp, viewport, shader, is2D);

            if (!image)
            {
                if (is2D) for (Sint8 i = 0; i < processedCounter - 2; i++) RasterizeTriangleColor2D(framebuffer, processed[0], processed[i + 1], processed[i + 2], shader, depthTest, viewport);
                else for (Sint8 i = 0; i < processedCounter - 2; i++) RasterizeTriangleColor3D(framebuffer, processed[0], processed[i + 1], processed[i + 2], shader, depthTest);
            }
            else
            {
                if (is2D) for (Sint8 i = 0; i < processedCounter - 2; i++) RasterizeTriangleImage2D(framebuffer, processed[0], processed[i + 1], processed[i + 2], shader, image, depthTest, viewport);
                else for (Sint8 i = 0; i < processedCounter - 2; i++) RasterizeTriangleImage3D(framebuffer, processed[0], processed[i + 1], processed[i + 2], shader, image, depthTest);
            }
        }
        break;

        case DrawMode::Quads:
        {
            if (!image)
            {
                for (int i = 0; i < 2; i++)
                {
                    bool is2D = false;
                    Uint8 processedCounter = 3;
                    std::array<_sr_impl::Vertex, 12> processed = { vertices[0], vertices[i + 1], vertices[i + 2] };
                    ProjectAndClipTriangle(processed, processedCounter, mvp, viewport, shader, is2D);

                    if (is2D) for (Sint8 j = 0; j < processedCounter - 2; j++) RasterizeTriangleColor2D(framebuffer, processed[0], processed[j + 1], processed[j + 2], shader, depthTest, viewport);
                    else for (Sint8 j = 0; j < processedCounter - 2; j++) RasterizeTriangleColor3D(framebuffer, processed[0], processed[j + 1], processed[j + 2], shader, depthTest);
                }
            }
            else
            {
                for (int i = 0; i < 2; i++)
                {
                    bool is2D = false;
                    Uint8 processedCounter = 3;
                    std::array<_sr_impl::Vertex, 12> processed = { vertices[0], vertices[i + 1], vertices[i + 2] };
                    ProjectAndClipTriangle(processed, processedCounter, mvp, viewport, shader, is2D);

                    if (is2D) for (Sint8 j = 0; j < processedCounter - 2; j++) RasterizeTriangleImage2D(framebuffer, processed[0], processed[j + 1], processed[j + 2], shader, image, depthTest, viewport);
                    else for (Sint8 j = 0; j < processedCounter - 2; j++) RasterizeTriangleImage3D(framebuffer, processed[0], processed[j + 1], processed[j + 2], shader, image, depthTest);
                }
            }
        }
        break;
    }

    vertexCounter = 0;
}
