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

#include "gfx/ext_gfx/nxRenderer.hpp"

#include "gfx/ext_gfx/nxTexture.hpp"
#include "shape/2D/nxTriangle.hpp"
#include "core/nxException.hpp"
#include "shape/2D/nxAABB.hpp"

using namespace nexus;

gfx::Renderer::Renderer(core::Window& window, int index, RendererFlag flags)
{
    renderer = SDL_CreateRenderer(window, index, static_cast<Uint32>(flags));

    if (renderer == nullptr)
    {
        throw core::NexusException("Renderer", "Failed to create renderer.",
                                    "SDL", SDL_GetError());
    }
}

gfx::Renderer::Renderer(Surface& surface)
{
    renderer = SDL_CreateSoftwareRenderer(surface);

    if (renderer == nullptr)
    {
        throw core::NexusException("Renderer", "Failed to create renderer.",
                                    "SDL", SDL_GetError());
    }
}

gfx::Renderer::~Renderer()
{
    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
}

gfx::RendererInfo gfx::Renderer::GetInfo() const
{
    RendererInfo info;

    if (SDL_GetRendererInfo(renderer, &info) < 0)
    {
        throw core::NexusException("Renderer", "Failed to get renderer info.",
                                    "SDL", SDL_GetError());
    }

    return info;
}

void gfx::Renderer::GetInfo(RendererInfo& info) const
{
    if (SDL_GetRendererInfo(renderer, &info) < 0)
    {
        throw core::NexusException("Renderer", "Failed to get renderer info.",
                                    "SDL", SDL_GetError());
    }
}

void gfx::Renderer::GetViewport(shape2D::Rectangle& viewport)
{
    SDL_RenderGetViewport(renderer, &viewport);
}

shape2D::Rectangle gfx::Renderer::GetViewport()
{
    shape2D::Rectangle viewport;
    SDL_RenderGetViewport(renderer, &viewport);
    return viewport;
}

SDL_Texture* gfx::Renderer::GetTarget()
{
    return SDL_GetRenderTarget(renderer);
}

math::IVec2 gfx::Renderer::GetLogicalSize()
{
    int w, h;
    SDL_RenderGetLogicalSize(renderer, &w, &h);
    return math::IVec2(w, h);
}

gfx::Color gfx::Renderer::GetColor()
{
    gfx::Color color;
    SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a);
    return color;
}

gfx::BlendMode gfx::Renderer::GetBlendMode()
{
    SDL_BlendMode blendMode;
    if (SDL_GetRenderDrawBlendMode(renderer, &blendMode) < 0)
    {
        throw core::NexusException("gfx::Renderer", "Unable to get BlendMode.",
            "SDL", SDL_GetError());
    }
    return static_cast<BlendMode>(blendMode);
}

void gfx::Renderer::ReadPixels(const shape2D::Rectangle& rect, gfx::PixelFormat format, void* pixels, int pitch)
{
    SDL_RenderReadPixels(renderer, &rect, static_cast<Uint32>(format), pixels, pitch);
}

void gfx::Renderer::SetViewport(const shape2D::Rectangle& viewport)
{
    SDL_RenderSetViewport(renderer, &viewport);
}

void gfx::Renderer::SetTarget(Texture& texture)
{
    if (SDL_SetRenderTarget(renderer, texture) < 0)
    {
        throw core::NexusException("gfx::Renderer", "Unable to set target texture.",
            "SDL", SDL_GetError());
    }
}

void gfx::Renderer::SetTarget(SDL_Texture* texture)
{
    if (SDL_SetRenderTarget(renderer, texture) < 0)
    {
        throw core::NexusException("gfx::Renderer", "Unable to set target texture.",
            "SDL", SDL_GetError());
    }
}

void gfx::Renderer::SetLogicalSize(int width, int height)
{
    if (SDL_RenderSetLogicalSize(renderer, width, height) != 0)
    {
        throw core::NexusException("Renderer", "Setting logical size of Renderer failed.",
                                    "SDL", SDL_GetError());
    }
}

