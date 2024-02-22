#ifndef NEXUS_EXT_GFX_RENDERER_HPP
#define NEXUS_EXT_GFX_RENDERER_HPP

#include "../../shape/2D/nxRectangle.hpp"
#include "../../shape/2D/nxPolygon.hpp"
#include "../../shape/2D/nxCircle.hpp"
#include "../../shape/2D/nxMesh.hpp"
#include "../../shape/2D/nxLine.hpp"

#include "../../core/nxException.hpp"
#include "../../core/nxWindow.hpp"
#include "../../gfx/nxSurface.hpp"
#include "../../math/nxVec2.hpp"
#include "../nxBlendMode.hpp"
#include "../nxPixel.hpp"

#include <SDL_blendmode.h>
#include <SDL_render.h>
#include <vector>

namespace nexus { namespace gfx {

    class Texture;

    /**
     * @enum RendererFlag
     * @brief Enumeration of renderer flags for configuring SDL renderers.
     *
     * This enumeration defines various renderer flags that can be used to configure
     * the behavior of SDL renderers.
     */
    enum class RendererFlag : Uint32
    {
        Software        = SDL_RENDERER_SOFTWARE,            ///< The renderer is a software fallback
        Accelerated     = SDL_RENDERER_ACCELERATED,         ///< The renderer uses hardware acceleration
        PresentVSync    = SDL_RENDERER_PRESENTVSYNC,        ///< Present is synchronized with the refresh rate
        TargetTexture   = SDL_RENDERER_TARGETTEXTURE        ///< The renderer supports rendering to texture
    };

    /**
     * @brief Bitwise OR operator for combining renderer flags.
     *
     * @param a The first flag to combine.
     * @param b The second flag to combine.
     * @return The result of combining the two flags using the OR operator.
     */
    NEXUS_API constexpr RendererFlag operator|(RendererFlag a, RendererFlag b)
    {
        return static_cast<RendererFlag>(static_cast<Uint32>(a) | static_cast<Uint32>(b));
    }

    /**
     * @brief Bitwise AND operator for checking if both flags are set.
     *
     * @param a The first flag to check.
     * @param b The second flag to check.
     * @return True if both flags are set, otherwise false.
     */
    NEXUS_API constexpr RendererFlag operator&(RendererFlag a, RendererFlag b)
    {
        return static_cast<RendererFlag>(static_cast<Uint32>(a) & static_cast<Uint32>(b));
    }

    /**
     * @brief Bitwise NOT operator for inverting a flag's value.
     *
     * @param a The flag to invert.
     * @return The inverted flag.
     */
    NEXUS_API constexpr RendererFlag operator~(RendererFlag a)
    {
        return static_cast<RendererFlag>(~static_cast<Uint32>(a));
    }

    /**
     * @brief Bitwise OR assignment operator for combining a flag with another.
     *
     * @param a The flag to combine.
     * @param b The flag to combine with.
     * @return The result of combining the two flags using the OR operator.
     */
    NEXUS_API constexpr RendererFlag operator|=(RendererFlag& a, RendererFlag b)
    {
        return static_cast<RendererFlag>(static_cast<Uint32>(a) | static_cast<Uint32>(b));
    }

    /**
     * @brief Alias for SDL_RendererInfo.
     * 
     * This typedef allows SDL_RendererInfo to be accessed 
     * conveniently within the nexus::gfx namespace.
     */
    typedef SDL_RendererInfo RendererInfo;

    /**
     * @brief A class that wraps SDL_Renderer.
     * 
     * This class provides a wrapper around SDL_Renderer,
     * simplifying its usage and managing its resources.
     */
    class NEXUS_API Renderer
    {
      private:
        SDL_Renderer *renderer; ///< Pointer to the SDL_Renderer instance.

      public:
        /**
         * @brief Gets the number of available rendering drivers.
         * 
         * @return The number of available rendering drivers, or -1 on error.
         */
        static int GetNumDrivers(void)
        {
            return SDL_GetNumRenderDrivers();
        }

