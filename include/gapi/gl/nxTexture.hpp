#ifndef NEXUS_GL_TEXTURE_HPP
#define NEXUS_GL_TEXTURE_HPP

#include "../../gfx/cmn_ext_gfx_gapi_impl/nxTexturePatch.hpp"

#include "../../core/nxException.hpp"
#include "../../gfx/nxSurface.hpp"
#include "../../gfx/nxPixel.hpp"
#include "../../math/nxMat4.hpp"

#include "./nxContextual.hpp"
#include "./nxCamera3D.hpp"
#include "./nxContext.hpp"
#include "./nxEnums.hpp"

#include <memory>

namespace nexus { namespace gl {
    using NinePatchInfo = _ext_gfx_gapi_impl::NinePatchInfo;
}}

namespace _gl_impl
{
    nexus::gl::TextureFormat ConvertPixelFormat(nexus::gfx::PixelFormat pf);

    class Texture : public nexus::gl::Contextual
    {
      public:
        enum class Wrap
        {
            Repeat,                 ///< Repeats texture in tiled mode
            Clamp,                  ///< Clamps texture to edge pixel in tiled mode
            MirrorRepeat,           ///< Mirrors and repeats the texture in tiled mode
            MirrorClamp             ///< Mirrors and clamps to border the texture in tiled mode
        };

        enum class Filter
        {
            Point,                  ///< No filter, just pixel approximation
            Bilinear,               ///< Linear filtering
            Trilinear,              ///< Trilinear filtering (linear with mipmaps)
            Anisotropic_4X,         ///< Anisotropic filtering 4x
            Anisotropic_8X,         ///< Anisotropic filtering 8x
            Anisotropic_16X,        ///< Anisotropic filtering 16x
        };

        enum class CubemapLayout
        {
            AutoDetect,             ///< Automatically detect layout type
            LineVertical,           ///< Layout is defined by a vertical line with faces
            LineHorizontal,         ///< Layout is defined by a horizontal line with faces
            CrossThreeByFour,       ///< Layout is defined by a 3x4 cross with cubemap faces
            CrossFourByThree,       ///< Layout is defined by a 4x3 cross with cubemap faces
            Panorama                ///< Layout is defined by a panorama image (equirrectangular map)
        };

      private:
        Uint32 id                       = 0;                            ///< Texture ID.
        int width                       = 0;                            ///< Width of the texture.
        int height                      = 0;                            ///< Height of the texture.
        int mipmaps                     = 0;                            ///< Number of mipmaps.
        nexus::gl::TextureFormat format    = nexus::gl::TextureFormat(0);     ///< Pixel format of the texture.

      private:
        void LoadFromMemory(const nexus::gfx::Surface& surface);
        void Create(int w, int h, nexus::gl::TextureFormat format);

      public:
        /**
         * @brief Constructs a texture object with the provided OpenGL context.
         * 
         * @param ctx The OpenGL context.
         */
        Texture(nexus::gl::Context& ctx);

        /**
         * @brief Constructs a texture object by loading an image file from the specified file path.
         * 
         * @param ctx The OpenGL context.
         * @param filePath The path to the image file.
         */
        Texture(nexus::gl::Context& ctx, const std::string& filePath);

        /**
         * @brief Constructs a texture object from an existing surface.
         * 
         * @param ctx The OpenGL context.
         * @param surface The surface containing pixel data.
         */
        Texture(nexus::gl::Context& ctx, const nexus::gfx::Surface& surface);

        /**
         * @brief Constructs an empty texture object with the specified dimensions and format.
         * 
         * @param ctx The OpenGL context.
         * @param w The width of the texture.
         * @param h The height of the texture.
         * @param format The pixel format of the texture.
         */
        Texture(nexus::gl::Context& ctx, int w, int h, nexus::gl::TextureFormat format);

        /**
         * @brief Constructs a texture object from an existing texture ID with additional information.
         * 
         * @param ctx The OpenGL context.
         * @param id The ID of the existing texture.
         * @param width The width of the texture.
         * @param height The height of the texture.
         * @param mipmaps The number of mipmaps in the texture.
         * @param format The pixel format of the texture.
         */
        Texture(nexus::gl::Context& ctx, Uint32 id, int width, int height, int mipmaps, nexus::gl::TextureFormat format);

