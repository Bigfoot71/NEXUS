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

#ifndef NEXUS_EXT_GFX_TEXTURE_HPP
#define NEXUS_EXT_GFX_TEXTURE_HPP

#include "../cmn_ext_gfx_gapi_impl/nxTexturePatch.hpp"
#include "../../shape/2D/nxRectangle.hpp"
#include "../../utils/nxContextual.hpp"
#include "../../core/nxException.hpp"
#include "../../math/nxVec2.hpp"
#include "./nxRenderer.hpp"
#include "../nxSurface.hpp"
#include "../nxPixel.hpp"
#include "../nxColor.hpp"

#include <SDL_stdinc.h>
#include <SDL_render.h>
#include <utility>

namespace nexus { namespace gfx {

    /**
     * @brief Enumeration of texture access types.
     *
     * This enumeration represents different access types for SDL textures.
     */
    enum class TextureAccess : int
    {
        Static      = SDL_TEXTUREACCESS_STATIC,         ///< Changes rarely, not lockable
        Streaming   = SDL_TEXTUREACCESS_STREAMING,      ///< Changes frequently, lockable
        Target      = SDL_TEXTUREACCESS_TARGET          ///< Texture can be used as a render target
    };

    /**
     * @brief Enumeration for texture flipping options.
     *
     * This enumeration represents different flipping options for rendering textures.
     */
    enum class TextureFlip : Uint32
    {
        None        = SDL_FLIP_NONE,            ///< Do not flip
        Horizontal  = SDL_FLIP_HORIZONTAL,      ///< Flip horizontally
        Vertical    = SDL_FLIP_VERTICAL         ///< Flip vertically
    };

    /**
     * @brief Alias ​​to use the NinePatchInfo implementation from this namespace.
     */
    using NinePatchInfo = _ext_gfx_gapi_impl::NinePatchInfo;

}}

namespace _gfx_impl {

    class Texture : public nexus::utils::Contextual<nexus::gfx::Renderer>
    {
      protected:
        SDL_Texture *data = nullptr;    ///< The SDL texture.
        int width = 0, height = 0;      ///< Texture dimensions.

      private:
        void Create(int w, int h, nexus::gfx::PixelFormat format, nexus::gfx::TextureAccess access);
        void LoadFromImageFile(const std::string& filename);
        void LoadFromSurface(nexus::gfx::Surface& surface);

      public:
        Texture(nexus::gfx::Renderer& ctx);
        Texture(nexus::gfx::Renderer& ctx, nexus::gfx::Surface& surface);
        Texture(nexus::gfx::Renderer& ctx, nexus::gfx::Surface&& surface);
        Texture(nexus::gfx::Renderer& ctx, const std::string& fileName);
        Texture(nexus::gfx::Renderer& ctx, int w, int h, nexus::gfx::PixelFormat format, nexus::gfx::TextureAccess access);

        ~Texture();

        /**
         * @brief Implicit conversion operator to retrieve a non-const pointer to the SDL_Texture.
         *
         * This operator allows implicit conversion to a non-const pointer to the SDL_Texture.
         * Use it with caution to prevent unintended modifications.
         *
         * @return A non-const pointer to the SDL_Texture.
         */
        operator SDL_Texture*() { return data; }

        /**
         * @brief Implicit conversion operator to retrieve a const pointer to the SDL_Texture.
         *
         * This operator allows implicit conversion to a const pointer to the SDL_Texture.
         * It can be used for read-only access.
         *
         * @return A const pointer to the SDL_Texture.
         */
        operator const SDL_Texture*() const { return data; }

        /**
         * @brief Draws a portion of the texture onto the screen.
         *
         * This function draws a portion of the texture onto the screen with optional transformations.
         *
         * @param src The source rectangle on the texture.
         * @param dst The destination rectangle on the screen.
         * @param origin The rotation origin point (optional, default is {0, 0}).
         * @param rotation The rotation angle in degrees (optional, default is 0.0f).
         * @param flip The texture flipping options (optional, default is nexus::gfx::TextureFlip::None).
         */
        void Draw(const nexus::shape2D::Rectangle& src, const nexus::shape2D::RectangleF& dst, const nexus::math::Vec2& origin = {}, float rotation = 0.0f, nexus::gfx::TextureFlip flip = nexus::gfx::TextureFlip::None) const;

        /**
         * @brief Draws the entire texture at a specific position.
         *
         * @param x The x-coordinate on the screen.
         * @param y The y-coordinate on the screen.
         */
        void Draw(float x, float y) const;

        /**
         * @brief Draws the entire texture at a specific position.
         *
         * @param position The position on the screen.
         */
        void Draw(const nexus::math::Vec2& position) const;