void gfx::Renderer::SetIntegerScale(bool enable)
{
    SDL_RenderSetIntegerScale(renderer, static_cast<SDL_bool>(enable));
}

void gfx::Renderer::Clear(gfx::Color color)
{
    gfx::Color previous;
    SDL_GetRenderDrawColor(renderer, &previous.r, &previous.g, &previous.b, &previous.a);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, previous.r, previous.g, previous.b, previous.a);
}

void gfx::Renderer::Present()
{
    SDL_RenderPresent(renderer);
}

void gfx::Renderer::SetColor(const gfx::Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void gfx::Renderer::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void gfx::Renderer::SetBlendMode(BlendMode blendMode)
{
    if (SDL_SetRenderDrawBlendMode(renderer, static_cast<SDL_BlendMode>(blendMode)) < 0)
    {
        throw core::NexusException("gfx::Renderer", "Unable to set BlendMode.",
            "SDL", SDL_GetError());
    }
}

void gfx::Renderer::DrawPoint(const math::Vec2& point)
{
    SDL_RenderDrawPoint(renderer, std::round(point.x), std::round(point.y));
}

void gfx::Renderer::DrawPoint(const math::IVec2& point)
{
    SDL_RenderDrawPoint(renderer, point.x, point.y);
}

void gfx::Renderer::DrawPoint(int x, int y)
{
    SDL_RenderDrawPoint(renderer, x, y);
}

void gfx::Renderer::DrawPoints(const std::vector<math::Vec2>& points)
{
    SDL_RenderDrawPointsF(renderer, reinterpret_cast<const SDL_FPoint*>(points.data()), points.size());
}

void gfx::Renderer::DrawPoints(const std::vector<math::IVec2>& points)
{
    SDL_RenderDrawPoints(renderer, reinterpret_cast<const SDL_Point*>(points.data()), points.size());
}

void gfx::Renderer::DrawLine(const math::Vec2& start, const math::Vec2& end)
{
    SDL_RenderDrawLine(renderer, std::round(start.x), std::round(start.y),
                    std::round(end.x), std::round(end.y));
}

void gfx::Renderer::DrawLine(const shape2D::Line& line)
{
    DrawLine(line.start, line.end);
}

void gfx::Renderer::DrawLine(const math::IVec2& start, const math::IVec2& end)
{
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
}

void gfx::Renderer::DrawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void gfx::Renderer::DrawLines(const std::vector<shape2D::Line>& lines)
{
    SDL_RenderDrawLinesF(renderer, reinterpret_cast<const SDL_FPoint*>(lines.data()), 2 * lines.size());
}

void gfx::Renderer::DrawLines(const std::vector<math::Vec2>& points)
{
    SDL_RenderDrawLinesF(renderer, reinterpret_cast<const SDL_FPoint*>(points.data()), points.size());
}

void gfx::Renderer::DrawLines(const std::vector<math::IVec2>& points)
{
    SDL_RenderDrawLines(renderer, reinterpret_cast<const SDL_Point*>(points.data()), points.size());
}

void gfx::Renderer::DrawPolygonLines(const shape2D::Polygon poly)
{
    SDL_RenderDrawLinesF(renderer, reinterpret_cast<const SDL_FPoint*>(poly.vertices.data()), poly.size());

    const shape2D::Line &lastEdge = poly.GetEdge(poly.size()-1);
    SDL_RenderDrawLineF(renderer, lastEdge.start.x, lastEdge.start.y, lastEdge.end.x, lastEdge.end.y);
}

void gfx::Renderer::DrawPolygonLines(const std::vector<math::Vec2>& verts)
{
    SDL_RenderDrawLinesF(renderer, reinterpret_cast<const SDL_FPoint*>(verts.data()), verts.size());

    const math::Vec2 &start(verts.at(verts.size()-1));
    const math::Vec2 &end(verts.at(0));

    SDL_RenderDrawLineF(renderer, start.x, start.y, end.x, end.y);
}

void gfx::Renderer::DrawTriangle(const shape2D::TriangleF& tri)
{
    const gfx::Color col = this->GetColor();

    const SDL_Vertex triangle[3] = {
        { tri.a, col, {} },
        { tri.b, col, {} },
        { tri.c, col, {} }
    };

    SDL_RenderGeometry(renderer, nullptr, triangle, 3, nullptr, 0);
}

void gfx::Renderer::DrawTriangleLines(const shape2D::TriangleF& tri)
{
    const SDL_FPoint triangle[6] = {
        tri.a, tri.b,
        tri.b, tri.c,
        tri.c, tri.a
    };

    SDL_RenderDrawLinesF(renderer, triangle, 6);
}

void gfx::Renderer::DrawTriangle(const shape2D::Vertex& v0, const shape2D::Vertex& v1, const shape2D::Vertex& v2, SDL_Texture* texture)
{
    const SDL_Vertex triangle[3] = { v0, v1, v2 };
    SDL_RenderGeometry(renderer, texture, triangle, 3, nullptr, 0);
}

void gfx::Renderer::DrawQuad(const shape2D::Vertex& v0, const shape2D::Vertex& v1, const shape2D::Vertex& v2, const shape2D::Vertex& v3, SDL_Texture* texture)
{
    const SDL_Vertex verts[6] = { v0, v1, v2, v2, v3, v0 };
    SDL_RenderGeometry(renderer, texture, verts, 6, nullptr, 0);
}

void gfx::Renderer::DrawRectangle(const shape2D::RectangleF& rect)
{
    SDL_RenderFillRectF(renderer, &rect);
}

void gfx::Renderer::DrawRectangle(const shape2D::Rectangle& rect)
{
    SDL_RenderFillRect(renderer, &rect);
}

void gfx::Renderer::DrawRectangle(float x, float y, float w, float h)
{
    const SDL_FRect rect{ x, y, w, h };
    SDL_RenderFillRectF(renderer, &rect);
}

void gfx::Renderer::DrawRectangle(int x, int y, int w, int h)
{
    const SDL_Rect rect{ x, y, w, h };
    SDL_RenderFillRect(renderer, &rect);
}

void gfx::Renderer::DrawRectangles(const std::vector<shape2D::Rectangle>& rects)
{
    SDL_RenderFillRects(renderer, rects.data(), rects.size());
}

void gfx::Renderer::DrawRectangles(const std::vector<shape2D::RectangleF>& rects)
{
    SDL_RenderFillRectsF(renderer, rects.data(), rects.size());
}

void gfx::Renderer::DrawRectangleLines(const shape2D::RectangleF& rect)
{
    SDL_RenderDrawRectF(renderer, &rect);
}

void gfx::Renderer::DrawRectangleLines(const shape2D::Rectangle& rect)
{
    SDL_RenderDrawRect(renderer, &rect);
}

void gfx::Renderer::DrawRectangleLines(float x, float y, float w, float h)
{
    const SDL_FRect rect{ x, y, w, h };
    SDL_RenderDrawRectF(renderer, &rect);
}

void gfx::Renderer::DrawRectangleLines(int x, int y, int w, int h)
{
    const SDL_Rect rect{ x, y, w, h };
    SDL_RenderDrawRect(renderer, &rect);
}

void gfx::Renderer::DrawAABB(const shape2D::AABB& aabb)
{
    DrawRectangleLines(shape2D::RectangleF((aabb.min + aabb.max) * 0.5f, aabb.max - aabb.min));
}

void gfx::Renderer::DrawRectanglesLines(const std::vector<shape2D::Rectangle>& rects)
{
    SDL_RenderDrawRects(renderer, rects.data(), rects.size());
}

void gfx::Renderer::DrawRectanglesLines(const std::vector<shape2D::RectangleF>& rects)
{
    SDL_RenderDrawRectsF(renderer, rects.data(), rects.size());
}

void gfx::Renderer::DrawCircle(float cx, float cy, float radius)
{
    math::IVec2 offset(0, radius);
    int d = radius - 1;

    while (offset.y >= offset.x)
    {
        SDL_RenderDrawLine(renderer, cx - offset.y, cy + offset.x, cx + offset.y, cy + offset.x);
        SDL_RenderDrawLine(renderer, cx - offset.x, cy + offset.y, cx + offset.x, cy + offset.y);
        SDL_RenderDrawLine(renderer, cx - offset.x, cy - offset.y, cx + offset.x, cy - offset.y);
        SDL_RenderDrawLine(renderer, cx - offset.y, cy - offset.x, cx + offset.y, cy - offset.x);

        if (d >= 2 * offset.x)
        {
            d -= 2 * offset.x + 1;
            offset.x += 1;
        }
        else if (d < 2 * (radius - offset.y))
        {
            d += 2 * offset.y - 1;
            offset.y -= 1;
        }
        else
        {
            d += 2 * (offset.y - offset.x - 1);
            offset.y -= 1, offset.x += 1;
        }
    }
}

void gfx::Renderer::DrawCircle(const shape2D::Circle& circle)
{
    DrawCircle(circle.center.x, circle.center.y, circle.radius);
}

void gfx::Renderer::DrawCircle(const math::Vec2& center, float radius)
{
    DrawCircle(center.x, center.y, radius);
}

void gfx::Renderer::DrawCircle(const math::IVec2& center, int radius)
{
    DrawCircle(static_cast<float>(center.x), static_cast<float>(center.y), static_cast<float>(radius));
}

void gfx::Renderer::DrawCircle(int x, int y, int radius)
{
    DrawCircle(static_cast<float>(x), static_cast<float>(y), static_cast<float>(radius));
}

void gfx::Renderer::DrawCircleLines(float cx, float cy, float radius)
{
    math::IVec2 offset(0, radius);
    int d = radius - 1;

    while (offset.y >= offset.x)
    {
        SDL_FPoint points[8] = {
            cx + offset.x, cy + offset.y,
            cx + offset.y, cy + offset.x,
            cx - offset.x, cy + offset.y,
            cx - offset.y, cy + offset.x,
            cx + offset.x, cy - offset.y,
            cx + offset.y, cy - offset.x,
            cx - offset.x, cy - offset.y,
            cx - offset.y, cy - offset.x
        };

        SDL_RenderDrawPointsF(renderer, points, 8);

        if (d >= 2*offset.x)
        {
            d -= 2*offset.x + 1;
            offset.x +=1;
        }
        else if (d < 2 * (radius - offset.y))
        {
            d += 2 * offset.y - 1;
            offset.y -= 1;
        }
        else
        {
            d += 2 * (offset.y - offset.x - 1);
            offset.y -= 1, offset.x += 1;
        }
    }
}

void gfx::Renderer::DrawCircleLines(const shape2D::Circle& circle)
{
    DrawCircleLines(circle.center.x, circle.center.y, circle.radius);
}

void gfx::Renderer::DrawCircleLines(const math::Vec2& center, float radius)
{
    DrawCircleLines(center.x, center.y, radius);
}

void gfx::Renderer::DrawCircleLines(const math::IVec2& center, int radius)
{
    DrawCircleLines(static_cast<float>(center.x), static_cast<float>(center.y), static_cast<float>(radius));
}

void gfx::Renderer::DrawCircleLines(int x, int y, int radius)
{
    DrawCircleLines(static_cast<float>(x), static_cast<float>(y), static_cast<float>(radius));
}

void gfx::Renderer::DrawMesh(const shape2D::Mesh& mesh, SDL_Texture* texture)
{
    SDL_RenderGeometry(renderer, texture,
        reinterpret_cast<const ::SDL_Vertex*>(mesh.vertices.data()), mesh.vertices.size(),
        mesh.indices.data(), mesh.indices.size());
}

void gfx::Renderer::DrawGeometry(const std::vector<shape2D::Vertex>& vertices, SDL_Texture* texture)
{
    SDL_RenderGeometry(renderer, texture,
        reinterpret_cast<const ::SDL_Vertex*>(vertices.data()), vertices.size(),
        nullptr, 0);
}
