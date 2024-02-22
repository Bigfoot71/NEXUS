#ifndef NEXUS_SR_TEXTURE_HPP
#define NEXUS_SR_TEXTURE_HPP

#include "../../gfx/cmn_ext_gfx_gapi_impl/nxTexturePatch.hpp"

#include "../../gfx/nxSurface.hpp"
#include "../../gfx/nxColor.hpp"
#include "../../gfx/nxPixel.hpp"

#include "./nxContextual.hpp"
#include "./nxCamera3D.hpp"

namespace nexus { namespace sr {
    using NinePatchInfo = _ext_gfx_gapi_impl::NinePatchInfo;
}}

namespace _sr_impl {

    class Texture : public nexus::sr::SoftContextual, public nexus::gfx::Surface
    {
      public:
        Texture(nexus::sr::Context& ctx)
        : nexus::sr::SoftContextual(ctx)
        , Surface(1, 1, nexus::gfx::White)
        { }

        Texture(nexus::sr::Context& ctx, SDL_Surface* sdlSurface, bool autoLifetimeManagement)
        : nexus::sr::SoftContextual(ctx)
        , Surface(sdlSurface, autoLifetimeManagement)
        { }

        Texture(nexus::sr::Context& ctx, int width, int height, const nexus::gfx::Color& color, nexus::gfx::PixelFormat format)
        : nexus::sr::SoftContextual(ctx)
        , Surface(width, height, color, format)
        { }

        Texture(nexus::sr::Context& ctx, const std::string& filePath)
        : nexus::sr::SoftContextual(ctx)
        , Surface(filePath)
        { }

        Texture(nexus::sr::Context& ctx, const std::vector<Uint8>& data)
        : nexus::sr::SoftContextual(ctx)
        , Surface(data)
        { }

        Texture(nexus::sr::Context& ctx, const std::vector<Uint8>& data, nexus::core::ImageFormat format)
        : nexus::sr::SoftContextual(ctx)
        , Surface(data, format)
        { }

        Texture(nexus::sr::Context& ctx, const void* data, size_t size)
        : nexus::sr::SoftContextual(ctx)
        , Surface(data, size)
        { }

        Texture(nexus::sr::Context& ctx, const void* data, size_t size, nexus::core::ImageFormat format)
        : nexus::sr::SoftContextual(ctx)
        , Surface(data, size, format)
        { }

