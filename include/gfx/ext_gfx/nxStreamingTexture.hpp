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

#ifndef NEXUS_EXT_GFX_STREAMING_TEXTURE_HPP
#define NEXUS_EXT_GFX_STREAMING_TEXTURE_HPP

#include "../../shape/2D/nxRectangle.hpp"
#include "../../gfx/nxPixel.hpp"
#include "./nxTexture.hpp"
#include <SDL_surface.h>
#include <SDL_pixels.h>
#include <SDL_render.h>

namespace _gfx_impl {

    class StreamingTexture : public Texture, public nexus::gfx::Surface
    {
      private:
        nexus::shape2D::Rectangle lockedArea; ///< The locked area of the texture.
        SDL_PixelFormat *format;              ///< The pixel format of the texture.

      public:
        /**
         * @brief Constructor for StreamingTexture.
         * @param ctx The graphics renderer context.
         * @param w The width of the texture.
         * @param h The height of the texture.
         * @param format The pixel format of the texture.
         */
        StreamingTexture(nexus::gfx::Renderer& ctx, int w, int h, nexus::gfx::PixelFormat format)
        : Texture(ctx, w, h, format, nexus::gfx::TextureAccess::Streaming)
        {
            this->format = SDL_AllocFormat(static_cast<Uint32>(format));
        }

        /**
         * @brief Destructor for StreamingTexture.
         *        Unlocks the texture if it is currently locked and frees the pixel format.
         */
        ~StreamingTexture()
        {
            if (surface) Unlock();
            SDL_FreeFormat(format);
        }
        
        /**
         * @note: Specifies explicit usage of functions from the Texture class,
         *        overriding those from gfx::Surface, to avoid unintended behavior.
         */

        using Texture::GetFormat;
        using Texture::GetWidth;
        using Texture::GetHeight;
        using Texture::GetWidthF;
        using Texture::GetHeightF;
        using Texture::GetSize;
        using Texture::GetRectSize;

        /**
         * @brief Indicates whether locking the texture is required for pixel access.
         * @return True, as locking is necessary for this type of texture.
         */
        bool MustLock() const override { return true; }

        /**
         * @brief Checks if the texture is currently locked.
         * @return True if the texture is locked, false otherwise.
         */
        bool IsLocked() const override { return this->surface != nullptr; }

        /**
         * @brief Checks if the texture is valid.
         * @return Always returns true as StreamingTexture is assumed to be valid.
         */
        bool IsValid() const override { return true; }

        /**
         * @brief Locks the entire texture for direct pixel manipulation.
         *        Equivalent to calling Lock(const shape2D::Rectangle& area) with the entire texture area.
         */
        void Lock() override { this->Lock({ 0, 0, width, height }); }

        /**
         * @brief Locks a portion of the texture for direct access to pixel data.
         * @param area The area of the texture to lock.
         * @throws core::NexusException if unable to lock the StreamingTexture.
         */
        void Lock(const nexus::shape2D::Rectangle& area);

        /**
         * @brief Unlocks the previously locked texture.
         */
        void Unlock() override;

        /**
         * @brief Gets the locked area of the texture.
         * @return A reference to the locked area rectangle.
         */
        const nexus::shape2D::Rectangle& GetLockedArea() const { return lockedArea; }

        /**
         * @brief Gets the pixel format of the texture.
         * @return A pointer to the SDL_PixelFormat structure representing the pixel format.
         */
        const SDL_PixelFormat* GetPixelFormat() const { return format; }

        /**
         * @brief Draws the specified Surface onto this StreamingTexture at the specified position.
         * @param other The Surface to draw onto this StreamingTexture.
         * @param x The x-coordinate of the top-left corner of the destination rectangle.
         * @param y The y-coordinate of the top-left corner of the destination rectangle.
         * @param ox The x-coordinate of the origin of the source rectangle.
         * @param oy The y-coordinate of the origin of the source rectangle.
         * @return A reference to the modified StreamingTexture, allowing method chaining.
         *
         * @throws core::NexusException if unable to draw the surface onto this StreamingTexture.
         */
        StreamingTexture& DrawImage(const Surface& other, int x, int y, int ox, int oy) override;

        /**
         * @brief Draws the specified Surface onto this StreamingTexture at the specified position.
         * @param other The Surface to draw onto this StreamingTexture.
         * @param position The position of the top-left corner of the destination rectangle.
         * @param origin The origin of the source rectangle.
         * @return A reference to the modified StreamingTexture, allowing method chaining.
         *
         * @throws core::NexusException if unable to draw the surface onto this StreamingTexture.
         */
        StreamingTexture& DrawImage(const Surface& other, const nexus::math::IVec2& position, const nexus::math::IVec2& origin) override;

        /**
         * @brief Draws the specified Surface onto this StreamingTexture with scaling and offset.
         * @param other The Surface to draw onto this StreamingTexture.
         * @param x The x-coordinate of the top-left corner of the destination rectangle.
         * @param y The y-coordinate of the top-left corner of the destination rectangle.
         * @param sx The scaling factor along the x-axis.
         * @param sy The scaling factor along the y-axis.
         * @param ox The x-coordinate of the origin of the source rectangle.
         * @param oy The y-coordinate of the origin of the source rectangle.
         * @return A reference to the modified StreamingTexture, allowing method chaining.
         *
         * @throws core::NexusException if unable to draw the surface onto this StreamingTexture.
         */
        StreamingTexture& DrawImageScaled(const Surface& other, float x, float y, float sx, float sy, float ox, float oy) override;

        /**
         * @brief Draws the specified Surface onto this StreamingTexture with scaling and offset.
         * @param other The Surface to draw onto this StreamingTexture.
         * @param position The position of the top-left corner of the destination rectangle.
         * @param scale The scaling factors along the x and y axes.
         * @param origin The origin of the source rectangle.
         * @return A reference to the modified StreamingTexture, allowing method chaining.
         *
         * @throws core::NexusException if unable to draw the surface onto this StreamingTexture.
         */
        StreamingTexture& DrawImageScaled(const Surface& other, const nexus::math::Vec2& position, const nexus::math::Vec2& scale, const nexus::math::Vec2& origin) override;

        /**
         * @brief Draws a portion of the specified Surface onto this StreamingTexture
         *        at the specified destination rectangle.
         * @param other The Surface to draw onto this StreamingTexture.
         * @param rectSrc The source rectangle defining the portion of the other Surface to draw.
         * @param rectDst The destination rectangle defining where to draw on this StreamingTexture.
         * @return A reference to the modified StreamingTexture, allowing method chaining.
         *
         * @throws core::NexusException if unable to draw the surface onto this StreamingTexture.
         */
        StreamingTexture& DrawImage(const Surface& other, const nexus::shape2D::Rectangle& rectSrc, nexus::shape2D::Rectangle rectDst) override;
    };

}

namespace nexus { namespace gfx {

    /**
     * @brief gfx::StreamingTexture class represents a streaming texture derived from the gfx::Texture class.
     *        It allows efficient streaming updates to the texture.
     */
    class NEXUS_API StreamingTexture : public utils::Container<_gfx_impl::StreamingTexture>
    {
      public:
        StreamingTexture(gfx::Renderer& ctx, int w, int h, nexus::gfx::PixelFormat format = gfx::PixelFormat::RGBA32)
        : Container(ctx, w, h, format)
        { }
    };

}}

#endif //NEXUS_EXT_GFX_STREAMING_TEXTURE_HPP