        /**
         * @brief Constructs a depth texture object with the specified dimensions and render buffer usage.
         * 
         * @param ctx The OpenGL context.
         * @param w The width of the texture.
         * @param h The height of the texture.
         * @param useRenderBuffer Whether to use a render buffer for depth storage.
         */
        Texture(nexus::gl::Context& ctx, int w, int h, bool useRenderBuffer);

        /**
         * @brief Constructs a cubemap texture object from an existing surface with the specified layout.
         * 
         * @param ctx The OpenGL context.
         * @param surface The surface containing cubemap images.
         * @param layout The layout type of the cubemap.
         */
        Texture(nexus::gl::Context& ctx, const nexus::gfx::Surface& surface, CubemapLayout layout);

        /**
         * @brief Destructor for the texture object.
         */
        ~Texture();

        /**
         * @brief Update the entire GPU texture with new pixel data from a surface.
         * 
         * @param image The surface containing the new pixel data.
         */
        void Update(const nexus::gfx::Surface& image);

        /**
         * @brief Update the entire GPU texture with new pixel data.
         * 
         * @param pixels A pointer to the new pixel data.
         * @note The pixel data must match the format of the texture.
         */
        void Update(const void *pixels);

        /**
         * @brief Update a rectangle area of the GPU texture with new pixel data from a surface.
         * 
         * @param image The surface containing the new pixel data.
         * @param rect The rectangle area within the texture to update.
         */
        void Update(const nexus::gfx::Surface& image, const nexus::shape2D::Rectangle& rect);

        /**
         * @brief Update a rectangle area of the GPU texture with new pixel data.
         * 
         * @param pixels A pointer to the new pixel data.
         * @param rect The rectangle area within the texture to update.
         * @note The pixel data must match the format of the texture.
         */
        void Update(const void *pixels, const nexus::shape2D::Rectangle& rect);

        /**
         * @brief Generate mipmaps for the GPU texture.
         * 
         * This function generates mipmaps for the texture, improving rendering quality when the texture is scaled down.
         */
        void GenMipmaps();

        /**
         * @brief Set the texture wrapping mode.
         * 
         * This function sets how the texture should be sampled when texture coordinates
         * fall outside the range [0, 1].
         * 
         * @param wrap The wrapping mode to set.
         */
        void SetWrap(Wrap wrap);

        /**
         * @brief Set the texture scaling filter mode.
         * 
         * This function sets the filtering mode used when sampling the texture at different
         * scales. It considers mipmaps if available in the texture and sets the filter mode
         * accordingly for minification and magnification.
         * 
         * @param filter The filtering mode to set.
         */
        void SetFilter(Filter filter);