        /**
         * @brief Draws the texture with rotation, scale, and flipping options.
         *
         * @param position The position on the screen.
         * @param rotation The rotation angle in degrees.
         * @param scale The scaling factor (default is 1).
         * @param flip The texture flipping options (optional, default is nexus::gfx::TextureFlip::None).
         */
        void Draw(const nexus::math::Vec2& position, float rotation, float scale, nexus::gfx::TextureFlip flip = nexus::gfx::TextureFlip::None) const;

        /**
         * @brief Draws the texture with rotation, origin point, scale, and flipping options.
         *
         * @param position The position on the screen.
         * @param rotation The rotation angle in degrees.
         * @param origin The rotation origin point (optional, default is {0, 0}).
         * @param scale The scaling factor (optional, default is {1, 1}).
         * @param flip The texture flipping options (optional, default is nexus::gfx::TextureFlip::None).
         */
        void Draw(const nexus::math::Vec2& position, float rotation, const nexus::math::Vec2& origin = { 0, 0 }, const nexus::math::Vec2& scale = { 1, 1 }, nexus::gfx::TextureFlip flip = nexus::gfx::TextureFlip::None) const;

        /**
         * @brief Draws a portion of the texture at a specific position with optional flipping.
         *
         * @param src The source rectangle on the texture.
         * @param position The position on the screen.
         * @param flip The texture flipping options (optional, default is nexus::gfx::TextureFlip::None).
         */
        void Draw(const nexus::shape2D::Rectangle& src, const nexus::math::Vec2& position, nexus::gfx::TextureFlip flip = nexus::gfx::TextureFlip::None) const;

        /**
         * @brief Draws a Nine Patch texture at a specific position with optional rotation and tinting.
         *
         * @param ninePatchInfo The Nine Patch information.
         * @param dest Destination rectangle to draw the Nine Patch texture.
         * @param origin Origin point for rotation and scaling.
         * @param rotation Rotation angle in degrees.
         * @param tint Tint color for the Nine Patch texture.
         */
        void DrawNinePatch(nexus::gfx::NinePatchInfo ninePatchInfo, const nexus::shape2D::RectangleF& dest, const nexus::math::Vec2& origin, float rotation, const nexus::gfx::Color& tint) const;

        /**
         * @brief Updates the SDL_Texture with pixel data.
         *
         * This function updates the texture with the provided pixel data.
         *
         * @param pixels Pointer to the pixel data.
         * @param pitch The pitch of the pixel data.
         * @param dest The destination rectangle on the texture (optional, default is full texture).
         *
         * @throws nexus::core::NexusException if the texture update fails.
         */
        void Update(const void* pixels, int pitch, const nexus::shape2D::Rectangle& dest = {});

        /**
         * @brief Updates the SDL_Texture with pixel data from a Surface.
         *
         * This function updates the texture with the pixel data from the provided surface.
         *
         * @param surface The source surface containing pixel data.
         * @param dest The destination rectangle on the texture (optional, default is full texture).
         *
         * @throws nexus::core::NexusException if the texture update fails.
         */
        void Update(const nexus::gfx::Surface& surface, nexus::shape2D::Rectangle dest = {});

        /**
         * @brief Get the pixel format of the texture.
         *
         * This function retrieves the pixel format of the texture.
         *
         * @return The pixel format of the texture.
         */
        nexus::gfx::PixelFormat GetFormat() const;

        /**
         * @brief Get the access type of the texture.
         *
         * This function retrieves the access type of the texture, indicating whether it is static, streaming, or a render target.
         *
         * @return The access type of the texture.
         */
        nexus::gfx::TextureAccess GetAccess() const;

        /**
         * @brief Gets the width of the texture.
         *
         * @return The width of the texture.
         */
        int GetWidth() const { return width; }

        /**
         * @brief Gets the height of the texture.
         *
         * @return The height of the texture.
         */
        int GetHeight() const { return height; }

        /**
         * @brief Gets the width of the texture as a floating-point value.
         *
         * @return The width of the texture as a float.
         */
        float GetWidthF() const { return static_cast<float>(width); }

        /**
         * @brief Gets the height of the texture as a floating-point value.
         *
         * @return The height of the texture as a float.
         */
        float GetHeightF() const { return static_cast<float>(height); }

        /**
         * @brief Gets the size of the texture as a 2D integer vector.
         *
         * @return The size of the texture as a nexus::math::IVec2.
         */
        nexus::math::IVec2 GetSize() const { return { width, height }; }

        /**
         * @brief Gets the size of the texture as a Rectangle with (0, 0) as the origin.
         *
         * @return The size of the texture as a nexus::shape2D::Rectangle.
         */
        nexus::shape2D::Rectangle GetRectSize() const { return { 0, 0, width, height }; }

        /**
         * @brief Get the blend mode of the texture.
         *
         * This function retrieves the blend mode currently set for the texture and returns it.
         * If an error occurs during the retrieval, a NexusException is thrown with relevant details.
         *
         * @return The blend mode of the texture.
         * @throws core::NexusException if an error occurs during retrieval.
         */
        nexus::gfx::BlendMode GetBlendMode() const;

