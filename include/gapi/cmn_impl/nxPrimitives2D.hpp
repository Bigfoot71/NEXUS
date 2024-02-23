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

#ifndef NEXUS_GAPI_IMPL_PRIMITIVES_2D_HPP
#define NEXUS_GAPI_IMPL_PRIMITIVES_2D_HPP

#include "../../shape/2D/nxRectangle.hpp"
#include "../../shape/2D/nxTriangle.hpp"
#include "../../shape/2D/nxPolygon.hpp"
#include "../../shape/2D/nxEllipse.hpp"
#include "../../shape/2D/nxCircle.hpp"
#include "../../shape/2D/nxLine.hpp"
#include "../../shape/2D/nxAABB.hpp"
#include "../../shape/2D/nxMesh.hpp"
#include "../../gfx/nxColor.hpp"
#include "../../math/nxMath.hpp"
#include "../../math/nxEase.hpp"
#include "../../math/nxVec2.hpp"
#include "./nxContext.hpp"
#include <type_traits>

#if SUPPORT_OPENGL
#   include "../gl/nxTexture.hpp"
#endif

#if SUPPORT_SOFTWARE_RASTERIZER
#   include "../sr/nxTexture.hpp"
#endif

// TODO: Find a solution so that the IF_GAPI_IS condition can be evaluated at compile time.
//       If the solution involves making gapi::Context a template, the implicit conversion
//       from App to Context and from Window to Context must still work with these functions.
//
// NOTE: I kept a copy where I attempted to resolve this issue with CRTP, and it worked,
//       but the implicit conversion from Window and App to Context no longer worked due to the template.

#define IF_GAPI_IS(gapi_ns) if (dynamic_cast<nexus::gapi_ns::Context*>(&ctx))
#define ELIF_GAPI_IS(gapi_ns) else IF_GAPI_IS(gapi_ns)

namespace _gapi_primitives_2d_impl {

    template <typename T_Texture>
    inline void SetTexture(nexus::gapi::Context& ctx, const T_Texture* texture)
    {
        IF_GAPI_IS(gl)
        {
            static_cast<nexus::gl::Context&>(ctx).SetTexture(texture ? (*texture)->GetID()
                : static_cast<nexus::gl::Context&>(ctx).GetTextureIdDefault());
        }
        ELIF_GAPI_IS(sr)
        {
            static_cast<nexus::sr::Context&>(ctx).SetTexture(texture);
        }
    }

}

namespace _gapi_primitives_2d {

    constexpr int SplineSegmentDivisions = 24;
    constexpr float SmoothCircleErrorRate = 0.5f;

    constexpr int CircleSegments = 36;
    constexpr float CircleSegStepLen = nexus::math::Tau / CircleSegments;

    /**
     * @brief Draws a pixel on the screen.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param color The color of the pixel.
     */
    NEXUS_API void DrawPixel(nexus::gapi::Context& ctx, float x, float y, const nexus::gfx::Color& color);

    /**
     * @brief Draws a pixel on the screen.
     * 
     * @param ctx The rendering context.
     * @param position The position of the pixel.
     * @param color The color of the pixel.
     */
    NEXUS_API inline void DrawPixel(nexus::gapi::Context& ctx, const nexus::math::Vec2& position, const nexus::gfx::Color& color)
    {
        DrawPixel(ctx, position.x, position.y, color);
    }

    /**
     * @brief Draws a line between two points on the screen.
     * 
     * @param ctx The rendering context.
     * @param x1 The x-coordinate of the starting point.
     * @param y1 The y-coordinate of the starting point.
     * @param x2 The x-coordinate of the ending point.
     * @param y2 The y-coordinate of the ending point.
     * @param color The color of the line.
     */
    NEXUS_API void DrawLine(nexus::gapi::Context& ctx, float x1, float y1, float x2, float y2, const nexus::gfx::Color& color);

    /**
     * @brief Draws a line between two points on the screen.
     * 
     * @param ctx The rendering context.
     * @param a The starting point of the line.
     * @param b The ending point of the line.
     * @param color The color of the line.
     */
    NEXUS_API inline void DrawLine(nexus::gapi::Context& ctx, const nexus::math::Vec2& a, const nexus::math::Vec2& b, const nexus::gfx::Color& color)
    {
        DrawLine(ctx, a.x, a.y, b.x, b.y, color);
    }

    /**
     * @brief Draws a line defined by a Line object on the screen.
     * 
     * @param ctx The rendering context.
     * @param line The Line object defining the line.
     * @param color The color of the line.
     */
    NEXUS_API inline void DrawLine(nexus::gapi::Context& ctx, const nexus::shape2D::Line& line, const nexus::gfx::Color& color)
    {
        DrawLine(ctx, line.start, line.end, color);
    }

    /**
     * @brief Draws a line between two points on the screen with a specified thickness.
     * 
     * @param ctx The rendering context.
     * @param a The starting point of the line.
     * @param b The ending point of the line.
     * @param thick The thickness of the line.
     * @param color The color of the line.
     */
    NEXUS_API void DrawLine(nexus::gapi::Context& ctx, const nexus::math::Vec2& a, const nexus::math::Vec2& b, float thick, const nexus::gfx::Color& color);

    /**
     * @brief Draws a line defined by a Line object on the screen with a specified thickness.
     * 
     * @param ctx The rendering context.
     * @param line The Line object defining the line.
     * @param thick The thickness of the line.
     * @param color The color of the line.
     */
    NEXUS_API inline void DrawLine(nexus::gapi::Context& ctx, const nexus::shape2D::Line& line, float thick, const nexus::gfx::Color& color)
    {
        DrawLine(ctx, line.start, line.end, thick, color);
    }

    /**
     * @brief Draws a series of connected lines (line strip) on the screen.
     * 
     * @param ctx The rendering context.
     * @param points An array of points defining the line strip.
     * @param count The number of points in the line strip.
     * @param color The color of the line strip.
     */
    NEXUS_API void DrawLineStrip(nexus::gapi::Context& ctx, const nexus::math::Vec2* points, std::size_t count, const nexus::gfx::Color& color);

    /**
     * @brief Draws a series of connected lines (line strip) on the screen.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points defining the line strip.
     * @param color The color of the line strip.
     */
    NEXUS_API inline void DrawLineStrip(nexus::gapi::Context& ctx, const std::vector<nexus::math::Vec2>& points, const nexus::gfx::Color& color)
    {
        DrawLineStrip(ctx, points.data(), points.size(), color);
    }

    /**
     * @brief Draws a series of connected lines (line strip) defined by an array of Line objects on the screen.
     * 
     * @param ctx The rendering context.
     * @param lines An array of Line objects defining the line strip.
     * @param count The number of Line objects in the line strip.
     * @param color The color of the line strip.
     */
    NEXUS_API void DrawLineStrip(nexus::gapi::Context& ctx, const nexus::shape2D::Line* lines, std::size_t count, const nexus::gfx::Color& color);

    /**
     * @brief Draws a series of connected lines (line strip) defined by a vector of Line objects on the screen.
     * 
     * @param ctx The rendering context.
     * @param lines A vector of Line objects defining the line strip.
     * @param color The color of the line strip.
     */
    NEXUS_API inline void DrawLineStrip(nexus::gapi::Context& ctx, const std::vector<nexus::shape2D::Line>& lines, const nexus::gfx::Color& color)
    {
        DrawLineStrip(ctx, lines.data(), lines.size(), color);
    }

    /**
     * @brief Draws a Bezier curve between two points on the screen with a specified thickness.
     * 
     * @param ctx The rendering context.
     * @param start The starting point of the Bezier curve.
     * @param end The ending point of the Bezier curve.
     * @param thick The thickness of the Bezier curve.
     * @param color The color of the Bezier curve.
     */
    NEXUS_API void DrawLineBezier(nexus::gapi::Context& ctx, const nexus::math::Vec2& start, const nexus::math::Vec2& end, float thick, const nexus::gfx::Color& color);

    /**
     * @brief Draws a circle on the screen with a specified radius and center coordinates.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the center of the circle.
     * @param y The y-coordinate of the center of the circle.
     * @param radius The radius of the circle.
     * @param color The color of the circle.
     */
    NEXUS_API void DrawCircle(nexus::gapi::Context& ctx, float x, float y, float radius, const nexus::gfx::Color& color);

    /**
     * @brief Draws a circle on the screen with a specified radius and center coordinates.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the circle.
     * @param radius The radius of the circle.
     * @param color The color of the circle.
     */
    NEXUS_API inline void DrawCircle(nexus::gapi::Context& ctx, const nexus::math::Vec2& center, float radius, const nexus::gfx::Color& color)
    {
        DrawCircle(ctx, center.x, center.y, radius, color);
    }

    /**
     * @brief Draws a circle on the screen defined by a Circle object with a specified color.
     * 
     * @param ctx The rendering context.
     * @param circle The Circle object defining the circle.
     * @param color The color of the circle.
     */
    NEXUS_API inline void DrawCircle(nexus::gapi::Context& ctx, const nexus::shape2D::Circle& circle, const nexus::gfx::Color& color)
    {
        DrawCircle(ctx, circle.center.x, circle.center.y, circle.radius, color);
    }

    /**
     * @brief Draws a gradient-filled circle on the screen with a specified radius and center coordinates.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the center of the circle.
     * @param y The y-coordinate of the center of the circle.
     * @param radius The radius of the circle.
     * @param color1 The inner color of the circle gradient.
     * @param color2 The outer color of the circle gradient.
     */
    NEXUS_API void DrawCircleGradient(nexus::gapi::Context& ctx, float x, float y, float radius, const nexus::gfx::Color& color1, const nexus::gfx::Color& color2);

    /**
     * @brief Draws a gradient-filled circle on the screen with a specified radius and center coordinates.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the circle.
     * @param radius The radius of the circle.
     * @param color1 The inner color of the circle gradient.
     * @param color2 The outer color of the circle gradient.
     */
    NEXUS_API inline void DrawCircleGradient(nexus::gapi::Context& ctx, const nexus::math::Vec2& center, float radius, const nexus::gfx::Color& color1, const nexus::gfx::Color& color2)
    {
        DrawCircleGradient(ctx, center.x, center.y, radius, color1, color2);
    }

    /**
     * @brief Draws a gradient-filled circle on the screen defined by a Circle object with a specified inner and outer color.
     * 
     * @param ctx The rendering context.
     * @param circle The Circle object defining the circle.
     * @param color1 The inner color of the circle gradient.
     * @param color2 The outer color of the circle gradient.
     */
    NEXUS_API inline void DrawCircleGradient(nexus::gapi::Context& ctx, const nexus::shape2D::Circle& circle, const nexus::gfx::Color& color1, const nexus::gfx::Color& color2)
    {
        DrawCircleGradient(ctx, circle.center, circle.radius, color1, color2);
    }

    /**
     * @brief Draws the outline of a circle on the screen with a specified radius and center coordinates.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the center of the circle.
     * @param y The y-coordinate of the center of the circle.
     * @param radius The radius of the circle.
     * @param color The color of the circle outline.
     */
    NEXUS_API void DrawCircleLines(nexus::gapi::Context& ctx, float x, float y, float radius, const nexus::gfx::Color& color);

    /**
     * @brief Draws the outline of a circle on the screen with a specified radius and center coordinates.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the circle.
     * @param radius The radius of the circle.
     * @param color The color of the circle outline.
     */
    NEXUS_API inline void DrawCircleLines(nexus::gapi::Context& ctx, const nexus::math::Vec2& center, float radius, const nexus::gfx::Color& color)
    {
        DrawCircleLines(ctx, center.x, center.y, radius, color);
    }

    /**
     * @brief Draws the outline of a circle on the screen defined by a Circle object with a specified color.
     * 
     * @param ctx The rendering context.
     * @param circle The Circle object defining the circle.
     * @param color The color of the circle outline.
     */
    NEXUS_API inline void DrawCircleLines(nexus::gapi::Context& ctx, const nexus::shape2D::Circle& circle, const nexus::gfx::Color& color)
    {
        DrawCircleLines(ctx, circle.center.x, circle.center.y, circle.radius, color);
    }

    /**
     * @brief Draws a sector (portion) of a circle on the screen defined by its center, radius, start and end angles, and the number of segments, with a specified color.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the circle.
     * @param radius The radius of the circle.
     * @param startAngle The starting angle of the sector (in degrees).
     * @param endAngle The ending angle of the sector (in degrees).
     * @param segments The number of segments to approximate the sector.
     * @param color The color of the sector.
     */
    NEXUS_API void DrawCircleSector(nexus::gapi::Context& ctx, const nexus::math::Vec2& center, float radius, float startAngle, float endAngle, int segments, const nexus::gfx::Color& color);

    /**
     * @brief Draws a sector (portion) of a circle on the screen defined by a Circle object, start and end angles, and the number of segments, with a specified color.
     * 
     * @param ctx The rendering context.
     * @param circle The Circle object defining the circle.
     * @param startAngle The starting angle of the sector (in degrees).
     * @param endAngle The ending angle of the sector (in degrees).
     * @param segments The number of segments to approximate the sector.
     * @param color The color of the sector.
     */
    NEXUS_API inline void DrawCircleSector(nexus::gapi::Context& ctx, const nexus::shape2D::Circle& circle, float startAngle, float endAngle, int segments, const nexus::gfx::Color& color)
    {
        DrawCircleSector(ctx, circle.center, circle.radius, startAngle, endAngle, segments, color);
    }

    /**
     * @brief Draws the outline of a sector (portion) of a circle on the screen defined by its center, radius, start and end angles, and the number of segments, with a specified color.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the circle.
     * @param radius The radius of the circle.
     * @param startAngle The starting angle of the sector (in degrees).
     * @param endAngle The ending angle of the sector (in degrees).
     * @param segments The number of segments to approximate the sector.
     * @param color The color of the sector outline.
     */
    NEXUS_API void DrawCircleSectorLines(nexus::gapi::Context& ctx, const nexus::math::Vec2& center, float radius, float startAngle, float endAngle, int segments, const nexus::gfx::Color& color);