        /**
         * @brief Gets information about a rendering driver.
         * 
         * @param index The index of the rendering driver to query.
         * @param info Pointer to an SDL_RendererInfo structure to be filled with information.
         * @return 0 on success, or -1 if the index is out of range or if there's an error.
         */
        static int GetDriverInfo(int index, SDL_RendererInfo* info)
        {
            return SDL_GetRenderDriverInfo(index, info);
        }

      public:
        /**
         * @brief Constructor for creating a renderer associated with a window.
         *
         * This constructor initializes a renderer associated with the provided window,
         * rendering with the specified driver index, and with the specified flags.
         *
         * @param window The window to associate with the renderer.
         * @param index The index of the rendering driver to initialize.
         * @param flags The renderer flags specifying rendering options.
         * @throws core::NexusException if renderer creation fails.
         */
        Renderer(core::Window& window, int index = -1, RendererFlag flags = RendererFlag::Accelerated);

        /**
         * @brief Constructor for creating a software renderer associated with a surface.
         *
         * This constructor initializes a software renderer associated with the provided surface.
         *
         * @param surface The surface to associate with the renderer.
         * @throws core::NexusException if renderer creation fails.
         */
        Renderer(Surface& surface);

        /**
         * @brief Destructor to clean up the Renderer object.
         */
        ~Renderer();

        /**
         * @brief Implicit conversion operator to retrieve a non-const pointer to the SDL_Renderer.
         *
         * This operator allows implicit conversion to a non-const pointer to the SDL_Renderer.
         * It can be used to access the underlying SDL_Renderer data, but use it with caution to prevent unintended modifications.
         *
         * @return A non-const pointer to the SDL_Renderer.
         */
        operator SDL_Renderer*() { return renderer; }

        /**
         * @brief Implicit conversion operator to retrieve a const pointer to the SDL_Renderer.
         *
         * This operator allows implicit conversion to a const pointer to the SDL_Renderer.
         * It can be used for read-only access to the underlying SDL_Renderer data.
         *
         * @return A const pointer to the SDL_Renderer.
         */
        operator const SDL_Renderer*() const { return renderer; }

        /**
         * @brief Get a pointer to the SDL_Renderer.
         *
         * This function allows direct access to the underlying SDL_Renderer data.
         *
         * @return A pointer to the SDL_Renderer.
         */
        SDL_Renderer* Get() const { return renderer; }

        /**
         * @brief Retrieves information about the renderer.
         * 
         * This function retrieves information about the renderer associated with this instance of Renderer.
         * 
         * @return A RendererInfo structure containing information about the renderer.
         */
        RendererInfo GetInfo() const;

        /**
         * @brief Retrieves information about the renderer.
         * 
         * This function retrieves information about the renderer associated with this instance of Renderer.
         * 
         * @param info A reference to a RendererInfo structure to be filled with information about the renderer.
         */
        void GetInfo(RendererInfo& info) const;

        /**
         * @brief Get the current rendering viewport.
         *
         * This function retrieves the current rendering viewport and stores it in the provided `viewport` parameter.
         *
         * @param viewport The rectangle to store the current viewport in.
         */
        void GetViewport(shape2D::Rectangle& viewport);

        /**
         * @brief Get the current rendering viewport.
         *
         * This function retrieves the current rendering viewport and returns it as a `shape2D::Rectangle`.
         *
         * @return The current rendering viewport as a `shape2D::Rectangle`.
         */
        shape2D::Rectangle GetViewport();

        /**
         * @brief Get the current rendering target as an unmanaged texture.
         *
         * This function retrieves the current rendering target and returns it as an unmanaged texture. An unmanaged texture does not automatically manage the lifetime of the SDL_Texture.
         *
         * @return The current rendering target as an unmanaged texture.
         */
        SDL_Texture* GetTarget();