        /**
         * @brief Draw the texture onto another texture or screen, specifying source and destination rectangles.
         * 
         * @param src The source rectangle from which to draw the texture.
         * @param dst The destination rectangle where the texture will be drawn.
         * @param origin The origin point around which to rotate and scale the texture (default is {0, 0}).
         * @param rotation The rotation angle of the texture in radians (default is 0.0f).
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
         * @param rotation The rotation angle of the texture in radians.
         * @param scale The scaling factor to apply to the texture.
         * @param tint The color tint to apply to the texture (default is nexus::gfx::White).
         */
        void Draw(const nexus::math::Vec2& position, float rotation, float scale, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draw the texture at the specified position with rotation, origin, scaling, and optional color tinting.
         * 
         * @param position The position at which to draw the texture.
         * @param rotation The rotation angle of the texture in radians.
         * @param origin The origin point around which to rotate and scale the texture (default is {0, 0}).
         * @param scale The scaling factor to apply to the texture (default is {1, 1}).
         * @param tint The color tint to apply to the texture (default is nexus::gfx::White).
         */
        void Draw(const nexus::math::Vec2& position, float rotation, const nexus::math::Vec2& origin = { 0, 0 }, const nexus::math::Vec2& scale = { 1, 1 }, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draw the texture onto another texture or screen at the specified position, using a source rectangle.
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
         * @param rotation The rotation angle of the texture in radians (default is 0.0f).
         * @param tint The color tint to apply to the texture (default is nexus::gfx::White).
         */
        void DrawNinePatch(nexus::sr::NinePatchInfo ninePatchInfo, const nexus::shape2D::RectangleF& dest, const nexus::math::Vec2& origin = { 0, 0 }, float rotation = 0.0f, const nexus::gfx::Color& tint = nexus::gfx::White);

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
        void DrawBillboard(const nexus::sr::Camera3D& camera, nexus::shape2D::Rectangle source, const nexus::math::Vec3& position, const nexus::math::Vec3& up, const nexus::math::Vec2& size, const nexus::math::Vec2& origin, float rotation = 0.0f, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draw a textured billboard in 3D space with a specified size.
         * 
         * @param camera The camera through which the billboard will be rendered.
         * @param position The position of the billboard in 3D space.
         * @param size The size of the billboard.
         * @param rotation The rotation angle of the billboard in degrees.
         * @param tint The color tint to apply to the billboard.
         */
        void DrawBillboard(const nexus::sr::Camera3D& camera, const nexus::math::Vec3& position, float size, float rotation = 0.0f, const nexus::gfx::Color& tint = nexus::gfx::White) const;

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
        void DrawBillboard(const nexus::sr::Camera3D& camera, const nexus::shape2D::Rectangle& source, const nexus::math::Vec3& position, const nexus::math::Vec2& size, float rotation = 0.0f, const nexus::gfx::Color& tint = nexus::gfx::White) const;
    };

}

namespace nexus { namespace sr {

    /**
     * @brief The `Texture` class represents a `gfx::Surface` linked to an instance of `sr::Context`.
     *
     * The primary purpose of this class is to render "textures" using the software rasterizer.
     * It is recommended for use in scenarios where rendering with a software rasterizer is desired.
     * Alternatively, consider using `gfx::Surface` directly for more general rendering purposes.
     *
     * Unlike `gfx::Texture`, which is designed to be used with `sr::Renderer` and may leverage
     * hardware acceleration if supported by the underlying hardware, `Texture` is specifically
     * tailored for software-based rendering.
     */
    class NEXUS_API Texture : public Container<_sr_impl::Texture>
    {
      public:
        /**
         * @brief Constructor to create a white 1x1 pixel Texture with the RGBA32 format.
         *
         * This constructor creates a new Texture with a white 1x1 pixel, RGBA32 format.
         * It is linked to the provided software rasterizer context.
         *
         * @param ctx The software rasterizer context to link the Texture to.
         */
        Texture(sr::Context& ctx)
        : Container(ctx)
        { }

        /**
         * @brief Constructor to create a Texture object from an existing surface.
         *
         * @param ctx The software rasterizer context to link the Texture to.
         * @param sdlSurface The existing SDL surface to encapsulate.
         */
        Texture(sr::Context& ctx, SDL_Surface* sdlSurface)
        : Container(ctx, sdlSurface, true)
        { }

        /**
         * @brief Constructor to create a Surface object with the specified width, height, and pixel format.
         *
         * This constructor creates a new SDL surface with the specified width, height, and pixel format.
         * If the surface creation fails, it throws a throw with an error message indicating the SDL error.
         *
         * @param ctx The software rasterizer context to link the Texture to.
         * @param width The width of the surface.
         * @param height The height of the surface.
         * @param color Optional fill color for initializing the surface (default is gfx::Blank).
         * @param format Optional pixel format of the surface (default is PixelFormat::RGBA32).
         */
        Texture(sr::Context& ctx, int width, int height, const gfx::Color& color = gfx::Blank, gfx::PixelFormat format = gfx::PixelFormat::RGBA32)
        : Container(ctx, width, height, color, format)
        { }

        /**
         * @brief Constructor to create a Surface object by loading an image from a file.
         *
         * This constructor loads an image file and creates an SDL surface from it.
         * If the image loading or surface creation fails, it throws a throw
         * with an error message indicating the SDL or SDL_image error.
         *
         * @param ctx The software rasterizer context to link the Texture to.
         * @param filePath The path to the image file to load.
         */
        Texture(sr::Context& ctx, const std::string& filePath)
        : Container(ctx, filePath)
        { }

        /**
         * @brief Constructs a Surface from image data stored in a vector of Uint8.
         *
         * This constructor loads an image from the provided vector of Uint8 data
         * and creates a Surface object. It attempts to automatically determine the image format.
         * If the image loading or surface creation fails, it throws an exception
         * with an error message indicating the SDL error.
         *
         * @param ctx The software rasterizer context to link the Texture to.
         * @param data The vector containing the image data.
         * @throws core::NexusException if the image loading or surface creation fails.
         */
        Texture(sr::Context& ctx, const std::vector<Uint8>& data)
        : Container(ctx, data)
        { }

        /**
         * @brief Constructs a Surface from image data stored in a vector of Uint8 with a specified format.
         *
         * This constructor loads an image from the provided vector of Uint8 data
         * and creates a Surface object. It allows specifying the image format to use.
         * If the image loading or surface creation fails, it throws an exception
         * with an error message indicating the SDL error.
         *
         * @param ctx The software rasterizer context to link the Texture to.
         * @param data The vector containing the image data.
         * @param format The format of the image data.
         * @throws core::NexusException if the image loading or surface creation fails.
         */
        Texture(sr::Context& ctx, const std::vector<Uint8>& data, core::ImageFormat format)
        : Container(ctx, data, format)
        { }

        /**
         * @brief Constructs a Surface from image data stored in a block of memory.
         *
         * This constructor loads an image from the provided block of memory specified
         * by a const void pointer and its size. It attempts to automatically determine the image format.
         * If the image loading or surface creation fails, it throws an exception
         * with an error message indicating the SDL error.
         *
         * @param ctx The software rasterizer context to link the Texture to.
         * @param data A pointer to the start of the memory block containing the image data.
         * @param size The size of the memory block.
         * @throws core::NexusException if the image loading or surface creation fails.
         */
        Texture(sr::Context& ctx, const void* data, size_t size)
        : Container(ctx, data, size)
        { }

        /**
         * @brief Constructs a Surface from image data stored in a block of memory with a specified format.
         *
         * This constructor loads an image from the provided block of memory specified
         * by a const void pointer and its size. It also allows specifying the image format to use.
         * If the image loading or surface creation fails, it throws an exception
         * with an error message indicating the SDL error.
         *
         * @param ctx The software rasterizer context to link the Texture to.
         * @param data A pointer to the start of the memory block containing the image data.
         * @param size The size of the memory block.
         * @param format The format of the image data.
         * @throws core::NexusException if the image loading or surface creation fails.
         */
        Texture(sr::Context& ctx, const void* data, size_t size, core::ImageFormat format)
        : Container(ctx, data, size, format)
        { }

        /**
         * @brief Conversion operator to obtain a non-const pointer to the encapsulated gfx::Surface.
         *
         * This operator allows implicit conversion to a non-const pointer to the gfx::Surface
         * encapsulated by the Texture. It returns the underlying pointer obtained from the
         * SoftContextual (obj) using the get() method.
         *
         * @return A non-const pointer to the encapsulated gfx::Surface.
         */
        operator gfx::Surface*() { return obj.get(); }

        /**
         * @brief Conversion operator to obtain a const pointer to the encapsulated gfx::Surface.
         *
         * This operator allows implicit conversion to a const pointer to the gfx::Surface
         * encapsulated by the Texture. It returns the underlying pointer obtained from the
         * SoftContextual (obj) using the get() method.
         *
         * @return A const pointer to the encapsulated gfx::Surface.
         */
        operator const gfx::Surface*() const { return obj.get(); }

        /**
         * @brief Conversion operator to obtain a non-const reference to the encapsulated gfx::Surface.
         *
         * This operator allows implicit conversion to a non-const reference to the gfx::Surface
         * encapsulated by the Texture. It returns a reference to the underlying gfx::Surface
         * obtained by dereferencing the SoftContextual (obj) pointer.
         *
         * @return A non-const reference to the encapsulated gfx::Surface.
         */
        operator gfx::Surface&() { return *obj; }

        /**
         * @brief Conversion operator to obtain a const reference to the encapsulated gfx::Surface.
         *
         * This operator allows implicit conversion to a const reference to the gfx::Surface
         * encapsulated by the Texture. It returns a const reference to the underlying gfx::Surface
         * obtained by dereferencing the SoftContextual (obj) pointer.
         *
         * @return A const reference to the encapsulated gfx::Surface.
         */
        operator const gfx::Surface&() const { return *obj; }
    };

}}

#endif //NEXUS_SR_TEXTURE_HPP