        /**
         * @brief Set the blend mode of the texture.
         *
         * This function sets the blend mode for the texture based on the provided BlendMode enum.
         * If an error occurs during the set operation, a NexusException is thrown with relevant details.
         *
         * @param blendMode The blend mode to set for the texture.
         * @throws core::NexusException if an error occurs during the set operation.
         */
        void SetBlendMode(nexus::gfx::BlendMode blendMode);

        /**
         * @brief Get the tint (color modulation) of the texture.
         *
         * This function retrieves the tint (color modulation) currently set for the texture
         * and returns it as a gfx::Color object. The tint consists of RGB color modulation
         * and an alpha modulation. If an error occurs during the retrieval, such as an invalid
         * texture, the returned color will be all zeros.
         *
         * @return The tint (color modulation) of the texture.
         */
        nexus::gfx::Color GetTint() const;

        /**
         * @brief Set the tint (color modulation) of the texture.
         *
         * This function sets the tint (color modulation) for the texture based on the provided
         * gfx::Color object. The tint consists of RGB color modulation and an alpha modulation.
         * If an error occurs during the set operation, such as an invalid texture or invalid color
         * values, no modifications are made to the texture.
         *
         * @param tint The tint (color modulation) to set for the texture.
         */
        void SetTint(const nexus::gfx::Color& tint);
    };

}

namespace nexus { namespace gfx {

    /**
     * @brief An encapsulation class for SDL_Texture.
     *
     * The Texture class serves as a C++ wrapper for SDL_Texture and simplifies texture creation,
     * rendering, and management in SDL applications. It does not manage the memory of the SDL_Texture.
     *
     * @note: This class is the one that should be used for rendering on a Renderer because its data
     * will be managed at the video driver level (see SDL implementation). However, this class cannot
     * be used by the software rasterizer; for that, you will need the sr::Texture class.
     *
     */
    class NEXUS_API Texture : public utils::Container<_gfx_impl::Texture>
    {
      public:
        using Access = TextureAccess;
        using Flip = TextureFlip;

      public:
        /**
         * @brief Constructor to create a Texture object of a single pixel.
         * @note: This constructor exists to match gl::Texture(gl::Context&)
         *
         * @param renderer The SDL renderer associated with the texture.
         */
        Texture(nexus::gfx::Renderer& ctx)
        : Container(ctx)
        { }

        /**
         * @brief Constructor to create a Texture object from a Surface object.
         *
         * This constructor takes a `Renderer` and a `Surface` object and creates a texture from the provided Surface using the given renderer.
         *
         * @param renderer The SDL renderer associated with the texture.
         * @param surface The Surface object to create the texture from.
         */
        Texture(nexus::gfx::Renderer& ctx, gfx::Surface& surface)
        : Container(ctx, surface)
        { }

        /**
         * @brief Constructor to create a Texture object from a Surface object (move semantics).
         *
         * This constructor takes a `Renderer` and a rvalue reference to a `Surface` object and creates a texture from the provided Surface using the given renderer.
         * The ownership of the Surface data is transferred. The surface will be immediately destroyed after the call.
         *
         * @param renderer The SDL renderer associated with the texture.
         * @param surface The Surface object to create the texture from (will be moved and destroyed).
         */
        Texture(nexus::gfx::Renderer& ctx, gfx::Surface&& surface)
        : Container(ctx, surface)
        { }

        /**
         * @brief Constructor to create a Texture object from an image file.
         *
         * @param renderer The SDL renderer associated with the texture.
         * @param filename The path to the image file to load and create the texture from.
         */
        Texture(nexus::gfx::Renderer& ctx, const std::string& fileName)
        : Container(ctx, fileName)
        { }

        /**
         * @brief Constructor to create a Texture object with a specified format, access type, width, and height.
         *
         * @param renderer The SDL renderer associated with the texture.
         * @param format The pixel format of the texture.
         * @param access The access type for the texture.
         * @param w The width of the texture.
         * @param h The height of the texture.
         */
        Texture(nexus::gfx::Renderer& ctx, int w, int h, gfx::PixelFormat format, Access access)
        : Container(ctx, w, h, format, access)
        { }

        /**
         * @brief Implicit conversion operator to retrieve a non-const pointer to the SDL_Texture.
         *
         * This operator allows implicit conversion to a non-const pointer to the SDL_Texture.
         * Use it with caution to prevent unintended modifications.
         *
         * @return A non-const pointer to the SDL_Texture.
         */
        operator SDL_Texture*() { return *obj; }

        /**
         * @brief Implicit conversion operator to retrieve a const pointer to the SDL_Texture.
         *
         * This operator allows implicit conversion to a const pointer to the SDL_Texture.
         * It can be used for read-only access.
         *
         * @return A const pointer to the SDL_Texture.
         */
        operator const SDL_Texture*() const { return *obj; }
    };

}}

#endif //NEXUS_EXT_GFX_TEXTURE_HPP