        /**
         * @brief Get the logical size for rendering.
         *
         * This function retrieves the current logical size for rendering and returns it as a math::IVec2.
         *
         * @return The logical size for rendering as a math::IVec2.
         */
        math::IVec2 GetLogicalSize();

        /**
         * @brief Get the current rendering draw color.
         *
         * This function retrieves the current rendering draw color, which is the color used for drawing operations in the renderer, including Clear and drawing primitives.
         *
         * @return The current rendering draw color as a Color structure.
         */
        Color GetColor();

        /**
         * @brief Get the current blend mode used for rendering.
         *
         * This function retrieves the current blend mode used for drawing operations in the renderer. The blend mode determines how the renderer combines the source and destination colors during rendering.
         *
         * @throws core::NexusException If unable to get the blend mode.
         *
         * @return The current blend mode as a BlendMode enumeration.
         */
        BlendMode GetBlendMode();

        /**
         * @brief Read pixel data from the renderer.
         *
         * This function reads pixel data from the specified region of the renderer and stores it in the provided memory buffer.
         *
         * @param rect The region to read pixels from.
         * @param format The desired pixel format for the pixel data.
         * @param pixels A pointer to the memory buffer where the pixel data will be stored.
         * @param pitch The pitch (stride) of the pixel data buffer.
         */
        void ReadPixels(const shape2D::Rectangle& rect, PixelFormat format, void* pixels, int pitch);

        /**
         * @brief Set the rendering viewport.
         *
         * This function sets the rendering viewport to the specified rectangle.
         *
         * @param viewport The rectangle defining the new viewport.
         */
        void SetViewport(const shape2D::Rectangle& viewport);

        /**
         * @brief Set the rendering target to a texture.
         *
         * This function sets the current rendering target to the specified texture. Subsequent rendering operations will be directed to this texture.
         *
         * @param texture The texture to use as the rendering target.
         */
        void SetTarget(Texture& texture);

        /**
         * @brief Set the rendering target to an SDL texture.
         *
         * This function sets the current rendering target to the specified SDL texture. Subsequent rendering operations will be directed to this texture.
         *
         * @param texture The SDL texture to use as the rendering target.
         */
        void SetTarget(SDL_Texture* texture);

        /**
         * @brief Set the logical size for rendering.
         *
         * This function sets the logical size for rendering. The logical size determines how the rendering output is scaled to the window or target surface.
         *
         * @param width The width of the logical rendering size.
         * @param height The height of the logical rendering size.
         * @throws core::NexusException if setting the logical size fails.
         */
        void SetLogicalSize(int width, int height);

        /**
         * @brief Enable or disable integer scaling.
         *
         * This function enables or disables integer scaling, which ensures rendering at integer scales for pixel-perfect rendering.
         *
         * @param enable `true` to enable integer scaling, `false` to disable it.
         */
        void SetIntegerScale(bool enable);

        /**
         * @brief Clear the renderer with a specific color.
         *
         * This function clears the renderer with the specified color,
         * effectively erasing the previous frame's content and filling it with the provided color.
         *
         * @param color The Color to use for clearing the renderer.
         */
        void Clear(Color color = Black);

        /**
         * @brief Present the renderer.
         *
         * This function presents the contents of the renderer to the window or target surface, displaying the rendered frame.
         */
        void Present();

        /**
         * @brief Set the rendering draw color.
         *
         * This function sets the color used for drawing operations in the renderer, including Clear and drawing primitives.
         *
         * @param color The color to set as the rendering draw color.
         */
        void SetColor(const Color& color);

        /**
         * @brief Sets the rendering draw color using individual RGBA components.
         *
         * This function sets the color used for drawing operations in the renderer by specifying the red (r), green (g),
         * blue (b), and alpha (a) components individually.
         *
         * @param r The red component of the color (0-255).
         * @param g The green component of the color (0-255).
         * @param b The blue component of the color (0-255).
         * @param a The alpha (transparency) component of the color (0-255, default is 255 for fully opaque).
         */
        void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