    /**
     * @brief Draws the outline of a sector (portion) of a circle on the screen defined by a Circle object, start and end angles, and the number of segments, with a specified color.
     * 
     * @param ctx The rendering context.
     * @param circle The Circle object defining the circle.
     * @param startAngle The starting angle of the sector (in degrees).
     * @param endAngle The ending angle of the sector (in degrees).
     * @param segments The number of segments to approximate the sector.
     * @param color The color of the sector outline.
     */
    NEXUS_API inline void DrawCircleSectorLines(nexus::gapi::Context& ctx, const nexus::shape2D::Circle& circle, float startAngle, float endAngle, int segments, const nexus::gfx::Color& color)
    {
        DrawCircleSectorLines(ctx, circle.center, circle.radius, startAngle, endAngle, segments, color);
    }

    /**
     * @brief Draws an ellipse on the screen with specified center coordinates and radii.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the center of the ellipse.
     * @param y The y-coordinate of the center of the ellipse.
     * @param rx The x-radius of the ellipse.
     * @param ry The y-radius of the ellipse.
     * @param color The color of the ellipse.
     */
    NEXUS_API void DrawEllipse(nexus::gapi::Context& ctx, float x, float y, float rx, float ry, const nexus::gfx::Color& color);

    /**
     * @brief Draws an ellipse on the screen with specified center coordinates and radii.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the ellipse.
     * @param rx The x-radius of the ellipse.
     * @param ry The y-radius of the ellipse.
     * @param color The color of the ellipse.
     */
    NEXUS_API inline void DrawEllipse(nexus::gapi::Context& ctx, const nexus::math::Vec2& center, float rx, float ry, const nexus::gfx::Color& color)
    {
        DrawEllipse(ctx, center.x, center.y, rx, ry, color);
    }

    /**
     * @brief Draws an ellipse on the screen with specified center coordinates and radii.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the ellipse.
     * @param radius The x and y radii of the ellipse.
     * @param color The color of the ellipse.
     */
    NEXUS_API inline void DrawEllipse(nexus::gapi::Context& ctx, const nexus::math::Vec2& center, const nexus::math::Vec2& radius, const nexus::gfx::Color& color)
    {
        DrawEllipse(ctx, center.x, center.y, radius.x, radius.y, color);
    }

    /**
     * @brief Draws an ellipse on the screen defined by an Ellipse object with a specified color.
     * 
     * @param ctx The rendering context.
     * @param ellipse The Ellipse object defining the ellipse.
     * @param color The color of the ellipse.
     */
    NEXUS_API inline void DrawEllipse(nexus::gapi::Context& ctx, const nexus::shape2D::Ellipse& ellipse, const nexus::gfx::Color& color)
    {
        DrawEllipse(ctx, ellipse.center.x, ellipse.center.y, ellipse.radius.x, ellipse.radius.y, color);
    }

    /**
     * @brief Draws the outline of an ellipse on the screen with specified center coordinates and radii.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the center of the ellipse.
     * @param y The y-coordinate of the center of the ellipse.
     * @param rx The x-radius of the ellipse.
     * @param ry The y-radius of the ellipse.
     * @param color The color of the ellipse outline.
     */
    NEXUS_API void DrawEllipseLines(nexus::gapi::Context& ctx, int x, int y, float rx, float ry, const nexus::gfx::Color& color);

    /**
     * @brief Draws the outline of an ellipse on the screen with specified center coordinates and radii.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the ellipse.
     * @param rx The x-radius of the ellipse.
     * @param ry The y-radius of the ellipse.
     * @param color The color of the ellipse outline.
     */
    NEXUS_API inline void DrawEllipseLines(nexus::gapi::Context& ctx, const nexus::math::Vec2& center, float rx, float ry, const nexus::gfx::Color& color)
    {
        DrawEllipseLines(ctx, center.x, center.y, rx, ry, color);
    }

    /**
     * @brief Draws the outline of an ellipse on the screen with specified center coordinates and radii.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the ellipse.
     * @param radius The x and y radii of the ellipse.
     * @param color The color of the ellipse outline.
     */
    NEXUS_API inline void DrawEllipseLines(nexus::gapi::Context& ctx, const nexus::math::Vec2& center, const nexus::math::Vec2& radius, const nexus::gfx::Color& color)
    {
        DrawEllipseLines(ctx, center.x, center.y, radius.x, radius.y, color);
    }

    /**
     * @brief Draws the outline of an ellipse on the screen defined by an Ellipse object with a specified color.
     * 
     * @param ctx The rendering context.
     * @param ellipse The Ellipse object defining the ellipse.
     * @param color The color of the ellipse outline.
     */
    NEXUS_API inline void DrawEllipseLines(nexus::gapi::Context& ctx, const nexus::shape2D::Ellipse& ellipse, const nexus::gfx::Color& color)
    {
        DrawEllipseLines(ctx, ellipse.center.x, ellipse.center.y, ellipse.radius.x, ellipse.radius.y, color);
    }

    /**
     * @brief Draws a ring (annulus) on the screen with specified center coordinates, inner and outer radii, start and end angles, and the number of segments, with a specified color.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the ring.
     * @param innerRadius The inner radius of the ring.
     * @param outerRadius The outer radius of the ring.
     * @param startAngle The starting angle of the ring (in degrees).
     * @param endAngle The ending angle of the ring (in degrees).
     * @param segments The number of segments to approximate the ring.
     * @param color The color of the ring.
     */
    NEXUS_API void DrawRing(nexus::gapi::Context& ctx, const nexus::math::Vec2& center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, const nexus::gfx::Color& color);

    /**
     * @brief Draws the outline of a ring (annulus) on the screen with specified center coordinates, inner and outer radii, start and end angles, and the number of segments, with a specified color.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the ring.
     * @param innerRadius The inner radius of the ring.
     * @param outerRadius The outer radius of the ring.
     * @param startAngle The starting angle of the ring (in degrees).
     * @param endAngle The ending angle of the ring (in degrees).
     * @param segments The number of segments to approximate the ring.
     * @param color The color of the ring outline.
     */
    NEXUS_API void DrawRingLines(nexus::gapi::Context& ctx, const nexus::math::Vec2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, const nexus::gfx::Color& color);

    /**
     * @brief Draws a rectangle on the screen with specified parameters: position, size, rotation, and color.
     * 
     * @param ctx The rendering context.
     * @param rec The RectangleF object defining the rectangle (position, size, rotation).
     * @param origin The origin point of the rectangle (default is {0, 0}).
     * @param rotation The rotation angle of the rectangle (default is 0).
     * @param color The color of the rectangle (default is nexus::gfx::White).
     */
    NEXUS_API void DrawRectangle(nexus::gapi::Context& ctx, const nexus::shape2D::RectangleF& rec, const nexus::math::Vec2& origin = { 0, 0 }, float rotation = 0, const nexus::gfx::Color& color = nexus::gfx::White);

    /**
     * @brief Draws a rectangle outline on the screen with specified RectangleF object and color.
     * 
     * @param ctx The rendering context.
     * @param rec The RectangleF object defining the rectangle (position, size, rotation).
     * @param color The color of the rectangle outline.
     */
    NEXUS_API inline void DrawRectangle(nexus::gapi::Context& ctx, const nexus::shape2D::RectangleF& rec, const nexus::gfx::Color& color)
    {
        DrawRectangle(ctx, rec, { 0, 0 }, 0, color);
    }

    /**
     * @brief Draws a rectangle outline on the screen with specified position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param color The color of the rectangle outline.
     */
    NEXUS_API inline void DrawRectangle(nexus::gapi::Context& ctx, float x, float y, float w, float h, const nexus::gfx::Color& color)
    {
        DrawRectangle(ctx, { x, y, w, h }, { 0, 0 }, 0, color);
    }

    /**
     * @brief Draws a rectangle on the screen with specified position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the top-left corner of the rectangle.
     * @param size The size (width and height) of the rectangle.
     * @param color The color of the rectangle.
     */
    NEXUS_API inline void DrawRectangle(nexus::gapi::Context& ctx, const nexus::math::Vec2& position, const nexus::math::Vec2& size, const nexus::gfx::Color& color)
    {
        DrawRectangle(ctx, { position.x, position.y, size.x, size.y }, { 0, 0 }, 0, color);
    }

    /**
     * @brief Draws a rectangle with a gradient fill on the screen with specified RectangleF object and colors for each corner.
     * 
     * @param ctx The rendering context.
     * @param rec The RectangleF object defining the rectangle (position, size, rotation).
     * @param c1 The color of the top-left corner.
     * @param c2 The color of the top-right corner.
     * @param c3 The color of the bottom-right corner.
     * @param c4 The color of the bottom-left corner.
     */
    NEXUS_API void DrawRectangleGradient(nexus::gapi::Context& ctx, const nexus::shape2D::RectangleF& rec, const nexus::gfx::Color& c1, const nexus::gfx::Color& c2, const nexus::gfx::Color& c3, const nexus::gfx::Color& c4);

    /**
     * @brief Draws a rectangle with a gradient fill on the screen with specified position, size, and colors for each corner.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param c1 The color of the top-left corner.
     * @param c2 The color of the top-right corner.
     * @param c3 The color of the bottom-right corner.
     * @param c4 The color of the bottom-left corner.
     */
    NEXUS_API inline void DrawRectangleGradient(nexus::gapi::Context& ctx, float x, float y, float w, float h, const nexus::gfx::Color& c1, const nexus::gfx::Color& c2, const nexus::gfx::Color& c3, const nexus::gfx::Color& c4)
    {
        DrawRectangleGradient(ctx, { x, y, w, h }, c1, c2, c3, c4);
    }

    /**
     * @brief Draws the outline of a rectangle on the screen with specified position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param color The color of the rectangle outline.
     */
    NEXUS_API void DrawRectangleLines(nexus::gapi::Context& ctx, float x, float y, float w, float h, const nexus::gfx::Color& color);

    /**
     * @brief Draws the outline of a rectangle on the screen with specified position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the top-left corner of the rectangle.
     * @param size The size (width and height) of the rectangle.
     * @param color The color of the rectangle outline.
     */
    NEXUS_API inline void DrawRectangleLines(nexus::gapi::Context& ctx, const nexus::math::Vec2& position, const nexus::math::Vec2 size, const nexus::gfx::Color& color)
    {
        DrawRectangleLines(ctx, position.x, position.y, size.x, size.y, color);
    }

    /**
     * @brief Draws the outline of an Axis-Aligned Bounding Box (AABB) on the screen with specified AABB object and color.
     * 
     * @param ctx The rendering context.
     * @param aabb The AABB object defining the AABB (axis-aligned bounding box).
     * @param color The color of the AABB outline.
     */
    NEXUS_API inline void DrawAABB(nexus::gapi::Context& ctx, const nexus::shape2D::AABB& aabb, const nexus::gfx::Color& color)
    {
        DrawRectangleLines(ctx, (aabb.min + aabb.max) * 0.5f, aabb.max - aabb.min, color);
    }

    /**
     * @brief Draws the outline of a rectangle on the screen with specified RectangleF object, line thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param rec The RectangleF object defining the rectangle (position, size, rotation).
     * @param lineThick The thickness of the rectangle outline.
     * @param color The color of the rectangle outline.
     */
    NEXUS_API void DrawRectangleLines(nexus::gapi::Context& ctx, const nexus::shape2D::RectangleF& rec, float lineThick, const nexus::gfx::Color& color);

    /**
     * @brief Draws the outline of a rectangle on the screen with specified position, size, line thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the top-left corner of the rectangle.
     * @param size The size (width and height) of the rectangle.
     * @param lineThick The thickness of the rectangle outline.
     * @param color The color of the rectangle outline.
     */
    NEXUS_API inline void DrawRectangleLines(nexus::gapi::Context& ctx, const nexus::math::Vec2& position, const nexus::math::Vec2 size, float lineThick, const nexus::gfx::Color& color)
    {
        DrawRectangleLines(ctx, { position, size }, lineThick, color);
    }

    /**
     * @brief Draws a rounded rectangle on the screen with specified RectangleF object, roundness, number of segments, and color.
     * 
     * @param ctx The rendering context.
     * @param rec The RectangleF object defining the rectangle (position, size, rotation).
     * @param roundness The radius of the corners of the rounded rectangle.
     * @param segments The number of segments used to approximate the rounded corners.
     * @param color The color of the rounded rectangle.
     */
    NEXUS_API void DrawRectangleRounded(nexus::gapi::Context& ctx, const nexus::shape2D::RectangleF& rec, float roundness, int segments, const nexus::gfx::Color& color);

    /**
     * @brief Draws a rounded rectangle on the screen with specified position, size, roundness, number of segments, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the top-left corner of the rectangle.
     * @param size The size (width and height) of the rectangle.
     * @param roundness The radius of the corners of the rounded rectangle.
     * @param segments The number of segments used to approximate the rounded corners.
     * @param color The color of the rounded rectangle.
     */
    NEXUS_API inline void DrawRectangleRounded(nexus::gapi::Context& ctx, const nexus::math::Vec2& position, const nexus::math::Vec2 size, float roundness, int segments, const nexus::gfx::Color& color)
    {
        DrawRectangleRounded(ctx, { position, size }, roundness, segments, color);
    }

    /**
     * @brief Draws the outline of a rounded rectangle on the screen with specified RectangleF object, roundness, number of segments, line thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param rec The RectangleF object defining the rectangle (position, size, rotation).
     * @param roundness The radius of the corners of the rounded rectangle.
     * @param segments The number of segments used to approximate the rounded corners.
     * @param lineThick The thickness of the rounded rectangle outline.
     * @param color The color of the rounded rectangle outline.
     */
    NEXUS_API void DrawRectangleRoundedLines(nexus::gapi::Context& ctx, const nexus::shape2D::RectangleF& rec, float roundness, int segments, float lineThick, const nexus::gfx::Color& color);