        /**
         * @brief Draw a portion of the texture onto another texture or screen, specifying source and destination rectangles.
         * 
         * @param src The source rectangle from which to draw the texture.
         * @param dst The destination rectangle where the texture will be drawn.
         * @param origin The origin point around which to rotate and scale the texture (default is {0, 0}).
         * @param rotation The rotation angle of the texture in degrees (default is 0.0f).
         * @param tint The color tint to apply to the texture (default is nexus::gfx::White).
         */
        void Draw(nexus::shape2D::RectangleF src, const nexus::shape2D::RectangleF& dst, const nexus::math::Vec2& origin = {}, float rotation = 0.0f, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draw the texture at the specified position with optional color tinting.
         * 
         * @param x The x-coordinate of the position to draw the texture.
         * @param y The y-coordinate of the position to draw the texture.
         * @param tint The color tint to apply to the texture (default is nexus::gfx::White).
         */
        void Draw(float x, float y, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draw the texture at the specified position with optional color tinting.
         * 
         * @param position The position at which to draw the texture.
         * @param tint The color tint to apply to the texture (default is nexus::gfx::White).
         */
        void Draw(const nexus::math::Vec2& position, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draw the texture at the specified position with rotation, scaling, and optional color tinting.
         * 
         * @param position The position at which to draw the texture.
         * @param rotation The rotation angle of the texture in degrees.
         * @param scale The scaling factor to apply to the texture.
         * @param tint The color tint to apply to the texture (default is nexus::gfx::White).
         */
        void Draw(const nexus::math::Vec2& position, float rotation, float scale, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draw the texture at the specified position with rotation, origin, scaling, and optional color tinting.
         * 
         * @param position The position at which to draw the texture.
         * @param rotation The rotation angle of the texture in degrees.
         * @param origin The origin point around which to rotate and scale the texture (default is {0, 0}).
         * @param scale The scaling factor to apply to the texture (default is {1, 1}).
         * @param tint The color tint to apply to the texture (default is nexus::gfx::White).
         */
        void Draw(const nexus::math::Vec2& position, float rotation, const nexus::math::Vec2& origin = { 0, 0 }, const nexus::math::Vec2& scale = { 1, 1 }, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draw a portion of the texture onto another texture or screen at the specified position, using a source rectangle.
         * 
         * @param src The source rectangle from which to draw the texture.
         * @param position The position at which to draw the texture.
         * @param tint The color tint to apply to the texture (default is nexus::gfx::White).
         */
        void Draw(const nexus::shape2D::RectangleF& src, const nexus::math::Vec2& position, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draw a nine-patch textured rectangle onto another texture or screen.
         * 
         * @param ninePatchInfo Information about the nine-patch texture.
         * @param dest The destination rectangle where the nine-patch texture will be drawn.
         * @param origin The origin point around which to rotate and scale the texture (default is {0, 0}).
         * @param rotation The rotation angle of the texture in degrees (default is 0.0f).
         * @param tint The color tint to apply to the texture (default is nexus::gfx::White).
         */
        void DrawNinePatch(nexus::gl::NinePatchInfo ninePatchInfo, const nexus::shape2D::RectangleF& dest, const nexus::math::Vec2& origin = { 0, 0 }, float rotation = 0.0f, const nexus::gfx::Color& tint = nexus::gfx::White);

        /**
         * @brief Draw a textured billboard in 3D space.
         * 
         * @param camera The camera through which the billboard will be rendered.
         * @param source The source rectangle from which to draw the texture.
         * @param position The position of the billboard in 3D space.
         * @param up The up vector of the billboard.
         * @param size The size of the billboard.
         * @param origin The origin point around which to rotate and scale the texture.
         * @param rotation The rotation angle of the billboard in degrees.
         * @param tint The color tint to apply to the billboard.
         */
        void DrawBillboard(const nexus::gl::Camera3D& camera, nexus::shape2D::Rectangle source, const nexus::math::Vec3& position, const nexus::math::Vec3& up, const nexus::math::Vec2& size, const nexus::math::Vec2& origin, float rotation = 0.0f, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draw a textured billboard in 3D space with a specified size.
         * 
         * @param camera The camera through which the billboard will be rendered.
         * @param position The position of the billboard in 3D space.
         * @param size The size of the billboard.
         * @param rotation The rotation angle of the billboard in degrees.
         * @param tint The color tint to apply to the billboard.
         */
        void DrawBillboard(const nexus::gl::Camera3D& camera, const nexus::math::Vec3& position, float size, float rotation = 0.0f, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draw a textured billboard in 3D space with a specified source rectangle and size.
         * 
         * @param camera The camera through which the billboard will be rendered.
         * @param source The source rectangle from which to draw the texture.
         * @param position The position of the billboard in 3D space.
         * @param size The size of the billboard.
         * @param rotation The rotation angle of the billboard in degrees.
         * @param tint The color tint to apply to the billboard.
         */
        void DrawBillboard(const nexus::gl::Camera3D& camera, const nexus::shape2D::Rectangle& source, const nexus::math::Vec3& position, const nexus::math::Vec2& size, float rotation = 0.0f, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Get the width of the texture.
         * 
         * @return The width of the texture.
         */
        int GetWidth() const { return width; }

        /**
         * @brief Get the height of the texture.
         * 
         * @return The height of the texture.
         */
        int GetHeight() const { return height; }

        /**
         * @brief Get the width of the texture as a floating-point value.
         * 
         * @return The width of the texture as a floating-point value.
         */
        float GetWidthF() const { return static_cast<float>(width); }

        /**
         * @brief Get the height of the texture as a floating-point value.
         * 
         * @return The height of the texture as a floating-point value.
         */
        float GetHeightF() const { return static_cast<float>(height); }

        /**
         * @brief Get the size of the texture as a 2D integer vector.
         * 
         * @return The size of the texture as a 2D integer vector (width, height).
         */
        nexus::math::IVec2 GetSize() const { return { width, height }; }

        /**
         * @brief Get the size of the texture as a rectangle.
         * 
         * @return The size of the texture as a rectangle with origin at (0, 0) and dimensions (width, height).
         */
        nexus::shape2D::Rectangle GetRectSize() const { return { 0, 0, width, height }; }

        /**
         * @brief Get the ID of the texture.
         * 
         * @return The ID of the texture.
         */
        Uint32 GetID() const { return id; }
    };
}

namespace nexus { namespace gl {

    class NEXUS_API Texture : public Container<_gl_impl::Texture>
    {
      public:
        using Format = TextureFormat;
        using Wrap = _gl_impl::Texture::Wrap;
        using Filter = _gl_impl::Texture::Filter;
        using CubemapLayout = _gl_impl::Texture::CubemapLayout;

      public:
        static TextureFormat ConvertPixelFormat(nexus::gfx::PixelFormat pf)
        {
            return _gl_impl::ConvertPixelFormat(pf);
        }

      public:
        /**
         * @brief Constructor that loads the texture by default.
         * @param ctx The OpenGL context.
         */
        Texture(nexus::gl::Context& ctx)
        : Container(ctx)
        { }

        /**
         * @brief Constructor that loads a texture from a file.
         * @param ctx The OpenGL context.
         * @param filePath The path to the texture file.
         */
        Texture(gl::Context& ctx, const std::string& filePath)
        : Container(ctx, filePath)
        { }

        /**
         * @brief Constructor that loads a texture from an in-memory surface.
         * @param ctx The OpenGL context.
         * @param surface The in-memory surface.
         */
        Texture(gl::Context& ctx, const gfx::Surface& surface)
        : Container(ctx, surface)
        { }

        /**
         * @brief Constructor that creates an empty texture.
         * @param ctx The OpenGL context.
         * @param w Width of the texture.
         * @param h Height of the texture.
         * @param format Format of the texture.
         */
        Texture(gl::Context& ctx, int w, int h, Format format)
        : Container(ctx, w, h, format)
        { }

        /**
         * @brief Constructor that creates a depth texture.
         * @param ctx The OpenGL context.
         * @param w Width of the texture.
         * @param h Height of the texture.
         * @param useRenderBuffer Whether to use a render buffer.
         */
        Texture(gl::Context& ctx, int w, int h, bool useRenderBuffer)
        : Container(ctx, w, h, useRenderBuffer)
        { }

        /**
         * @brief Constructor that creates a cubemap texture.
         * @param ctx The OpenGL context.
         * @param surface The in-memory surface.
         * @param layout Layout of the cubemap.
         */
        Texture(gl::Context& ctx, const nexus::gfx::Surface& surface, CubemapLayout layout)
        : Container(ctx, surface, layout)
        { }

        /**
         * @brief Constructor that creates a texture with specified parameters.
         * @param ctx The OpenGL context.
         * @param id The OpenGL texture ID.
         * @param width Width of the texture.
         * @param height Height of the texture.
         * @param mipmaps Number of mipmaps.
         * @param format Format of the texture.
         */
        Texture(nexus::gl::Context& ctx, Uint32 id, int width, int height, int mipmaps, Format format)
        : Container(ctx, id, width, height, mipmaps, format)
        { }
    };

}}

#endif //NEXUS_GL_TEXTURE_HPP