        /**
         * @brief Set the blend mode used for rendering.
         *
         * This function sets the blend mode used for drawing operations in the renderer. The blend mode determines how the renderer combines the source and destination colors during rendering.
         *
         * @throws core::NexusException If unable to set the blend mode.
         *
         * @param blendMode The blend mode to set for rendering.
         */
        void SetBlendMode(BlendMode blendMode);

        /**
         * @brief Draw a point on the renderer.
         *
         * This function draws a single point at the specified coordinates on the renderer.
         *
         * @param point The 2D vector containing the coordinates of the point.
         */
        void DrawPoint(const math::Vec2& point);

        /**
         * @brief Draw a point on the renderer.
         *
         * This function draws a single point at the specified integer coordinates on the renderer.
         *
         * @param point The 2D vector containing the integer coordinates of the point.
         */
        void DrawPoint(const math::IVec2& point);

        /**
         * @brief Draw a point on the renderer.
         *
         * This function draws a single point at the specified integer coordinates on the renderer.
         *
         * @param x The x-coordinate of the point.
         * @param y The y-coordinate of the point.
         */
        void DrawPoint(int x, int y);

        /**
         * @brief Draw multiple points on the renderer.
         *
         * This function draws multiple points on the renderer using a vector of 2D vectors.
         *
         * @param points A vector of 2D vectors representing the coordinates of the points.
         */
        void DrawPoints(const std::vector<math::Vec2>& points);

        /**
         * @brief Draw multiple points on the renderer.
         *
         * This function draws multiple points on the renderer using a vector of integer 2D vectors.
         *
         * @param points A vector of integer 2D vectors representing the coordinates of the points.
         */
        void DrawPoints(const std::vector<math::IVec2>& points);

        /**
         * @brief Draw a line on the renderer.
         *
         * This function draws a line segment on the renderer between two points specified by their coordinates.
         *
         * @param start The starting point of the line.
         * @param end The ending point of the line.
         */
        void DrawLine(const math::Vec2& start, const math::Vec2& end);

        /**
         * @brief Draw a line on the renderer.
         *
         * This function draws a line segment on the renderer between two points specified by their coordinates.
         *
         * @param line The starting and ending point of the line.
         */
        void DrawLine(const shape2D::Line& line);

        /**
         * @brief Draw a line on the renderer.
         *
         * This function draws a line segment on the renderer between two points specified by their integer coordinates.
         *
         * @param start The starting point of the line.
         * @param end The ending point of the line.
         */
        void DrawLine(const math::IVec2& start, const math::IVec2& end);

        /**
         * @brief Draw a line on the renderer.
         *
         * This function draws a line segment on the renderer between two points specified by their integer coordinates.
         *
         * @param x1 The x-coordinate of the starting point.
         * @param y1 The y-coordinate of the starting point.
         * @param x2 The x-coordinate of the ending point.
         * @param y2 The y-coordinate of the ending point.
         */
        void DrawLine(int x1, int y1, int x2, int y2);

        /**
         * @brief Draw multiple line segments on the renderer.
         *
         * This function draws multiple line segments on the renderer using a vector of 2D lines.
         *
         * @param lines A vector of 2D lines to be drawn on the renderer.
         */
        void DrawLines(const std::vector<shape2D::Line>& lines);

        /**
         * @brief Draw multiple line segments on the renderer.
         *
         * This function draws multiple line segments on the renderer using a vector of 2D points.
         *
         * @param points A vector of 2D points that define the line segments to be drawn.
         */
        void DrawLines(const std::vector<math::Vec2>& points);

        /**
         * @brief Draw multiple line segments on the renderer.
         *
         * This function draws multiple line segments on the renderer using a vector of integer 2D points.
         *
         * @param points A vector of integer 2D points that define the line segments to be drawn.
         */
        void DrawLines(const std::vector<math::IVec2>& points);

        /**
         * @brief Draw the lines of a polygon on the renderer.
         *
         * This function draws the lines of a polygon on the renderer. The polygon is specified as a `Polygon` structure.
         *
         * @param poly The `Polygon` object representing the polygon to be drawn.
         */
        void DrawPolygonLines(const shape2D::Polygon poly);