    /**
     * @brief Draws the outline of a rounded rectangle on the screen with specified position, size, roundness, number of segments, line thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the top-left corner of the rectangle.
     * @param size The size (width and height) of the rectangle.
     * @param roundness The radius of the corners of the rounded rectangle.
     * @param segments The number of segments used to approximate the rounded corners.
     * @param lineThick The thickness of the rounded rectangle outline.
     * @param color The color of the rounded rectangle outline.
     */
    NEXUS_API inline void DrawRectangleRoundedLines(nexus::gapi::Context& ctx, const nexus::math::Vec2& position, const nexus::math::Vec2 size, float roundness, int segments, float lineThick, const nexus::gfx::Color& color)
    {
        DrawRectangleRoundedLines(ctx, { position, size }, roundness, segments, lineThick, color);
    }

    /**
     * @brief Draws a triangle on the screen with specified vertices and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The first vertex of the triangle.
     * @param p2 The second vertex of the triangle.
     * @param p3 The third vertex of the triangle.
     * @param color The color of the triangle.
     */
    NEXUS_API void DrawTriangle(nexus::gapi::Context& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& p2, const nexus::math::Vec2& p3, const nexus::gfx::Color& color);

    /**
     * @brief Draws a triangle on the screen with specified TriangleF object and color.
     * 
     * @param ctx The rendering context.
     * @param tri The TriangleF object defining the triangle (vertices).
     * @param color The color of the triangle.
     */
    NEXUS_API inline void DrawTriangle(nexus::gapi::Context& ctx, const nexus::shape2D::TriangleF& tri, const nexus::gfx::Color& color)
    {
        DrawTriangle(ctx, tri.a, tri.b, tri.c, color);
    }

    /**
     * @brief Draws the outline of a triangle on the screen with specified vertices and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The first vertex of the triangle.
     * @param p2 The second vertex of the triangle.
     * @param p3 The third vertex of the triangle.
     * @param color The color of the triangle outline.
     */
    NEXUS_API void DrawTriangleLines(nexus::gapi::Context& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& p2, const nexus::math::Vec2& p3, const nexus::gfx::Color& color);

    /**
     * @brief Draws the outline of a triangle on the screen with specified TriangleF object and color.
     * 
     * @param ctx The rendering context.
     * @param tri The TriangleF object defining the triangle (vertices).
     * @param color The color of the triangle outline.
     */
    NEXUS_API inline void DrawTriangleLines(nexus::gapi::Context& ctx, const nexus::shape2D::TriangleF& tri, const nexus::gfx::Color& color)
    {
        DrawTriangleLines(ctx, tri.a, tri.b, tri.c, color);
    }

    /**
     * @brief Draws a triangle fan on the screen using the provided mesh and optional texture.
     * 
     * @note: This function is available to avoid rendering nexus::shape2D::Mesh useless outside the context of nexus::gfx::Renderer,
     *       it is easier to use than a gapi::Mesh for 2D rendering but is less efficient because it requires
     *       a copy of the vertices in the context's batch render buffer.
     * 
     * @param ctx The rendering context.
     * @param mesh The mesh representing the triangle fan.
     * @param texture Optional texture to apply to the triangle fan (default is nullptr).
     */
    template <typename T_Texture>
    NEXUS_API void DrawTriangleFan(nexus::gapi::Context& ctx, const nexus::shape2D::Mesh& mesh, const T_Texture* texture = nullptr)
    {
        const auto &vertices = mesh.vertices;
        if (vertices.size() < 3) return;

        _gapi_primitives_2d_impl::SetTexture(ctx, texture);

        IF_GAPI_IS(sr)
        {
            ctx.Begin(nexus::gapi::DrawMode::Triangles);

                for (int i = 1; i < vertices.size() - 1; i++)
                {
                    ctx.Color(vertices[0].color);
                    ctx.TexCoord(vertices[0].texcoord);
                    ctx.Vertex(vertices[0].position);

                    ctx.Color(vertices[i].color);
                    ctx.TexCoord(vertices[i].texcoord);
                    ctx.Vertex(vertices[i].position);

                    ctx.Color(vertices[i + 1].color);
                    ctx.TexCoord(vertices[i + 1].texcoord);
                    ctx.Vertex(vertices[i + 1].position);
                }

            ctx.End();
        }
        else
        {
            ctx.Begin(nexus::gapi::DrawMode::Quads);

                for (int i = 1; i < vertices.size() - 1; i++)
                {
                    ctx.Color(vertices[0].color);
                    ctx.TexCoord(vertices[0].texcoord);
                    ctx.Vertex(vertices[0].position);

                    ctx.Color(vertices[i].color);
                    ctx.TexCoord(vertices[i].texcoord);
                    ctx.Vertex(vertices[i].position);

                    ctx.Color(vertices[i + 1].color);
                    ctx.TexCoord(vertices[i + 1].texcoord);
                    ctx.Vertex(vertices[i + 1].position);

                    ctx.Color(vertices[i + 1].color);
                    ctx.TexCoord(vertices[i + 1].texcoord);
                    ctx.Vertex(vertices[i + 1].position);
                }

            ctx.End();
        }

        ctx.UnsetTexture();
    }

    /**
     * @brief Draws a triangle fan on the screen using the provided points and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the triangle fan.
     * @param count The number of points in the array.
     * @param color The color of the triangle fan.
     */
    NEXUS_API void DrawTriangleFan(nexus::gapi::Context& ctx, const nexus::math::Vec2* points, std::size_t count, const nexus::gfx::Color& color);

    /**
     * @brief Draws a triangle fan on the screen using the provided vector of points and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the triangle fan.
     * @param color The color of the triangle fan.
     */
    NEXUS_API inline void DrawTriangleFan(nexus::gapi::Context& ctx, const std::vector<nexus::math::Vec2>& points, const nexus::gfx::Color& color)
    {
        DrawTriangleFan(ctx, points.data(), points.size(), color);
    }

    /**
     * @brief Draws a triangle fan on the screen using the provided vector of triangles and color.
     * 
     * @param ctx The rendering context.
     * @param triangles A vector of triangles representing the vertices of the triangle fan.
     * @param color The color of the triangle fan.
     */
    NEXUS_API inline void DrawTriangleFan(nexus::gapi::Context& ctx, const std::vector<nexus::shape2D::TriangleF>& triangles, const nexus::gfx::Color& color)
    {
        DrawTriangleFan(ctx, reinterpret_cast<const nexus::math::Vec2*>(triangles.data()), 3 * triangles.size(), color);
    }

    /**
     * @brief Draws a triangle strip on the screen using the provided mesh and optional texture.
     * 
     * @note: This function is available to avoid rendering nexus::shape2D::Mesh useless outside the context of nexus::gfx::Renderer,
     *       it is easier to use than a gapi::Mesh for 2D rendering but is less efficient because it requires
     *       a copy of the vertices in the context's batch render buffer.
     * 
     * @param ctx The rendering context.
     * @param mesh The mesh representing the triangle strip.
     * @param texture Optional texture to apply to the triangle strip (default is nullptr).
     */
    template <typename T_Texture>
    NEXUS_API void DrawTriangleStrip(nexus::gapi::Context& ctx, const nexus::shape2D::Mesh& mesh, const T_Texture* texture = nullptr)
    {
        const auto &vertices = mesh.vertices;
        if (vertices.size() < 3) return;

        _gapi_primitives_2d_impl::SetTexture(ctx, texture);

        IF_GAPI_IS(sr)
        {
            ctx.Begin(nexus::gapi::DrawMode::Triangles);

                for (int i = 2; i < vertices.size(); i++)
                {
                    if (i % 2 == 0)
                    {
                        ctx.Color(vertices[i].color);
                        ctx.TexCoord(vertices[i].texcoord);
                        ctx.Vertex(vertices[i].position);

                        ctx.Color(vertices[i - 2].color);
                        ctx.TexCoord(vertices[i - 2].texcoord);
                        ctx.Vertex(vertices[i - 2].position);

                        ctx.Color(vertices[i - 1].color);
                        ctx.TexCoord(vertices[i - 1].texcoord);
                        ctx.Vertex(vertices[i - 1].position);
                    }
                    else
                    {
                        ctx.Color(vertices[i].color);
                        ctx.TexCoord(vertices[i].texcoord);
                        ctx.Vertex(vertices[i].position);

                        ctx.Color(vertices[i - 1].color);
                        ctx.TexCoord(vertices[i - 1].texcoord);
                        ctx.Vertex(vertices[i - 1].position);

                        ctx.Color(vertices[i - 2].color);
                        ctx.TexCoord(vertices[i - 2].texcoord);
                        ctx.Vertex(vertices[i - 2].position);
                    }
                }

            ctx.End();
        }
        else
        {
            ctx.Begin(nexus::gapi::DrawMode::Quads);

                for (int i = 2; i < vertices.size(); i++)
                {
                    if (i % 2 == 0)
                    {
                        ctx.Color(vertices[i].color);
                        ctx.TexCoord(vertices[i].texcoord);
                        ctx.Vertex(vertices[i].position);

                        ctx.Color(vertices[i - 2].color);
                        ctx.TexCoord(vertices[i - 2].texcoord);
                        ctx.Vertex(vertices[i - 2].position);

                        ctx.Color(vertices[i - 1].color);
                        ctx.TexCoord(vertices[i - 1].texcoord);
                        ctx.Vertex(vertices[i - 1].position);

                        ctx.Color(vertices[i - 1].color);
                        ctx.TexCoord(vertices[i - 1].texcoord);
                        ctx.Vertex(vertices[i - 1].position);
                    }
                    else
                    {
                        ctx.Color(vertices[i].color);
                        ctx.TexCoord(vertices[i].texcoord);
                        ctx.Vertex(vertices[i].position);

                        ctx.Color(vertices[i - 1].color);
                        ctx.TexCoord(vertices[i - 1].texcoord);
                        ctx.Vertex(vertices[i - 1].position);

                        ctx.Color(vertices[i - 2].color);
                        ctx.TexCoord(vertices[i - 2].texcoord);
                        ctx.Vertex(vertices[i - 2].position);

                        ctx.Color(vertices[i - 2].color);
                        ctx.TexCoord(vertices[i - 2].texcoord);
                        ctx.Vertex(vertices[i - 2].position);
                    }
                }

            ctx.End();
        }

        ctx.UnsetTexture();
    }

    /**
     * @brief Draws a triangle strip on the screen using the provided points and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the triangle strip.
     * @param count The number of points in the array.
     * @param color The color of the triangle strip.
     */
    NEXUS_API void DrawTriangleStrip(nexus::gapi::Context& ctx, const nexus::math::Vec2* points, std::size_t count, const nexus::gfx::Color& color);

    /**
     * @brief Draws a triangle strip on the screen using the provided vector of points and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the triangle strip.
     * @param color The color of the triangle strip.
     */
    NEXUS_API inline void DrawTriangleStrip(nexus::gapi::Context& ctx, const std::vector<nexus::math::Vec2>& points, const nexus::gfx::Color& color)
    {
        DrawTriangleStrip(ctx, points.data(), points.size(), color);
    }

    /**
     * @brief Draws a triangle strip on the screen using the provided vector of triangles and color.
     * 
     * @param ctx The rendering context.
     * @param triangles A vector of triangles representing the vertices of the triangle strip.
     * @param color The color of the triangle strip.
     */
    NEXUS_API inline void DrawTriangleStrip(nexus::gapi::Context& ctx, const std::vector<nexus::shape2D::TriangleF>& triangles, const nexus::gfx::Color& color)
    {
        DrawTriangleStrip(ctx, reinterpret_cast<const nexus::math::Vec2*>(triangles.data()), 3 * triangles.size(), color);
    }

    /**
     * @brief Draws a regular polygon on the screen with the specified center, number of sides, radius, rotation, and color.
     * 
     * @param ctx The rendering context.
     * @param center The center of the polygon.
     * @param sides The number of sides of the polygon.
     * @param radius The radius of the polygon.
     * @param rotation The rotation angle of the polygon in degrees.
     * @param color The color of the polygon.
     */
    NEXUS_API void DrawPolygon(nexus::gapi::Context& ctx, const nexus::math::Vec2& center, int sides, float radius, float rotation, const nexus::gfx::Color& color);

    /**
     * @brief Draws the outline of a regular polygon on the screen with the specified center, number of sides, radius, rotation, and color.
     * 
     * @param ctx The rendering context.
     * @param center The center of the polygon.
     * @param sides The number of sides of the polygon.
     * @param radius The radius of the polygon.
     * @param rotation The rotation angle of the polygon in degrees.
     * @param color The color of the polygon outline.
     */
    NEXUS_API void DrawPolygonLines(nexus::gapi::Context& ctx, const nexus::math::Vec2& center, int sides, float radius, float rotation, const nexus::gfx::Color& color);

    /**
     * @brief Draws the outline of a regular polygon on the screen with the specified center, number of sides, radius, rotation, line thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param center The center of the polygon.
     * @param sides The number of sides of the polygon.
     * @param radius The radius of the polygon.
     * @param rotation The rotation angle of the polygon in degrees.
     * @param lineThick The thickness of the polygon outline.
     * @param color The color of the polygon outline.
     */
    NEXUS_API void DrawPolygonLines(nexus::gapi::Context& ctx, const nexus::math::Vec2& center, int sides, float radius, float rotation, float lineThick, const nexus::gfx::Color& color);

    /**
     * @brief Draws a linear spline on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the spline.
     * @param count The number of points in the array (minimum 2).
     * @param thick The thickness of the spline.
     * @param color The color of the spline.
     */
    NEXUS_API void DrawSplineLinear(nexus::gapi::Context& ctx, const nexus::math::Vec2* points, std::size_t count, float thick, const nexus::gfx::Color& color);

    /**
     * @brief Draws a linear spline on the screen using the provided vector of points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the spline.
     * @param thick The thickness of the spline.
     * @param color The color of the spline.
     */
    NEXUS_API inline void DrawSplineLinear(nexus::gapi::Context& ctx, const std::vector<nexus::math::Vec2>& points, float thick, const nexus::gfx::Color& color)
    {
        DrawSplineLinear(ctx, points.data(), points.size(), thick, color);
    }