        /**
         * @brief Draw the lines of a polygon on the renderer using vector of 2D points.
         *
         * This function draws the lines of a polygon on the renderer using a vector of 2D points to specify the vertices of the polygon.
         *
         * @param verts A vector of 2D points representing the vertices of the polygon to be drawn.
         */
        void DrawPolygonLines(const std::vector<math::Vec2>& verts);

        /**
         * @brief Draw a filled triangle on the renderer.
         *
         * This function draws a filled triangle on the renderer using the specified triangle object.
         *
         * @param tri The triangle object representing the vertices and color of the triangle to be drawn.
         */
        void DrawTriangle(const shape2D::TriangleF& tri);

        /**
         * @brief Draw the lines of a triangle on the renderer.
         *
         * This function draws the lines of a triangle on the renderer using the specified triangle object.
         *
         * @param tri The triangle object representing the vertices of the triangle to be drawn.
         */
        void DrawTriangleLines(const shape2D::TriangleF& tri);

        /**
         * @brief Draw a filled triangle on the renderer using specified vertices.
         *
         * This function draws a filled triangle on the renderer using the specified vertices, each containing position, color, and texture coordinates.
         *
         * @param v0 The vertex representing the first point of the triangle.
         * @param v1 The vertex representing the second point of the triangle.
         * @param v2 The vertex representing the third point of the triangle.
         * @param texture (Optional) A pointer to the texture to be applied to the triangle. Default is nullptr if no texture is provided.
         */
        void DrawTriangle(const shape2D::Vertex& v0, const shape2D::Vertex& v1, const shape2D::Vertex& v2, SDL_Texture* texture = nullptr);

        /**
         * @brief Draw a filled quad on the renderer using specified vertices.
         *
         * This function draws a filled quad on the renderer using the specified vertices, each containing position, color, and texture coordinates.
         *
         * @param v0 The vertex representing the first point of the quad.
         * @param v1 The vertex representing the second point of the quad.
         * @param v2 The vertex representing the third point of the quad.
         * @param v3 The vertex representing the fourth point of the quad.
         * @param texture (Optional) A pointer to the texture to be applied to the quad. Default is nullptr if no texture is provided.
         */
        void DrawQuad(const shape2D::Vertex& v0, const shape2D::Vertex& v1, const shape2D::Vertex& v2, const shape2D::Vertex& v3, SDL_Texture* texture = nullptr);

        /**
         * @brief Draw a filled rectangle on the renderer.
         *
         * This function draws a filled rectangle on the renderer using the provided RectangleF.
         *
         * @param rect The RectangleF that defines the dimensions and position of the rectangle to be drawn.
         */
        void DrawRectangle(const shape2D::RectangleF& rect);

        /**
         * @brief Draw a filled rectangle on the renderer.
         *
         * This function draws a filled rectangle on the renderer using the provided Rectangle.
         *
         * @param rect The Rectangle that defines the dimensions and position of the rectangle to be drawn.
         */
        void DrawRectangle(const shape2D::Rectangle& rect);

        /**
         * @brief Draw a filled rectangle on the renderer.
         *
         * This function draws a filled rectangle on the renderer using the provided float coordinates and dimensions.
         *
         * @param x The x-coordinate of the top-left corner of the rectangle.
         * @param y The y-coordinate of the top-left corner of the rectangle.
         * @param w The width of the rectangle.
         * @param h The height of the rectangle.
         */
        void DrawRectangle(float x, float y, float w, float h);

        /**
         * @brief Draw a filled rectangle on the renderer.
         *
         * This function draws a filled rectangle on the renderer using the provided integer coordinates and dimensions.
         *
         * @param x The x-coordinate of the top-left corner of the rectangle.
         * @param y The y-coordinate of the top-left corner of the rectangle.
         * @param w The width of the rectangle.
         * @param h The height of the rectangle.
         */
        void DrawRectangle(int x, int y, int w, int h);