    /**
     * @brief Draws a B-Spline curve on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the B-Spline curve.
     * @param count The number of points in the array (minimum 4).
     * @param thick The thickness of the B-Spline curve.
     * @param color The color of the B-Spline curve.
     */
    NEXUS_API void DrawSplineBasis(nexus::gapi::Context& ctx, const nexus::math::Vec2* points, std::size_t count, float thick, const nexus::gfx::Color& color);

    /**
     * @brief Draws a B-Spline curve on the screen using the provided vector of points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the B-Spline curve.
     * @param thick The thickness of the B-Spline curve.
     * @param color The color of the B-Spline curve.
     */
    NEXUS_API inline void DrawSplineBasis(nexus::gapi::Context& ctx, const std::vector<nexus::math::Vec2>& points, float thick, const nexus::gfx::Color& color)
    {
        DrawSplineBasis(ctx, points.data(), points.size(), thick, color);
    }

    /**
     * @brief Draws a Catmull-Rom spline curve on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the Catmull-Rom spline curve.
     * @param count The number of points in the array (minimum 4).
     * @param thick The thickness of the Catmull-Rom spline curve.
     * @param color The color of the Catmull-Rom spline curve.
     */
    NEXUS_API void DrawSplineCatmullRom(nexus::gapi::Context& ctx, const nexus::math::Vec2* points, std::size_t count, float thick, const nexus::gfx::Color& color);

    /**
     * @brief Draws a Catmull-Rom spline curve on the screen using the provided vector of points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the Catmull-Rom spline curve.
     * @param thick The thickness of the Catmull-Rom spline curve.
     * @param color The color of the Catmull-Rom spline curve.
     */
    NEXUS_API inline void DrawSplineCatmullRom(nexus::gapi::Context& ctx, const std::vector<nexus::math::Vec2> points, float thick, const nexus::gfx::Color& color)
    {
        DrawSplineCatmullRom(ctx, points.data(), points.size(), thick, color);
    }

    /**
     * @brief Draws a Quadratic Bezier spline curve on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the Quadratic Bezier spline curve.
     * @param count The number of points in the array (minimum 3).
     * @param thick The thickness of the Quadratic Bezier spline curve.
     * @param color The color of the Quadratic Bezier spline curve.
     */
    NEXUS_API void DrawSplineBezierQuadratic(nexus::gapi::Context& ctx, const nexus::math::Vec2* points, std::size_t count, float thick, const nexus::gfx::Color& color);

    /**
     * @brief Draws a Quadratic Bezier spline curve on the screen using the provided vector of points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the Quadratic Bezier spline curve.
     * @param thick The thickness of the Quadratic Bezier spline curve.
     * @param color The color of the Quadratic Bezier spline curve.
     */
    NEXUS_API inline void DrawSplineBezierQuadratic(nexus::gapi::Context& ctx, const std::vector<nexus::math::Vec2> points, float thick, const nexus::gfx::Color& color)
    {
        DrawSplineBezierQuadratic(ctx, points.data(), points.size(), thick, color);
    }

    /**
     * @brief Draws a Cubic Bezier spline curve on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the Cubic Bezier spline curve.
     * @param count The number of points in the array (minimum 4).
     * @param thick The thickness of the Cubic Bezier spline curve.
     * @param color The color of the Cubic Bezier spline curve.
     */
    NEXUS_API void DrawSplineBezierCubic(nexus::gapi::Context& ctx, const nexus::math::Vec2* points, std::size_t count, float thick, const nexus::gfx::Color& color);

    /**
     * @brief Draws a Cubic Bezier spline curve on the screen using the provided vector of points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the Cubic Bezier spline curve.
     * @param thick The thickness of the Cubic Bezier spline curve.
     * @param color The color of the Cubic Bezier spline curve.
     */
    NEXUS_API inline void DrawSplineBezierCubic(nexus::gapi::Context& ctx, const std::vector<nexus::math::Vec2>& points, float thick, const nexus::gfx::Color& color)
    {
        DrawSplineBezierCubic(ctx, points.data(), points.size(), thick, color);
    }

    /**
     * @brief Draws a linear spline segment on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The start point of the spline segment.
     * @param p2 The end point of the spline segment.
     * @param thick The thickness of the spline segment.
     * @param color The color of the spline segment.
     */
    NEXUS_API void DrawSplineSegmentLinear(nexus::gapi::Context& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& p2, float thick, const nexus::gfx::Color& color);

    /**
     * @brief Draws a B-Spline spline segment on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The first control point of the spline segment.
     * @param p2 The second control point of the spline segment.
     * @param p3 The third control point of the spline segment.
     * @param p4 The fourth control point of the spline segment.
     * @param thick The thickness of the spline segment.
     * @param color The color of the spline segment.
     */
    NEXUS_API void DrawSplineSegmentBasis(nexus::gapi::Context& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& p2, const nexus::math::Vec2& p3, const nexus::math::Vec2& p4, float thick, const nexus::gfx::Color& color);

    /**
     * @brief Draws a Catmull-Rom spline segment on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The first control point of the spline segment.
     * @param p2 The second control point of the spline segment.
     * @param p3 The third control point of the spline segment.
     * @param p4 The fourth control point of the spline segment.
     * @param thick The thickness of the spline segment.
     * @param color The color of the spline segment.
     */
    NEXUS_API void DrawSplineSegmentCatmullRom(nexus::gapi::Context& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& p2, const nexus::math::Vec2& p3, const nexus::math::Vec2& p4, float thick, const nexus::gfx::Color& color);

    /**
     * @brief Draws a Quadratic Bezier spline segment on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The start point of the spline segment.
     * @param c2 The control point of the spline segment.
     * @param p3 The end point of the spline segment.
     * @param thick The thickness of the spline segment.
     * @param color The color of the spline segment.
     */
    NEXUS_API void DrawSplineSegmentBezierQuadratic(nexus::gapi::Context& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& c2, const nexus::math::Vec2& p3, float thick, const nexus::gfx::Color& color);

    /**
     * @brief Draws a Cubic Bezier spline segment on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The start point of the spline segment.
     * @param c2 The first control point of the spline segment.
     * @param c3 The second control point of the spline segment.
     * @param p4 The end point of the spline segment.
     * @param thick The thickness of the spline segment.
     * @param color The color of the spline segment.
     */
    NEXUS_API void DrawSplineSegmentBezierCubic(nexus::gapi::Context& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& c2, const nexus::math::Vec2& c3, const nexus::math::Vec2& p4, float thick, const nexus::gfx::Color& color);

}

#if 0

#define IF_GAPI_IS(gapi_ns) if constexpr(std::is_same_v<T_Context, nexus::gapi_ns::Context>)
#define ELIF_GAPI_IS(gapi_ns) else IF_GAPI_IS(gapi_ns)

namespace _gapi_primitives_2d_impl {

    template <typename T_Context, typename T_Texture>
    inline void SetTexture(nexus::gapi::Context<T_Context>& ctx, const T_Texture* texture)
    {
        IF_GAPI_IS(gl)
        {
            static_cast<nexus::gl::Context&>(ctx).SetTexture(texture ? (*texture)->GetID()
                : static_cast<nexus::gl::Context&>(ctx).GetTextureIdDefault());
        }
        ELIF_GAPI_IS(sr)
        {
            static_cast<nexus::sr::Context&>(ctx).SetTexture(texture);
        }
    }

}

namespace _gapi_primitives_2d {

    constexpr int SplineSegmentDivisions = 24;
    constexpr float SmoothCircleErrorRate = 0.5f;

    constexpr int CircleSegments = 36;
    constexpr float CircleSegStepLen = nexus::math::Tau / CircleSegments;