        /**
         * @brief Draw multiple filled rectangles on the renderer.
         *
         * This function draws multiple filled rectangles on the renderer using a vector of Rectangle objects.
         *
         * @param rects A vector of Rectangle objects defining the dimensions and positions of the rectangles to be drawn.
         */
        void DrawRectangles(const std::vector<shape2D::Rectangle>& rects);

        /**
         * @brief Draw multiple filled rectangles on the renderer.
         *
         * This function draws multiple filled rectangles on the renderer using a vector of RectangleF objects.
         *
         * @param rects A vector of RectangleF objects defining the dimensions and positions of the rectangles to be drawn.
         */
        void DrawRectangles(const std::vector<shape2D::RectangleF>& rects);

        /**
         * @brief Draw the outline of a rectangle on the renderer.
         *
         * This function draws the outline of a rectangle on the renderer using the provided RectangleF.
         *
         * @param rect The RectangleF that defines the dimensions and position of the rectangle outline to be drawn.
         */
        void DrawRectangleLines(const shape2D::RectangleF& rect);

        /**
         * @brief Draw the outline of a rectangle on the renderer.
         *
         * This function draws the outline of a rectangle on the renderer using the provided Rectangle.
         *
         * @param rect The Rectangle that defines the dimensions and position of the rectangle outline to be drawn.
         */
        void DrawRectangleLines(const shape2D::Rectangle& rect);

        /**
         * @brief Draw the outline of a rectangle on the renderer.
         *
         * This function draws the outline of a rectangle on the renderer using the provided float coordinates and dimensions.
         *
         * @param x The x-coordinate of the top-left corner of the rectangle.
         * @param y The y-coordinate of the top-left corner of the rectangle.
         * @param w The width of the rectangle.
         * @param h The height of the rectangle.
         */
        void DrawRectangleLines(float x, float y, float w, float h);

        /**
         * @brief Draw the outline of a rectangle on the renderer.
         *
         * This function draws the outline of a rectangle on the renderer using the provided integer coordinates and dimensions.
         *
         * @param x The x-coordinate of the top-left corner of the rectangle.
         * @param y The y-coordinate of the top-left corner of the rectangle.
         * @param w The width of the rectangle.
         * @param h The height of the rectangle.
         */
        void DrawRectangleLines(int x, int y, int w, int h);

        /**
         * @brief Draw the outline of an Axis-Aligned Bounding Box (AABB) on the renderer.
         *
         * This function draws the outline of an Axis-Aligned Bounding Box (AABB) on the renderer using the provided AABB structure.
         *
         * @param aabb The Axis-Aligned Bounding Box (AABB) to be drawn.
         */
        void DrawAABB(const shape2D::AABB& aabb);

        /**
         * @brief Draw the outlines of multiple rectangles on the renderer.
         *
         * This function draws the outlines of multiple rectangles on the renderer using a vector of Rectangle objects.
         *
         * @param rects A vector of Rectangle objects defining the dimensions and positions of the rectangle outlines to be drawn.
         */
        void DrawRectanglesLines(const std::vector<shape2D::Rectangle>& rects);

        /**
         * @brief Draw the outlines of multiple rectangles on the renderer.
         *
         * This function draws the outlines of multiple rectangles on the renderer using a vector of RectangleF objects.
         *
         * @param rects A vector of RectangleF objects defining the dimensions and positions of the rectangle outlines to be drawn.
         */
        void DrawRectanglesLines(const std::vector<shape2D::RectangleF>& rects);

        /**
         * @brief Draw a filled circle on the renderer.
         *
         * This function draws a filled circle on the renderer using the provided float coordinates and radius.
         *
         * @param cx The x-coordinate of the center of the filled circle.
         * @param cy The y-coordinate of the center of the filled circle.
         * @param radius The radius of the filled circle.
         */
        void DrawCircle(float cx, float cy, float radius);