    /**
     * @brief Draws a pixel on the screen.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param color The color of the pixel.
     */
    template <typename T_Context>
    void DrawPixel(nexus::gapi::Context<T_Context>& ctx, float x, float y, const nexus::gfx::Color& color)
    {
        ctx.SetDefaultTexture();
        ctx.Begin(nexus::gapi::DrawMode::Quads);

            ctx.Normal(0.0f, 0.0f, 1.0f);
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

    /**
     * @brief Draws a pixel on the screen.
     * 
     * @param ctx The rendering context.
     * @param position The position of the pixel.
     * @param color The color of the pixel.
     */
    template <typename T_Context>
    inline void DrawPixel(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& position, const nexus::gfx::Color& color)
    {
        DrawPixel(ctx, position.x, position.y, color);
    }

    /**
     * @brief Draws a line between two points on the screen.
     * 
     * @param ctx The rendering context.
     * @param x1 The x-coordinate of the starting point.
     * @param y1 The y-coordinate of the starting point.
     * @param x2 The x-coordinate of the ending point.
     * @param y2 The y-coordinate of the ending point.
     * @param color The color of the line.
     */
    template <typename T_Context>
    void DrawLine(nexus::gapi::Context<T_Context>& ctx, float x1, float y1, float x2, float y2, const nexus::gfx::Color& color)
    {
        ctx.Begin(nexus::gapi::DrawMode::Lines);
            ctx.Color(color);
            ctx.Vertex(x1, y1);
            ctx.Vertex(x2, y2);
        ctx.End();
    }

    /**
     * @brief Draws a line between two points on the screen.
     * 
     * @param ctx The rendering context.
     * @param a The starting point of the line.
     * @param b The ending point of the line.
     * @param color The color of the line.
     */
    template <typename T_Context>
    inline void DrawLine(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& a, const nexus::math::Vec2& b, const nexus::gfx::Color& color)
    {
        DrawLine(ctx, a.x, a.y, b.x, b.y, color);
    }

    /**
     * @brief Draws a line defined by a Line object on the screen.
     * 
     * @param ctx The rendering context.
     * @param line The Line object defining the line.
     * @param color The color of the line.
     */
    template <typename T_Context>
    inline void DrawLine(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::Line& line, const nexus::gfx::Color& color)
    {
        DrawLine(ctx, line.start, line.end, color);
    }

    /**
     * @brief Draws a line between two points on the screen with a specified thickness.
     * 
     * @param ctx The rendering context.
     * @param a The starting point of the line.
     * @param b The ending point of the line.
     * @param thick The thickness of the line.
     * @param color The color of the line.
     */
    template <typename T_Context>
    void DrawLine(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& a, const nexus::math::Vec2& b, float thick, const nexus::gfx::Color& color)
    {
        const nexus::math::Vec2 delta = b - a;
        float length = delta.Magnitude();

        if (length > 0 && thick > 0)
        {
            float scale = thick/(2*length);

            const nexus::math::Vec2 radius(
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

    /**
     * @brief Draws a line defined by a Line object on the screen with a specified thickness.
     * 
     * @param ctx The rendering context.
     * @param line The Line object defining the line.
     * @param thick The thickness of the line.
     * @param color The color of the line.
     */
    template <typename T_Context>
    inline void DrawLine(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::Line& line, float thick, const nexus::gfx::Color& color)
    {
        DrawLine(ctx, line.start, line.end, thick, color);
    }

    /**
     * @brief Draws a series of connected lines (line strip) on the screen.
     * 
     * @param ctx The rendering context.
     * @param points An array of points defining the line strip.
     * @param count The number of points in the line strip.
     * @param color The color of the line strip.
     */
    template <typename T_Context>
    void DrawLineStrip(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2* points, std::size_t count, const nexus::gfx::Color& color)
    {
        if (count < 2) return;
        ctx.Begin(nexus::gapi::DrawMode::Lines);

            ctx.Color(color);

            for (std::size_t i = 0; i < count - 1; i++)
            {
                ctx.Vertex(points[i].x, points[i].y);
                ctx.Vertex(points[i + 1].x, points[i + 1].y);
            }

        ctx.End();
    }

    /**
     * @brief Draws a series of connected lines (line strip) on the screen.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points defining the line strip.
     * @param color The color of the line strip.
     */
    template <typename T_Context>
    inline void DrawLineStrip(nexus::gapi::Context<T_Context>& ctx, const std::vector<nexus::math::Vec2>& points, const nexus::gfx::Color& color)
    {
        DrawLineStrip(ctx, points.data(), points.size(), color);
    }

    /**
     * @brief Draws a series of connected lines (line strip) defined by an array of Line objects on the screen.
     * 
     * @param ctx The rendering context.
     * @param lines An array of Line objects defining the line strip.
     * @param count The number of Line objects in the line strip.
     * @param color The color of the line strip.
     */
    template <typename T_Context>
    void DrawLineStrip(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::Line* lines, std::size_t count, const nexus::gfx::Color& color)
    {
        ctx.Begin(nexus::gapi::DrawMode::Lines);

            ctx.Color(color);

            for (std::size_t i = 0; i < count; i++)
            {
                const auto &line = lines[i];
                ctx.Vertex(line.start.x, line.start.y);
                ctx.Vertex(line.end.x, line.end.y);
            }

        ctx.End();
    }

    /**
     * @brief Draws a series of connected lines (line strip) defined by a vector of Line objects on the screen.
     * 
     * @param ctx The rendering context.
     * @param lines A vector of Line objects defining the line strip.
     * @param color The color of the line strip.
     */
    template <typename T_Context>
    inline void DrawLineStrip(nexus::gapi::Context<T_Context>& ctx, const std::vector<nexus::shape2D::Line>& lines, const nexus::gfx::Color& color)
    {
        DrawLineStrip(ctx, lines.data(), lines.size(), color);
    }

    /**
     * @brief Draws a Bezier curve between two points on the screen with a specified thickness.
     * 
     * @param ctx The rendering context.
     * @param start The starting point of the Bezier curve.
     * @param end The ending point of the Bezier curve.
     * @param thick The thickness of the Bezier curve.
     * @param color The color of the Bezier curve.
     */
    template <typename T_Context>
    void DrawLineBezier(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& start, const nexus::math::Vec2& end, float thick, const nexus::gfx::Color& color)
    {
        nexus::math::Vec2 previous = start;
        nexus::math::Vec2 current;

        std::vector<nexus::math::Vec2> points(2 * SplineSegmentDivisions + 2);

        for (int i = 1; i <= SplineSegmentDivisions; i++)
        {
            const double t = static_cast<double>(i) / static_cast<double>(SplineSegmentDivisions);
            current.y = static_cast<float>(nexus::math::EaseCubicInOut(t) * (end.y - start.y) + start.y);
            current.x = previous.x + (end.x - start.x)/SplineSegmentDivisions;

            const nexus::math::Vec2 delta = current - previous;
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

    /**
     * @brief Draws a circle on the screen with a specified radius and center coordinates.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the center of the circle.
     * @param y The y-coordinate of the center of the circle.
     * @param radius The radius of the circle.
     * @param color The color of the circle.
     */
    template <typename T_Context>
    void DrawCircle(nexus::gapi::Context<T_Context>& ctx, float x, float y, float radius, const nexus::gfx::Color& color)
    {
        float angle = 0.0f;

        ctx.SetDefaultTexture();
        ctx.Begin(nexus::gapi::DrawMode::Quads);

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

    /**
     * @brief Draws a circle on the screen with a specified radius and center coordinates.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the circle.
     * @param radius The radius of the circle.
     * @param color The color of the circle.
     */
    template <typename T_Context>
    inline void DrawCircle(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& center, float radius, const nexus::gfx::Color& color)
    {
        DrawCircle(ctx, center.x, center.y, radius, color);
    }

    /**
     * @brief Draws a circle on the screen defined by a Circle object with a specified color.
     * 
     * @param ctx The rendering context.
     * @param circle The Circle object defining the circle.
     * @param color The color of the circle.
     */
    template <typename T_Context>
    inline void DrawCircle(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::Circle& circle, const nexus::gfx::Color& color)
    {
        DrawCircle(ctx, circle.center.x, circle.center.y, circle.radius, color);
    }

    /**
     * @brief Draws a gradient-filled circle on the screen with a specified radius and center coordinates.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the center of the circle.
     * @param y The y-coordinate of the center of the circle.
     * @param radius The radius of the circle.
     * @param color1 The inner color of the circle gradient.
     * @param color2 The outer color of the circle gradient.
     */
    template <typename T_Context>
    void DrawCircleGradient(nexus::gapi::Context<T_Context>& ctx, float x, float y, float radius, const nexus::gfx::Color& color1, const nexus::gfx::Color& color2)
    {
        float angle = 0.0f;

        ctx.Begin(nexus::gapi::DrawMode::Triangles);
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

    /**
     * @brief Draws a gradient-filled circle on the screen with a specified radius and center coordinates.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the circle.
     * @param radius The radius of the circle.
     * @param color1 The inner color of the circle gradient.
     * @param color2 The outer color of the circle gradient.
     */
    template <typename T_Context>
    inline void DrawCircleGradient(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& center, float radius, const nexus::gfx::Color& color1, const nexus::gfx::Color& color2)
    {
        DrawCircleGradient(ctx, center.x, center.y, radius, color1, color2);
    }

    /**
     * @brief Draws a gradient-filled circle on the screen defined by a Circle object with a specified inner and outer color.
     * 
     * @param ctx The rendering context.
     * @param circle The Circle object defining the circle.
     * @param color1 The inner color of the circle gradient.
     * @param color2 The outer color of the circle gradient.
     */
    template <typename T_Context>
    inline void DrawCircleGradient(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::Circle& circle, const nexus::gfx::Color& color1, const nexus::gfx::Color& color2)
    {
        DrawCircleGradient(ctx, circle.center, circle.radius, color1, color2);
    }

    /**
     * @brief Draws the outline of a circle on the screen with a specified radius and center coordinates.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the center of the circle.
     * @param y The y-coordinate of the center of the circle.
     * @param radius The radius of the circle.
     * @param color The color of the circle outline.
     */
    template <typename T_Context>
    void DrawCircleLines(nexus::gapi::Context<T_Context>& ctx, float x, float y, float radius, const nexus::gfx::Color& color)
    {
        float angle = 0.0f;

        ctx.Begin(nexus::gapi::DrawMode::Lines);

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

    /**
     * @brief Draws the outline of a circle on the screen with a specified radius and center coordinates.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the circle.
     * @param radius The radius of the circle.
     * @param color The color of the circle outline.
     */
    template <typename T_Context>
    inline void DrawCircleLines(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& center, float radius, const nexus::gfx::Color& color)
    {
        DrawCircleLines(ctx, center.x, center.y, radius, color);
    }

    /**
     * @brief Draws the outline of a circle on the screen defined by a Circle object with a specified color.
     * 
     * @param ctx The rendering context.
     * @param circle The Circle object defining the circle.
     * @param color The color of the circle outline.
     */
    template <typename T_Context>
    inline void DrawCircleLines(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::Circle& circle, const nexus::gfx::Color& color)
    {
        DrawCircleLines(ctx, circle.center.x, circle.center.y, circle.radius, color);
    }

    /**
     * @brief Draws a sector (portion) of a circle on the screen defined by its center, radius, start and end angles, and the number of segments, with a specified color.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the circle.
     * @param radius The radius of the circle.
     * @param startAngle The starting angle of the sector (in degrees).
     * @param endAngle The ending angle of the sector (in degrees).
     * @param segments The number of segments to approximate the sector.
     * @param color The color of the sector.
     */
    template <typename T_Context>
    void DrawCircleSector(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& center, float radius, float startAngle, float endAngle, int segments, const nexus::gfx::Color& color)
    {
        if (radius <= 0.0f) radius = 0.1f;  // Avoid div by zero

        // Function expects (endAngle > startAngle)
        if (endAngle < startAngle)
        {
            std::swap(startAngle, endAngle);
        }

        // Convert degrees to radians
        startAngle *= nexus::math::Deg2Rad;
        endAngle *= nexus::math::Deg2Rad;

        int minSegments = std::ceil((endAngle - startAngle) / nexus::math::DegToRad(90.0f));

        if (segments < minSegments)
        {
            // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
            float th = std::acos(2 * std::pow(1 - SmoothCircleErrorRate / radius, 2) - 1);
            segments = std::ceil(nexus::math::Tau / th) / 4.0f;

            if (segments <= 0) segments = minSegments;
        }

        float stepLength = (endAngle - startAngle) / static_cast<float>(segments);
        float angle = startAngle;

        ctx.SetDefaultTexture(); ;
        ctx.Begin(nexus::gapi::DrawMode::Quads);

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

    /**
     * @brief Draws a sector (portion) of a circle on the screen defined by a Circle object, start and end angles, and the number of segments, with a specified color.
     * 
     * @param ctx The rendering context.
     * @param circle The Circle object defining the circle.
     * @param startAngle The starting angle of the sector (in degrees).
     * @param endAngle The ending angle of the sector (in degrees).
     * @param segments The number of segments to approximate the sector.
     * @param color The color of the sector.
     */
    template <typename T_Context>
    inline void DrawCircleSector(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::Circle& circle, float startAngle, float endAngle, int segments, const nexus::gfx::Color& color)
    {
        DrawCircleSector(ctx, circle.center, circle.radius, startAngle, endAngle, segments, color);
    }

    /**
     * @brief Draws the outline of a sector (portion) of a circle on the screen defined by its center, radius, start and end angles, and the number of segments, with a specified color.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the circle.
     * @param radius The radius of the circle.
     * @param startAngle The starting angle of the sector (in degrees).
     * @param endAngle The ending angle of the sector (in degrees).
     * @param segments The number of segments to approximate the sector.
     * @param color The color of the sector outline.
     */
    template <typename T_Context>
    void DrawCircleSectorLines(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& center, float radius, float startAngle, float endAngle, int segments, const nexus::gfx::Color& color)
    {
        if (radius <= 0.0f) radius = 0.1f;  // Avoid div by zero issue

        // Function expects (endAngle > startAngle)
        if (endAngle < startAngle)
        {
            std::swap(startAngle, endAngle);
        }

        // Convert degrees to radians
        startAngle *= nexus::math::Deg2Rad;
        endAngle *= nexus::math::Deg2Rad;

        int minSegments = std::ceil((endAngle - startAngle) / nexus::math::DegToRad(90.0f));

        if (segments < minSegments)
        {
            // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
            float th = std::acos(2 * std::pow(1 - SmoothCircleErrorRate / radius, 2) - 1);
            segments = std::ceil(nexus::math::Tau / th) / 4.0f;

            if (segments <= 0) segments = minSegments;
        }

        float stepLength = (endAngle - startAngle) / static_cast<float>(segments);
        float angle = startAngle;
        bool showCapLines = true;

        ctx.Begin(nexus::gapi::DrawMode::Lines);

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

    /**
     * @brief Draws the outline of a sector (portion) of a circle on the screen defined by a Circle object, start and end angles, and the number of segments, with a specified color.
     * 
     * @param ctx The rendering context.
     * @param circle The Circle object defining the circle.
     * @param startAngle The starting angle of the sector (in degrees).
     * @param endAngle The ending angle of the sector (in degrees).
     * @param segments The number of segments to approximate the sector.
     * @param color The color of the sector outline.
     */
    template <typename T_Context>
    inline void DrawCircleSectorLines(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::Circle& circle, float startAngle, float endAngle, int segments, const nexus::gfx::Color& color)
    {
        DrawCircleSectorLines(ctx, circle.center, circle.radius, startAngle, endAngle, segments, color);
    }

    /**
     * @brief Draws an ellipse on the screen with specified center coordinates and radii.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the center of the ellipse.
     * @param y The y-coordinate of the center of the ellipse.
     * @param rx The x-radius of the ellipse.
     * @param ry The y-radius of the ellipse.
     * @param color The color of the ellipse.
     */
    template <typename T_Context>
    void DrawEllipse(nexus::gapi::Context<T_Context>& ctx, float x, float y, float rx, float ry, const nexus::gfx::Color& color)
    {
        float angle = 0.0f;

        ctx.Begin(nexus::gapi::DrawMode::Triangles);

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

    /**
     * @brief Draws an ellipse on the screen with specified center coordinates and radii.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the ellipse.
     * @param rx The x-radius of the ellipse.
     * @param ry The y-radius of the ellipse.
     * @param color The color of the ellipse.
     */
    template <typename T_Context>
    inline void DrawEllipse(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& center, float rx, float ry, const nexus::gfx::Color& color)
    {
        DrawEllipse(ctx, center.x, center.y, rx, ry, color);
    }

    /**
     * @brief Draws an ellipse on the screen with specified center coordinates and radii.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the ellipse.
     * @param radius The x and y radii of the ellipse.
     * @param color The color of the ellipse.
     */
    template <typename T_Context>
    inline void DrawEllipse(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& center, const nexus::math::Vec2& radius, const nexus::gfx::Color& color)
    {
        DrawEllipse(ctx, center.x, center.y, radius.x, radius.y, color);
    }

    /**
     * @brief Draws an ellipse on the screen defined by an Ellipse object with a specified color.
     * 
     * @param ctx The rendering context.
     * @param ellipse The Ellipse object defining the ellipse.
     * @param color The color of the ellipse.
     */
    template <typename T_Context>
    inline void DrawEllipse(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::Ellipse& ellipse, const nexus::gfx::Color& color)
    {
        DrawEllipse(ctx, ellipse.center.x, ellipse.center.y, ellipse.radius.x, ellipse.radius.y, color);
    }

    /**
     * @brief Draws the outline of an ellipse on the screen with specified center coordinates and radii.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the center of the ellipse.
     * @param y The y-coordinate of the center of the ellipse.
     * @param rx The x-radius of the ellipse.
     * @param ry The y-radius of the ellipse.
     * @param color The color of the ellipse outline.
     */
    template <typename T_Context>
    void DrawEllipseLines(nexus::gapi::Context<T_Context>& ctx, int x, int y, float rx, float ry, const nexus::gfx::Color& color)
    {
        float angle = 0.0f;

        ctx.Begin(nexus::gapi::DrawMode::Lines);

            ctx.Color(color);

            for (int i = 0; i < CircleSegments; i++)
            {
                ctx.Vertex(x + std::cos(angle + CircleSegStepLen) * rx, y + std::sin(angle + CircleSegStepLen) * ry);
                ctx.Vertex(x + std::cos(angle) * rx, y + std::sin(angle) * ry);

                angle += CircleSegStepLen;
            }

        ctx.End();
    }

    /**
     * @brief Draws the outline of an ellipse on the screen with specified center coordinates and radii.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the ellipse.
     * @param rx The x-radius of the ellipse.
     * @param ry The y-radius of the ellipse.
     * @param color The color of the ellipse outline.
     */
    template <typename T_Context>
    inline void DrawEllipseLines(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& center, float rx, float ry, const nexus::gfx::Color& color)
    {
        DrawEllipseLines(ctx, center.x, center.y, rx, ry, color);
    }

    /**
     * @brief Draws the outline of an ellipse on the screen with specified center coordinates and radii.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the ellipse.
     * @param radius The x and y radii of the ellipse.
     * @param color The color of the ellipse outline.
     */
    template <typename T_Context>
    inline void DrawEllipseLines(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& center, const nexus::math::Vec2& radius, const nexus::gfx::Color& color)
    {
        DrawEllipseLines(ctx, center.x, center.y, radius.x, radius.y, color);
    }

    /**
     * @brief Draws the outline of an ellipse on the screen defined by an Ellipse object with a specified color.
     * 
     * @param ctx The rendering context.
     * @param ellipse The Ellipse object defining the ellipse.
     * @param color The color of the ellipse outline.
     */
    template <typename T_Context>
    inline void DrawEllipseLines(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::Ellipse& ellipse, const nexus::gfx::Color& color)
    {
        DrawEllipseLines(ctx, ellipse.center.x, ellipse.center.y, ellipse.radius.x, ellipse.radius.y, color);
    }

    /**
     * @brief Draws a ring (annulus) on the screen with specified center coordinates, inner and outer radii, start and end angles, and the number of segments, with a specified color.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the ring.
     * @param innerRadius The inner radius of the ring.
     * @param outerRadius The outer radius of the ring.
     * @param startAngle The starting angle of the ring (in degrees).
     * @param endAngle The ending angle of the ring (in degrees).
     * @param segments The number of segments to approximate the ring.
     * @param color The color of the ring.
     */
    template <typename T_Context>
    void DrawRing(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, const nexus::gfx::Color& color)
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

        int minSegments = std::ceil((endAngle - startAngle) / nexus::math::DegToRad(90.0f));

        if (segments < minSegments)
        {
            // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
            float th = std::acos(2 * std::pow(1 - SmoothCircleErrorRate / outerRadius, 2) - 1);
            segments = std::ceil(nexus::math::Tau / th) / 4.0f;

            if (segments <= 0) segments = minSegments;
        }

        float stepLength = (endAngle - startAngle)/segments;
        float angle = startAngle;

        ctx.SetDefaultTexture();
        ctx.Begin(nexus::gapi::DrawMode::Quads);

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

    /**
     * @brief Draws the outline of a ring (annulus) on the screen with specified center coordinates, inner and outer radii, start and end angles, and the number of segments, with a specified color.
     * 
     * @param ctx The rendering context.
     * @param center The center coordinates of the ring.
     * @param innerRadius The inner radius of the ring.
     * @param outerRadius The outer radius of the ring.
     * @param startAngle The starting angle of the ring (in degrees).
     * @param endAngle The ending angle of the ring (in degrees).
     * @param segments The number of segments to approximate the ring.
     * @param color The color of the ring outline.
     */
    template <typename T_Context>
    void DrawRingLines(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, const nexus::gfx::Color& color)
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

        int minSegments = std::ceil((endAngle - startAngle) / nexus::math::DegToRad(90.0f));

        if (segments < minSegments)
        {
            // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
            float th = std::acos(2 * std::pow(1 - SmoothCircleErrorRate / outerRadius, 2) - 1);
            segments = std::ceil(nexus::math::Tau / th) / 4.0f;

            if (segments <= 0) segments = minSegments;
        }

        float stepLength = (endAngle - startAngle)/segments;
        float angle = startAngle;
        bool showCapLines = true;

        ctx.Begin(nexus::gapi::DrawMode::Lines);

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

    /**
     * @brief Draws a rectangle on the screen with specified parameters: position, size, rotation, and color.
     * 
     * @param ctx The rendering context.
     * @param rec The RectangleF object defining the rectangle (position, size, rotation).
     * @param origin The origin point of the rectangle (default is {0, 0}).
     * @param rotation The rotation angle of the rectangle (default is 0).
     * @param color The color of the rectangle (default is nexus::gfx::White).
     */
    template <typename T_Context>
    void DrawRectangle(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::RectangleF& rec, const nexus::math::Vec2& origin = { 0, 0 }, float rotation = 0, const nexus::gfx::Color& color = nexus::gfx::White)
    {
        nexus::math::Vec2 topLeft, topRight;
        nexus::math::Vec2 bottomLeft, bottomRight;

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
            rotation *= nexus::math::Deg2Rad;

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
        ctx.Begin(nexus::gapi::DrawMode::Quads);

            ctx.Normal(0.0f, 0.0f, 1.0f);
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

    /**
     * @brief Draws a rectangle outline on the screen with specified RectangleF object and color.
     * 
     * @param ctx The rendering context.
     * @param rec The RectangleF object defining the rectangle (position, size, rotation).
     * @param color The color of the rectangle outline.
     */
    template <typename T_Context>
    inline void DrawRectangle(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::RectangleF& rec, const nexus::gfx::Color& color)
    {
        DrawRectangle(ctx, rec, { 0, 0 }, 0, color);
    }

    /**
     * @brief Draws a rectangle outline on the screen with specified position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param color The color of the rectangle outline.
     */
    template <typename T_Context>
    inline void DrawRectangle(nexus::gapi::Context<T_Context>& ctx, float x, float y, float w, float h, const nexus::gfx::Color& color)
    {
        DrawRectangle(ctx, { x, y, w, h }, { 0, 0 }, 0, color);
    }

    /**
     * @brief Draws a rectangle on the screen with specified position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the top-left corner of the rectangle.
     * @param size The size (width and height) of the rectangle.
     * @param color The color of the rectangle.
     */
    template <typename T_Context>
    inline void DrawRectangle(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& position, const nexus::math::Vec2& size, const nexus::gfx::Color& color)
    {
        DrawRectangle(ctx, { position.x, position.y, size.x, size.y }, { 0, 0 }, 0, color);
    }

    /**
     * @brief Draws a rectangle with a gradient fill on the screen with specified RectangleF object and colors for each corner.
     * 
     * @param ctx The rendering context.
     * @param rec The RectangleF object defining the rectangle (position, size, rotation).
     * @param c1 The color of the top-left corner.
     * @param c2 The color of the top-right corner.
     * @param c3 The color of the bottom-right corner.
     * @param c4 The color of the bottom-left corner.
     */
    template <typename T_Context>
    void DrawRectangleGradient(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::RectangleF& rec, const nexus::gfx::Color& c1, const nexus::gfx::Color& c2, const nexus::gfx::Color& c3, const nexus::gfx::Color& c4)
    {
        ctx.SetDefaultTexture();
        ctx.Begin(nexus::gapi::DrawMode::Quads);

            ctx.Normal(0.0f, 0.0f, 1.0f);

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

    /**
     * @brief Draws a rectangle with a gradient fill on the screen with specified position, size, and colors for each corner.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param c1 The color of the top-left corner.
     * @param c2 The color of the top-right corner.
     * @param c3 The color of the bottom-right corner.
     * @param c4 The color of the bottom-left corner.
     */
    template <typename T_Context>
    inline void DrawRectangleGradient(nexus::gapi::Context<T_Context>& ctx, float x, float y, float w, float h, const nexus::gfx::Color& c1, const nexus::gfx::Color& c2, const nexus::gfx::Color& c3, const nexus::gfx::Color& c4)
    {
        DrawRectangleGradient(ctx, { x, y, w, h }, c1, c2, c3, c4);
    }

    /**
     * @brief Draws the outline of a rectangle on the screen with specified position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param x The x-coordinate of the top-left corner of the rectangle.
     * @param y The y-coordinate of the top-left corner of the rectangle.
     * @param w The width of the rectangle.
     * @param h The height of the rectangle.
     * @param color The color of the rectangle outline.
     */
    template <typename T_Context>
    void DrawRectangleLines(nexus::gapi::Context<T_Context>& ctx, float x, float y, float w, float h, const nexus::gfx::Color& color)
    {
        ctx.Begin(nexus::gapi::DrawMode::Lines);

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

    /**
     * @brief Draws the outline of a rectangle on the screen with specified position, size, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the top-left corner of the rectangle.
     * @param size The size (width and height) of the rectangle.
     * @param color The color of the rectangle outline.
     */
    template <typename T_Context>
    inline void DrawRectangleLines(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& position, const nexus::math::Vec2 size, const nexus::gfx::Color& color)
    {
        DrawRectangleLines(ctx, position.x, position.y, size.x, size.y, color);
    }

    /**
     * @brief Draws the outline of an Axis-Aligned Bounding Box (AABB) on the screen with specified AABB object and color.
     * 
     * @param ctx The rendering context.
     * @param aabb The AABB object defining the AABB (axis-aligned bounding box).
     * @param color The color of the AABB outline.
     */
    template <typename T_Context>
    inline void DrawAABB(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::AABB& aabb, const nexus::gfx::Color& color)
    {
        DrawRectangleLines(ctx, (aabb.min + aabb.max) * 0.5f, aabb.max - aabb.min, color);
    }

    /**
     * @brief Draws the outline of a rectangle on the screen with specified RectangleF object, line thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param rec The RectangleF object defining the rectangle (position, size, rotation).
     * @param lineThick The thickness of the rectangle outline.
     * @param color The color of the rectangle outline.
     */
    template <typename T_Context>
    void DrawRectangleLines(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::RectangleF& rec, float lineThick, const nexus::gfx::Color& color)
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

    /**
     * @brief Draws the outline of a rectangle on the screen with specified position, size, line thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the top-left corner of the rectangle.
     * @param size The size (width and height) of the rectangle.
     * @param lineThick The thickness of the rectangle outline.
     * @param color The color of the rectangle outline.
     */
    template <typename T_Context>
    inline void DrawRectangleLines(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& position, const nexus::math::Vec2 size, float lineThick, const nexus::gfx::Color& color)
    {
        DrawRectangleLines(ctx, { position, size }, lineThick, color);
    }

    /**
     * @brief Draws a rounded rectangle on the screen with specified RectangleF object, roundness, number of segments, and color.
     * 
     * @param ctx The rendering context.
     * @param rec The RectangleF object defining the rectangle (position, size, rotation).
     * @param roundness The radius of the corners of the rounded rectangle.
     * @param segments The number of segments used to approximate the rounded corners.
     * @param color The color of the rounded rectangle.
     */
    template <typename T_Context>
    void DrawRectangleRounded(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::RectangleF& rec, float roundness, int segments, const nexus::gfx::Color& color)
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
            segments = std::ceil(nexus::math::Tau / th) / 4.0f;

            if (segments <= 0) segments = 4;
        }

        float stepLength = nexus::math::DegToRad(90.0f)/segments;

        // Coordinates of the 12 points that define the rounded rect
        const nexus::math::Vec2 point[12] = {
            {rec.x + radius, rec.y}, {(rec.x + rec.w) - radius, rec.y}, { rec.x + rec.w, rec.y + radius },      // PO, P1, P2
            {rec.x + rec.w, (rec.y + rec.h) - radius}, {(rec.x + rec.w) - radius, rec.y + rec.h},                   // P3, P4
            {rec.x + radius, rec.y + rec.h}, { rec.x, (rec.y + rec.h) - radius}, {rec.x, rec.y + radius},       // P5, P6, P7
            {rec.x + radius, rec.y + radius}, {(rec.x + rec.w) - radius, rec.y + radius},                           // P8, P9
            {(rec.x + rec.w) - radius, (rec.y + rec.h) - radius}, {rec.x + radius, (rec.y + rec.h) - radius}        // P10, P11
        };

        // [1] Upper Left Corner, [3] Upper Right Corner, [5] Lower Right Corner, [7] Lower Left Corner
        const nexus::math::Vec2 centers[4] = { point[8], point[9], point[10], point[11] };
        constexpr float angles[4] = { nexus::math::DegToRad(180.0f), nexus::math::DegToRad(270.0f), 0.0f, nexus::math::DegToRad(90.0f) };

        ctx.SetDefaultTexture();
        ctx.Begin(nexus::gapi::DrawMode::Quads);

            ctx.Color(color);

            for (int k = 0; k < 4; ++k)
            {
                float angle = angles[k];
                const nexus::math::Vec2 &center = centers[k];

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

    /**
     * @brief Draws a rounded rectangle on the screen with specified position, size, roundness, number of segments, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the top-left corner of the rectangle.
     * @param size The size (width and height) of the rectangle.
     * @param roundness The radius of the corners of the rounded rectangle.
     * @param segments The number of segments used to approximate the rounded corners.
     * @param color The color of the rounded rectangle.
     */
    template <typename T_Context>
    inline void DrawRectangleRounded(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& position, const nexus::math::Vec2 size, float roundness, int segments, const nexus::gfx::Color& color)
    {
        DrawRectangleRounded(ctx, { position, size }, roundness, segments, color);
    }

    /**
     * @brief Draws the outline of a rounded rectangle on the screen with specified RectangleF object, roundness, number of segments, line thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param rec The RectangleF object defining the rectangle (position, size, rotation).
     * @param roundness The radius of the corners of the rounded rectangle.
     * @param segments The number of segments used to approximate the rounded corners.
     * @param lineThick The thickness of the rounded rectangle outline.
     * @param color The color of the rounded rectangle outline.
     */
    template <typename T_Context>
    void DrawRectangleRoundedLines(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::RectangleF& rec, float roundness, int segments, float lineThick, const nexus::gfx::Color& color)
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
            segments = std::ceil(nexus::math::Tau / th) / 4.0f;

            if (segments <= 0) segments = 4;
        }

        float stepLength = nexus::math::DegToRad(90.0f)/segments;
        const float outerRadius = radius + lineThick, innerRadius = radius;

        const nexus::math::Vec2 point[16] = {
            {rec.x + innerRadius, rec.y - lineThick}, {(rec.x + rec.w) - innerRadius, rec.y - lineThick}, { rec.x + rec.w + lineThick, rec.y + innerRadius },   // PO, P1, P2
            {rec.x + rec.w + lineThick, (rec.y + rec.h) - innerRadius}, {(rec.x + rec.w) - innerRadius, rec.y + rec.h + lineThick},                                 // P3, P4
            {rec.x + innerRadius, rec.y + rec.h + lineThick}, { rec.x - lineThick, (rec.y + rec.h) - innerRadius}, {rec.x - lineThick, rec.y + innerRadius},    // P5, P6, P7
            {rec.x + innerRadius, rec.y}, {(rec.x + rec.w) - innerRadius, rec.y},                                                                                   // P8, P9
            { rec.x + rec.w, rec.y + innerRadius }, {rec.x + rec.w, (rec.y + rec.h) - innerRadius},                                                                 // P10, P11
            {(rec.x + rec.w) - innerRadius, rec.y + rec.h}, {rec.x + innerRadius, rec.y + rec.h},                                                                   // P12, P13
            { rec.x, (rec.y + rec.h) - innerRadius}, {rec.x, rec.y + innerRadius}                                                                                   // P14, P15
        };

        const nexus::math::Vec2 centers[4] = {
            {rec.x + innerRadius, rec.y + innerRadius}, {(rec.x + rec.w) - innerRadius, rec.y + innerRadius},                                                       // P16, P17
            {(rec.x + rec.w) - innerRadius, (rec.y + rec.h) - innerRadius}, {rec.x + innerRadius, (rec.y + rec.h) - innerRadius}                                    // P18, P19
        };

        constexpr float angles[4] = {
            nexus::math::DegToRad(180.0f),
            nexus::math::DegToRad(270.0f),
            0.0f,
            nexus::math::DegToRad(90.0f)
        };

        if (lineThick > 1)
        {
            ctx.SetDefaultTexture();
            ctx.Begin(nexus::gapi::DrawMode::Quads);

                ctx.Color(color);

                for (int k = 0; k < 4; ++k)
                {
                    float angle = angles[k];
                    const nexus::math::Vec2 &center = centers[k];

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
            ctx.Begin(nexus::gapi::DrawMode::Lines);

                // Draw all the 4 corners
                for (int k = 0; k < 4; ++k)
                {
                    float angle = angles[k];
                    const nexus::math::Vec2 &center = centers[k];

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

    /**
     * @brief Draws the outline of a rounded rectangle on the screen with specified position, size, roundness, number of segments, line thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param position The position of the top-left corner of the rectangle.
     * @param size The size (width and height) of the rectangle.
     * @param roundness The radius of the corners of the rounded rectangle.
     * @param segments The number of segments used to approximate the rounded corners.
     * @param lineThick The thickness of the rounded rectangle outline.
     * @param color The color of the rounded rectangle outline.
     */
    template <typename T_Context>
    inline void DrawRectangleRoundedLines(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& position, const nexus::math::Vec2 size, float roundness, int segments, float lineThick, const nexus::gfx::Color& color)
    {
        DrawRectangleRoundedLines(ctx, { position, size }, roundness, segments, lineThick, color);
    }

    /**
     * @brief Draws a triangle on the screen with specified vertices and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The first vertex of the triangle.
     * @param p2 The second vertex of the triangle.
     * @param p3 The third vertex of the triangle.
     * @param color The color of the triangle.
     */
    template <typename T_Context>
    void DrawTriangle(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& p2, const nexus::math::Vec2& p3, const nexus::gfx::Color& color)
    {
        IF_GAPI_IS(sr)
        {
            ctx.Begin(nexus::gapi::DrawMode::Triangles);

                ctx.Color(color);

                ctx.Vertex(p1.x, p1.y);
                ctx.Vertex(p2.x, p2.y);
                ctx.Vertex(p3.x, p3.y);

            ctx.End();
        }
        else
        {
            ctx.SetDefaultTexture();
            ctx.Begin(nexus::gapi::DrawMode::Quads);

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
    }

    /**
     * @brief Draws a triangle on the screen with specified TriangleF object and color.
     * 
     * @param ctx The rendering context.
     * @param tri The TriangleF object defining the triangle (vertices).
     * @param color The color of the triangle.
     */
    template <typename T_Context>
    inline void DrawTriangle(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::TriangleF& tri, const nexus::gfx::Color& color)
    {
        DrawTriangle(ctx, tri.a, tri.b, tri.c, color);
    }

    /**
     * @brief Draws the outline of a triangle on the screen with specified vertices and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The first vertex of the triangle.
     * @param p2 The second vertex of the triangle.
     * @param p3 The third vertex of the triangle.
     * @param color The color of the triangle outline.
     */
    template <typename T_Context>
    void DrawTriangleLines(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& p2, const nexus::math::Vec2& p3, const nexus::gfx::Color& color)
    {
        ctx.Begin(nexus::gapi::DrawMode::Lines);

            ctx.Color(color);
            ctx.Vertex(p1.x, p1.y);
            ctx.Vertex(p2.x, p2.y);

            ctx.Vertex(p2.x, p2.y);
            ctx.Vertex(p3.x, p3.y);

            ctx.Vertex(p3.x, p3.y);
            ctx.Vertex(p1.x, p1.y);

        ctx.End();
    }

    /**
     * @brief Draws the outline of a triangle on the screen with specified TriangleF object and color.
     * 
     * @param ctx The rendering context.
     * @param tri The TriangleF object defining the triangle (vertices).
     * @param color The color of the triangle outline.
     */
    template <typename T_Context>
    inline void DrawTriangleLines(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::TriangleF& tri, const nexus::gfx::Color& color)
    {
        DrawTriangleLines(ctx, tri.a, tri.b, tri.c, color);
    }

    /**
     * @brief Draws a triangle fan on the screen using the provided mesh and optional texture.
     * 
     * @note: This function is available to avoid rendering nexus::shape2D::Mesh useless outside the context of nexus::gfx::Renderer,
     *       it is easier to use than a gapi::Mesh for 2D rendering but is less efficient because it requires
     *       a copy of the vertices in the context's batch render buffer.
     * 
     * @param ctx The rendering context.
     * @param mesh The mesh representing the triangle fan.
     * @param texture Optional texture to apply to the triangle fan (default is nullptr).
     */
    template <typename T_Context, typename T_Texture>
    void DrawTriangleFan(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::Mesh& mesh, const T_Texture* texture = nullptr)
    {
        const auto &vertices = mesh.vertices;
        if (vertices.size() < 3) return;

        _gapi_primitives_2d_impl::SetTexture(ctx, texture);

        IF_GAPI_IS(sr)
        {
            ctx.Begin(nexus::gapi::DrawMode::Triangles);

                for (int i = 1; i < vertices.size() - 1; i++)
                {
                    ctx.Color(vertices[0].color);
                    ctx.TexCoord(vertices[0].texcoord);
                    ctx.Vertex(vertices[0].position);

                    ctx.Color(vertices[i].color);
                    ctx.TexCoord(vertices[i].texcoord);
                    ctx.Vertex(vertices[i].position);

                    ctx.Color(vertices[i + 1].color);
                    ctx.TexCoord(vertices[i + 1].texcoord);
                    ctx.Vertex(vertices[i + 1].position);
                }

            ctx.End();
        }
        else
        {
            ctx.Begin(nexus::gapi::DrawMode::Quads);

                for (int i = 1; i < vertices.size() - 1; i++)
                {
                    ctx.Color(vertices[0].color);
                    ctx.TexCoord(vertices[0].texcoord);
                    ctx.Vertex(vertices[0].position);

                    ctx.Color(vertices[i].color);
                    ctx.TexCoord(vertices[i].texcoord);
                    ctx.Vertex(vertices[i].position);

                    ctx.Color(vertices[i + 1].color);
                    ctx.TexCoord(vertices[i + 1].texcoord);
                    ctx.Vertex(vertices[i + 1].position);

                    ctx.Color(vertices[i + 1].color);
                    ctx.TexCoord(vertices[i + 1].texcoord);
                    ctx.Vertex(vertices[i + 1].position);
                }

            ctx.End();
        }

        ctx.UnsetTexture();
    }

    /**
     * @brief Draws a triangle fan on the screen using the provided points and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the triangle fan.
     * @param count The number of points in the array.
     * @param color The color of the triangle fan.
     */
    template <typename T_Context>
    void DrawTriangleFan(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2* points, std::size_t count, const nexus::gfx::Color& color)
    {
        if (count < 3) return;

        ctx.SetDefaultTexture();

        IF_GAPI_IS(sr)
        {
            ctx.Begin(nexus::gapi::DrawMode::Triangles);

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
            ctx.Begin(nexus::gapi::DrawMode::Quads);

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
        }

        ctx.UnsetTexture();
    }

    /**
     * @brief Draws a triangle fan on the screen using the provided vector of points and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the triangle fan.
     * @param color The color of the triangle fan.
     */
    template <typename T_Context>
    inline void DrawTriangleFan(nexus::gapi::Context<T_Context>& ctx, const std::vector<nexus::math::Vec2>& points, const nexus::gfx::Color& color)
    {
        DrawTriangleFan(ctx, points.data(), points.size(), color);
    }

    /**
     * @brief Draws a triangle fan on the screen using the provided vector of triangles and color.
     * 
     * @param ctx The rendering context.
     * @param triangles A vector of triangles representing the vertices of the triangle fan.
     * @param color The color of the triangle fan.
     */
    template <typename T_Context>
    inline void DrawTriangleFan(nexus::gapi::Context<T_Context>& ctx, const std::vector<nexus::shape2D::TriangleF>& triangles, const nexus::gfx::Color& color)
    {
        DrawTriangleFan(ctx, reinterpret_cast<const nexus::math::Vec2*>(triangles.data()), 3 * triangles.size(), color);
    }

    /**
     * @brief Draws a triangle strip on the screen using the provided mesh and optional texture.
     * 
     * @note: This function is available to avoid rendering nexus::shape2D::Mesh useless outside the context of nexus::gfx::Renderer,
     *       it is easier to use than a gapi::Mesh for 2D rendering but is less efficient because it requires
     *       a copy of the vertices in the context's batch render buffer.
     * 
     * @param ctx The rendering context.
     * @param mesh The mesh representing the triangle strip.
     * @param texture Optional texture to apply to the triangle strip (default is nullptr).
     */
    template <typename T_Context, typename T_Texture>
    void DrawTriangleStrip(nexus::gapi::Context<T_Context>& ctx, const nexus::shape2D::Mesh& mesh, const T_Texture* texture = nullptr)
    {
        const auto &vertices = mesh.vertices;
        if (vertices.size() < 3) return;

        _gapi_primitives_2d_impl::SetTexture(ctx, texture);

        IF_GAPI_IS(sr)
        {
            ctx.Begin(nexus::gapi::DrawMode::Triangles);

                for (int i = 2; i < vertices.size(); i++)
                {
                    if (i % 2 == 0)
                    {
                        ctx.Color(vertices[i].color);
                        ctx.TexCoord(vertices[i].texcoord);
                        ctx.Vertex(vertices[i].position);

                        ctx.Color(vertices[i - 2].color);
                        ctx.TexCoord(vertices[i - 2].texcoord);
                        ctx.Vertex(vertices[i - 2].position);

                        ctx.Color(vertices[i - 1].color);
                        ctx.TexCoord(vertices[i - 1].texcoord);
                        ctx.Vertex(vertices[i - 1].position);
                    }
                    else
                    {
                        ctx.Color(vertices[i].color);
                        ctx.TexCoord(vertices[i].texcoord);
                        ctx.Vertex(vertices[i].position);

                        ctx.Color(vertices[i - 1].color);
                        ctx.TexCoord(vertices[i - 1].texcoord);
                        ctx.Vertex(vertices[i - 1].position);

                        ctx.Color(vertices[i - 2].color);
                        ctx.TexCoord(vertices[i - 2].texcoord);
                        ctx.Vertex(vertices[i - 2].position);
                    }
                }

            ctx.End();
        }
        else
        {
            ctx.Begin(nexus::gapi::DrawMode::Quads);

                for (int i = 2; i < vertices.size(); i++)
                {
                    if (i % 2 == 0)
                    {
                        ctx.Color(vertices[i].color);
                        ctx.TexCoord(vertices[i].texcoord);
                        ctx.Vertex(vertices[i].position);

                        ctx.Color(vertices[i - 2].color);
                        ctx.TexCoord(vertices[i - 2].texcoord);
                        ctx.Vertex(vertices[i - 2].position);

                        ctx.Color(vertices[i - 1].color);
                        ctx.TexCoord(vertices[i - 1].texcoord);
                        ctx.Vertex(vertices[i - 1].position);

                        ctx.Color(vertices[i - 1].color);
                        ctx.TexCoord(vertices[i - 1].texcoord);
                        ctx.Vertex(vertices[i - 1].position);
                    }
                    else
                    {
                        ctx.Color(vertices[i].color);
                        ctx.TexCoord(vertices[i].texcoord);
                        ctx.Vertex(vertices[i].position);

                        ctx.Color(vertices[i - 1].color);
                        ctx.TexCoord(vertices[i - 1].texcoord);
                        ctx.Vertex(vertices[i - 1].position);

                        ctx.Color(vertices[i - 2].color);
                        ctx.TexCoord(vertices[i - 2].texcoord);
                        ctx.Vertex(vertices[i - 2].position);

                        ctx.Color(vertices[i - 2].color);
                        ctx.TexCoord(vertices[i - 2].texcoord);
                        ctx.Vertex(vertices[i - 2].position);
                    }
                }

            ctx.End();
        }

        ctx.UnsetTexture();
    }

    /**
     * @brief Draws a triangle strip on the screen using the provided points and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the triangle strip.
     * @param count The number of points in the array.
     * @param color The color of the triangle strip.
     */
    template <typename T_Context>
    void DrawTriangleStrip(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2* points, std::size_t count, const nexus::gfx::Color& color)
    {
        if (count < 3) return;

        ctx.Begin(nexus::gapi::DrawMode::Triangles);

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

    /**
     * @brief Draws a triangle strip on the screen using the provided vector of points and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the triangle strip.
     * @param color The color of the triangle strip.
     */
    template <typename T_Context>
    inline void DrawTriangleStrip(nexus::gapi::Context<T_Context>& ctx, const std::vector<nexus::math::Vec2>& points, const nexus::gfx::Color& color)
    {
        DrawTriangleStrip(ctx, points.data(), points.size(), color);
    }

    /**
     * @brief Draws a triangle strip on the screen using the provided vector of triangles and color.
     * 
     * @param ctx The rendering context.
     * @param triangles A vector of triangles representing the vertices of the triangle strip.
     * @param color The color of the triangle strip.
     */
    template <typename T_Context>
    inline void DrawTriangleStrip(nexus::gapi::Context<T_Context>& ctx, const std::vector<nexus::shape2D::TriangleF>& triangles, const nexus::gfx::Color& color)
    {
        DrawTriangleStrip(ctx, reinterpret_cast<const nexus::math::Vec2*>(triangles.data()), 3 * triangles.size(), color);
    }

    /**
     * @brief Draws a regular polygon on the screen with the specified center, number of sides, radius, rotation, and color.
     * 
     * @param ctx The rendering context.
     * @param center The center of the polygon.
     * @param sides The number of sides of the polygon.
     * @param radius The radius of the polygon.
     * @param rotation The rotation angle of the polygon in degrees.
     * @param color The color of the polygon.
     */
    template <typename T_Context>
    void DrawPolygon(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& center, int sides, float radius, float rotation, const nexus::gfx::Color& color)
    {
        if (sides < 3) sides = 3;

        rotation *= nexus::math::Deg2Rad;
        float angleStep = nexus::math::Tau / sides;

        ctx.SetDefaultTexture();
        ctx.Begin(nexus::gapi::DrawMode::Quads);

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

    /**
     * @brief Draws the outline of a regular polygon on the screen with the specified center, number of sides, radius, rotation, and color.
     * 
     * @param ctx The rendering context.
     * @param center The center of the polygon.
     * @param sides The number of sides of the polygon.
     * @param radius The radius of the polygon.
     * @param rotation The rotation angle of the polygon in degrees.
     * @param color The color of the polygon outline.
     */
    template <typename T_Context>
    void DrawPolygonLines(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& center, int sides, float radius, float rotation, const nexus::gfx::Color& color)
    {
        if (sides < 3) sides = 3;

        rotation *= nexus::math::Deg2Rad;
        float angleStep = nexus::math::Tau / sides;

        ctx.Begin(nexus::gapi::DrawMode::Lines);

            ctx.Color(color);

            for (int i = 0; i < sides; i++)
            {
                ctx.Vertex(center.x + std::cos(rotation) * radius, center.y + std::sin(rotation) * radius);
                ctx.Vertex(center.x + std::cos(rotation + angleStep) * radius, center.y + std::sin(rotation + angleStep) * radius);

                rotation += angleStep;
            }

        ctx.End();
    }

    /**
     * @brief Draws the outline of a regular polygon on the screen with the specified center, number of sides, radius, rotation, line thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param center The center of the polygon.
     * @param sides The number of sides of the polygon.
     * @param radius The radius of the polygon.
     * @param rotation The rotation angle of the polygon in degrees.
     * @param lineThick The thickness of the polygon outline.
     * @param color The color of the polygon outline.
     */
    template <typename T_Context>
    void DrawPolygonLines(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& center, int sides, float radius, float rotation, float lineThick, const nexus::gfx::Color& color)
    {
        if (sides < 3) sides = 3;

        float centralAngle = rotation * nexus::math::Deg2Rad;
        float exteriorAngle = nexus::math::Tau / sides;

        float innerRadius = radius - lineThick * std::cos(exteriorAngle * 0.5f);

        ctx.SetDefaultTexture();
        ctx.Begin(nexus::gapi::DrawMode::Quads);

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

    /**
     * @brief Draws a linear spline on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the spline.
     * @param count The number of points in the array (minimum 2).
     * @param thick The thickness of the spline.
     * @param color The color of the spline.
     */
    template <typename T_Context>
    void DrawSplineLinear(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2* points, std::size_t count, float thick, const nexus::gfx::Color& color)
    {
        nexus::math::Vec2 delta{};
        float length = 0.0f;
        float scale = 0.0f;
        
        for (int i = 0; i < count - 1; i++)
        {
            delta = points[i + 1] - points[i];
            length = delta.Length();

            if (length > 0) scale = thick / (2 * length);

            nexus::math::Vec2 radius = {
                -scale*delta.y, scale*delta.x
            };

            nexus::math::Vec2 strip[4] = {
                { points[i].x - radius.x, points[i].y - radius.y },
                { points[i].x + radius.x, points[i].y + radius.y },
                { points[i + 1].x - radius.x, points[i + 1].y - radius.y },
                { points[i + 1].x + radius.x, points[i + 1].y + radius.y }
            };

            DrawTriangleStrip(ctx, strip, 4, color);
        }
    }

    /**
     * @brief Draws a linear spline on the screen using the provided vector of points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the spline.
     * @param thick The thickness of the spline.
     * @param color The color of the spline.
     */
    template <typename T_Context>
    inline void DrawSplineLinear(nexus::gapi::Context<T_Context>& ctx, const std::vector<nexus::math::Vec2>& points, float thick, const nexus::gfx::Color& color)
    {
        DrawSplineLinear(ctx, points.data(), points.size(), thick, color);
    }

    /**
     * @brief Draws a B-Spline curve on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the B-Spline curve.
     * @param count The number of points in the array (minimum 4).
     * @param thick The thickness of the B-Spline curve.
     * @param color The color of the B-Spline curve.
     */
    template <typename T_Context>
    void DrawSplineBasis(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2* points, std::size_t count, float thick, const nexus::gfx::Color& color)
    {
        if (count < 4) return;

        float a[4]{};
        float b[4]{};
        float dy = 0.0f;
        float dx = 0.0f;
        float size = 0.0f;

        nexus::math::Vec2 currentPoint{};
        nexus::math::Vec2 nextPoint{};
        nexus::math::Vec2 vertices[2 * SplineSegmentDivisions + 2]{};

        for (int i = 0; i < (count - 3); i++)
        {
            float t = 0.0f;
            nexus::math::Vec2 p1 = points[i], p2 = points[i + 1], p3 = points[i + 2], p4 = points[i + 3];

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

    /**
     * @brief Draws a B-Spline curve on the screen using the provided vector of points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the B-Spline curve.
     * @param thick The thickness of the B-Spline curve.
     * @param color The color of the B-Spline curve.
     */
    template <typename T_Context>
    inline void DrawSplineBasis(nexus::gapi::Context<T_Context>& ctx, const std::vector<nexus::math::Vec2>& points, float thick, const nexus::gfx::Color& color)
    {
        DrawSplineBasis(ctx, points.data(), points.size(), thick, color);
    }

    /**
     * @brief Draws a Catmull-Rom spline curve on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the Catmull-Rom spline curve.
     * @param count The number of points in the array (minimum 4).
     * @param thick The thickness of the Catmull-Rom spline curve.
     * @param color The color of the Catmull-Rom spline curve.
     */
    template <typename T_Context>
    void DrawSplineCatmullRom(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2* points, std::size_t count, float thick, const nexus::gfx::Color& color)
    {
        if (count < 4) return;

        float dy = 0.0f;
        float dx = 0.0f;
        float size = 0.0f;

        nexus::math::Vec2 currentPoint = points[1];
        nexus::math::Vec2 nextPoint{};
        nexus::math::Vec2 vertices[2 * SplineSegmentDivisions + 2]{};

        DrawCircle(ctx, currentPoint, thick/2.0f, color);   // Draw init line circle-cap

        for (int i = 0; i < (count - 3); i++)
        {
            float t = 0.0f;
            nexus::math::Vec2 p1 = points[i], p2 = points[i + 1], p3 = points[i + 2], p4 = points[i + 3];

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

    /**
     * @brief Draws a Catmull-Rom spline curve on the screen using the provided vector of points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the Catmull-Rom spline curve.
     * @param thick The thickness of the Catmull-Rom spline curve.
     * @param color The color of the Catmull-Rom spline curve.
     */
    template <typename T_Context>
    inline void DrawSplineCatmullRom(nexus::gapi::Context<T_Context>& ctx, const std::vector<nexus::math::Vec2> points, float thick, const nexus::gfx::Color& color)
    {
        DrawSplineCatmullRom(ctx, points.data(), points.size(), thick, color);
    }

    /**
     * @brief Draws a Quadratic Bezier spline curve on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the Quadratic Bezier spline curve.
     * @param count The number of points in the array (minimum 3).
     * @param thick The thickness of the Quadratic Bezier spline curve.
     * @param color The color of the Quadratic Bezier spline curve.
     */
    template <typename T_Context>
    void DrawSplineBezierQuadratic(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2* points, std::size_t count, float thick, const nexus::gfx::Color& color)
    {
        if (count < 3) return;
        
        for (int i = 0; i < count - 2; i++)
        {
            DrawSplineSegmentBezierQuadratic(ctx, points[i], points[i + 1], points[i + 2], thick, color);
        }
    }

    /**
     * @brief Draws a Quadratic Bezier spline curve on the screen using the provided vector of points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the Quadratic Bezier spline curve.
     * @param thick The thickness of the Quadratic Bezier spline curve.
     * @param color The color of the Quadratic Bezier spline curve.
     */
    template <typename T_Context>
    inline void DrawSplineBezierQuadratic(nexus::gapi::Context<T_Context>& ctx, const std::vector<nexus::math::Vec2> points, float thick, const nexus::gfx::Color& color)
    {
        DrawSplineBezierQuadratic(ctx, points.data(), points.size(), thick, color);
    }

    /**
     * @brief Draws a Cubic Bezier spline curve on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points An array of points representing the vertices of the Cubic Bezier spline curve.
     * @param count The number of points in the array (minimum 4).
     * @param thick The thickness of the Cubic Bezier spline curve.
     * @param color The color of the Cubic Bezier spline curve.
     */
    template <typename T_Context>
    void DrawSplineBezierCubic(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2* points, std::size_t count, float thick, const nexus::gfx::Color& color)
    {
        if (count < 4) return;
        
        for (int i = 0; i < count - 3; i++)
        {
            DrawSplineSegmentBezierCubic(ctx, points[i], points[i + 1], points[i + 2], points[i + 3], thick, color);
        }
    }

    /**
     * @brief Draws a Cubic Bezier spline curve on the screen using the provided vector of points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param points A vector of points representing the vertices of the Cubic Bezier spline curve.
     * @param thick The thickness of the Cubic Bezier spline curve.
     * @param color The color of the Cubic Bezier spline curve.
     */
    template <typename T_Context>
    inline void DrawSplineBezierCubic(nexus::gapi::Context<T_Context>& ctx, const std::vector<nexus::math::Vec2>& points, float thick, const nexus::gfx::Color& color)
    {
        DrawSplineBezierCubic(ctx, points.data(), points.size(), thick, color);
    }

    /**
     * @brief Draws a linear spline segment on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The start point of the spline segment.
     * @param p2 The end point of the spline segment.
     * @param thick The thickness of the spline segment.
     * @param color The color of the spline segment.
     */
    template <typename T_Context>
    void DrawSplineSegmentLinear(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& p2, float thick, const nexus::gfx::Color& color)
    {
        // NOTE: For the linear spline we don't use subdivisions, just a single quad
        
        nexus::math::Vec2 delta = p2 - p1;
        float length = delta.Length();

        if (length > 0 && thick > 0)
        {
            float scale = thick/(2*length);

            nexus::math::Vec2 radius = {
                -scale * delta.y, scale * delta.x
            };

            nexus::math::Vec2 strip[4] = {
                { p1.x - radius.x, p1.y - radius.y },
                { p1.x + radius.x, p1.y + radius.y },
                { p2.x - radius.x, p2.y - radius.y },
                { p2.x + radius.x, p2.y + radius.y }
            };

            DrawTriangleStrip(ctx, strip, 4, color);
        }
    }

    /**
     * @brief Draws a B-Spline spline segment on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The first control point of the spline segment.
     * @param p2 The second control point of the spline segment.
     * @param p3 The third control point of the spline segment.
     * @param p4 The fourth control point of the spline segment.
     * @param thick The thickness of the spline segment.
     * @param color The color of the spline segment.
     */
    template <typename T_Context>
    void DrawSplineSegmentBasis(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& p2, const nexus::math::Vec2& p3, const nexus::math::Vec2& p4, float thick, const nexus::gfx::Color& color)
    {
        constexpr float step = 1.0f / SplineSegmentDivisions;

        nexus::math::Vec2 currentPoint{};
        nexus::math::Vec2 nextPoint{};
        float t = 0.0f;
        
        nexus::math::Vec2 points[2 * SplineSegmentDivisions + 2]{};
        
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

    /**
     * @brief Draws a Catmull-Rom spline segment on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The first control point of the spline segment.
     * @param p2 The second control point of the spline segment.
     * @param p3 The third control point of the spline segment.
     * @param p4 The fourth control point of the spline segment.
     * @param thick The thickness of the spline segment.
     * @param color The color of the spline segment.
     */
    template <typename T_Context>
    void DrawSplineSegmentCatmullRom(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& p2, const nexus::math::Vec2& p3, const nexus::math::Vec2& p4, float thick, const nexus::gfx::Color& color)
    {
        constexpr float step = 1.0f / SplineSegmentDivisions;

        nexus::math::Vec2 currentPoint = p1;
        nexus::math::Vec2 nextPoint{};
        float t = 0.0f;
        
        nexus::math::Vec2 points[2 * SplineSegmentDivisions + 2]{};

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

    /**
     * @brief Draws a Quadratic Bezier spline segment on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The start point of the spline segment.
     * @param c2 The control point of the spline segment.
     * @param p3 The end point of the spline segment.
     * @param thick The thickness of the spline segment.
     * @param color The color of the spline segment.
     */
    template <typename T_Context>
    void DrawSplineSegmentBezierQuadratic(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& c2, const nexus::math::Vec2& p3, float thick, const nexus::gfx::Color& color)
    {
        constexpr float step = 1.0f / SplineSegmentDivisions;

        nexus::math::Vec2 previous = p1;
        nexus::math::Vec2 current{};
        float t = 0.0f;

        nexus::math::Vec2 points[2 * SplineSegmentDivisions + 2]{};

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

    /**
     * @brief Draws a Cubic Bezier spline segment on the screen using the provided points, thickness, and color.
     * 
     * @param ctx The rendering context.
     * @param p1 The start point of the spline segment.
     * @param c2 The first control point of the spline segment.
     * @param c3 The second control point of the spline segment.
     * @param p4 The end point of the spline segment.
     * @param thick The thickness of the spline segment.
     * @param color The color of the spline segment.
     */
    template <typename T_Context>
    void DrawSplineSegmentBezierCubic(nexus::gapi::Context<T_Context>& ctx, const nexus::math::Vec2& p1, const nexus::math::Vec2& c2, const nexus::math::Vec2& c3, const nexus::math::Vec2& p4, float thick, const nexus::gfx::Color& color)
    {
        constexpr float step = 1.0f / SplineSegmentDivisions;

        nexus::math::Vec2 previous = p1;
        nexus::math::Vec2 current{};
        float t = 0.0f;

        nexus::math::Vec2 points[2 * SplineSegmentDivisions + 2]{};

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

}

#endif //0

#endif //NEXUS_GAPI_IMPL_PRIMITIVES_2D_HPP