        /**
         * @brief Draw a filled circle on the renderer.
         *
         * This function draws a filled circle on the renderer using the provided Circle.
         *
         * @param circle The Circle that defines the center and radius of the filled circle to be drawn.
         */
        void DrawCircle(const shape2D::Circle& circle);

        /**
         * @brief Draw a filled circle on the renderer.
         *
         * This function draws a filled circle on the renderer using the provided center point and radius.
         *
         * @param center The center point of the filled circle.
         * @param radius The radius of the filled circle.
         */
        void DrawCircle(const math::Vec2& center, float radius);

        /**
         * @brief Draw a filled circle on the renderer.
         *
         * This function draws a filled circle on the renderer using the provided center point and radius.
         *
         * @param center The center point of the filled circle.
         * @param radius The radius of the filled circle.
         */
        void DrawCircle(const math::IVec2& center, int radius);

        /**
         * @brief Draw a filled circle on the renderer.
         *
         * This function draws a filled circle on the renderer using the provided integer coordinates and radius.
         *
         * @param x The x-coordinate of the center of the filled circle.
         * @param y The y-coordinate of the center of the filled circle.
         * @param radius The radius of the filled circle.
         */
        void DrawCircle(int x, int y, int radius);

        /**
         * @brief Draw the outline of a circle on the renderer.
         *
         * This function draws the outline of a circle on the renderer using the provided float coordinates and radius.
         *
         * @param cx The x-coordinate of the center of the circle outline.
         * @param cy The y-coordinate of the center of the circle outline.
         * @param radius The radius of the circle outline.
         */
        void DrawCircleLines(float cx, float cy, float radius);

        /**
         * @brief Draw the outline of a circle on the renderer.
         *
         * This function draws the outline of a circle on the renderer using the provided Circle.
         *
         * @param circle The Circle that defines the center and radius of the circle outline to be drawn.
         */
        void DrawCircleLines(const shape2D::Circle& circle);

        /**
         * @brief Draw the outline of a circle on the renderer.
         *
         * This function draws the outline of a circle on the renderer using the provided center point and radius.
         *
         * @param center The center point of the circle outline.
         * @param radius The radius of the circle outline.
         */
        void DrawCircleLines(const math::Vec2& center, float radius);

        /**
         * @brief Draw the outline of a circle on the renderer.
         *
         * This function draws the outline of a circle on the renderer using the provided center point and radius.
         *
         * @param center The center point of the circle outline.
         * @param radius The radius of the circle outline.
         */
        void DrawCircleLines(const math::IVec2& center, int radius);

        /**
         * @brief Draw the outline of a circle on the renderer.
         *
         * This function draws the outline of a circle on the renderer using the provided integer coordinates and radius.
         *
         * @param x The x-coordinate of the center of the circle outline.
         * @param y The y-coordinate of the center of the circle outline.
         * @param radius The radius of the circle outline.
         */
        void DrawCircleLines(int x, int y, int radius);

        /**
         * @brief Draw the vertices of a 2D mesh on the renderer.
         *
         * This function draws the vertices of a 2D mesh on the renderer using the provided mesh data.
         *
         * @param mesh The 2D mesh data containing vertices and indices.
         * @param texture Optional parameter specifying the texture to apply to the mesh.
         *                If nullptr, no texture will be applied.
         */
        void DrawMesh(const shape2D::Mesh& mesh, SDL_Texture* texture = nullptr);

        /**
         * @brief Draw the vertices of a 2D geometry on the renderer.
         *
         * This function draws the vertices of a 2D geometry on the renderer using the provided vertex data.
         *
         * @param vertices The vector of 2D vertices representing the geometry.
         * @param texture Optional parameter specifying the texture to apply to the geometry.
         *                If nullptr, no texture will be applied.
         */
        void DrawGeometry(const std::vector<shape2D::Vertex>& vertices, SDL_Texture* texture = nullptr);
    };

}}

#endif //NEXUS_EXT_GFX_RENDERER_HPP
