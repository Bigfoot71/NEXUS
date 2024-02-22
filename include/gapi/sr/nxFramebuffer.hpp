#ifndef NEXUS_SR_FRAMEBUFFER_HPP
#define NEXUS_SR_FRAMEBUFFER_HPP

#include "../../shape/2D/nxRectangle.hpp"
#include "../../gfx/nxSurface.hpp"
#include "../../gfx/nxPixel.hpp"
#include "../../math/nxVec2.hpp"
#include "./nxDepthBuffer.hpp"
#include "./nxContextual.hpp"
#include "./nxEnums.hpp"
#include <SDL_stdinc.h>
#include <algorithm>
#include <limits>

namespace nexus { namespace sr {

    class NEXUS_API Framebuffer : public gfx::Surface
    {
      private:
        sr::DepthBuffer depth;

      public:
        /**
         * @brief Constructor for Framebuffer class.
         *
         * Constructs a Framebuffer object with the specified width, height, and pixel format.
         *
         * @param w The width of the framebuffer.
         * @param h The height of the framebuffer.
         * @param format The pixel format of the framebuffer (default is RGBA32).
         */
        Framebuffer(int w, int h, gfx::PixelFormat format = gfx::PixelFormat::RGBA32)
        : gfx::Surface(w, h, gfx::Blank, format)
        , depth(w, h)
        { }

        /**
         * @brief Constructor for Framebuffer class taking an existing surface.
         *
         * Constructs a Framebuffer object using an existing surface, transferring ownership.
         * The depth buffer is resized to match the dimensions of the new surface.
         *
         * @param surface An rvalue reference to an existing surface.
         */
        Framebuffer(gfx::Surface&& surface)
        : gfx::Surface(std::move(surface))
        , depth(this->surface->w, this->surface->h)
        { }

        /**
         * @brief Sets a new surface for the framebuffer.
         *
         * Sets a new surface for the framebuffer, transferring ownership.
         * The depth buffer is resized to match the dimensions of the new surface.
         *
         * @param surface An rvalue reference to the new surface.
         */
        void SetSurface(gfx::Surface&& surface)
        {
            *static_cast<gfx::Surface*>(this) = std::move(surface);
            depth.Resize(this->surface->w, this->surface->h);
        }

        /**
         * @brief Locks the framebuffer for rendering.
         *
         * This function is used to lock the framebuffer for rendering.
         */
        void Begin()
        {
            if (this->MustLock()) this->Lock();
        }

        /**
         * @brief Unlocks the framebuffer after rendering.
         *
         * This function unlocks the framebuffer after rendering within a specific viewport.
         */
        void End()
        {
            if (this->MustLock()) this->Unlock();
        }

        /**
         * @brief Clears the framebuffer by setting the pixel at (0, 0) to a blank color and resetting the depth buffer.
         *
         * This function clears the framebuffer by setting the pixel at (0, 0) to a blank color and resetting the depth buffer.
         */
        void Clear(const gfx::Color& color)
        {
            static_cast<gfx::Surface*>(this)->SetPixelUnsafe(0, 0, color);
            depth.ForceDepth(0, 0, depth.MaxDepth);

            Uint8 *pTex = static_cast<Uint8*>(surface->pixels) + surface->format->BytesPerPixel;
            float *pDepth = depth.buffer.data() + 1;

            const Uint8 *pTexEnd = pTex + surface->w * surface->h * surface->format->BytesPerPixel;

            for (; pTex < pTexEnd; pTex += surface->format->BytesPerPixel, pDepth++)
            {
                std::memcpy(pTex, surface->pixels, surface->format->BytesPerPixel);
                std::memcpy(pDepth, depth.buffer.data(), sizeof(float));
            }
        }

        /**
         * @brief Gets the depth value at the specified coordinates.
         *
         * This function retrieves the depth value at the specified coordinates.
         * @warning: This function is not safe and does not check if the given coordinates are out of bounds.
         * @note: Begin does not need to have been called for this to work.
         *
         * @param x The x-coordinate of the depth value.
         * @param y The y-coordinate of the depth value.
         * @return The depth value at the specified coordinates.
         */
        float GetDepthUnsafe(int x, int y) const
        {
            return depth.Get(x, y);
        }

        /**
         * @brief Retrieves the depth value at the specified index.
         *
         * Retrieves the depth value at the specified index in the depth buffer.
         * @warning: This function is not safe and does not check if the given coordinates are out of bounds.
         * @note: Begin does not need to have been called for this to work.
         *
         * @param i The index of the depth value.
         * @return The depth value at the specified index.
         */
        float GetDepthUnsafe(int i) const
        {
            return depth.Get(i);
        }

        /**
         * @brief Sets the color of the pixel at the specified coordinates, considering the provided depth.
         *
         * This function sets the color of the pixel at the specified coordinates, considering the provided depth.
         * @warning: This function is not safe and does not check if the given coordinates are out of bounds.
         *
         * @param x The x-coordinate of the pixel.
         * @param y The y-coordinate of the pixel.
         * @param z The depth value for the pixel.
         * @param color The color to set for the pixel.
         */
        void SetPixelDepthUnsafe(int x, int y, float z, const gfx::Color& color)
        {
            if (!depth.SetDepth(x, y, z)) return;
            static_cast<gfx::Surface*>(this)->SetPixelUnsafe(x, y, color);
        }

        /**
         * @brief Sets the color of the pixel at the specified index, considering the provided depth.
         *
         * Sets the color of the pixel at the specified index, considering the provided depth.
         * @warning: This function is not safe and does not check if the given coordinates are out of bounds.
         *
         * @param i The index of the pixel.
         * @param z The depth value for the pixel.
         * @param color The color to set for the pixel.
         */
        void SetPixelDepthUnsafe(int i, float z, const gfx::Color& color)
        {
            if (!depth.SetDepth(i, z)) return;
            static_cast<gfx::Surface*>(this)->SetPixelUnsafe(i, color);
        }

        /**
         * @brief Sets the color of the pixel at the specified coordinates, considering the provided depth if depth testing is enabled.
         *
         * This function sets the color of the pixel at the specified coordinates, considering the provided depth if depth testing is enabled.
         * @warning: This function is not safe and does not check if the given coordinates are out of bounds.
         *
         * @param x The x-coordinate of the pixel.
         * @param y The y-coordinate of the pixel.
         * @param z The depth value for the pixel.
         * @param color The color to set for the pixel.
         * @param depthTest If true, depth testing is performed before setting the pixel.
         */
        void SetPixelDepthUnsafe(int x, int y, float z, const gfx::Color& color, bool depthTest)
        {
            if (depthTest && !depth.SetDepth(x, y, z)) return;
            static_cast<gfx::Surface*>(this)->SetPixelUnsafe(x, y, color);
        }

        /**
         * @brief Sets the color of the pixel at the specified index, considering the provided depth if depth testing is enabled.
         *
         * Sets the color of the pixel at the specified index, considering the provided depth if depth testing is enabled.
         * @warning: This function is not safe and does not check if the given coordinates are out of bounds.
         *
         * @param i The index of the pixel.
         * @param z The depth value for the pixel.
         * @param color The color to set for the pixel.
         * @param depthTest If true, depth testing is performed before setting the pixel.
         */
        void SetPixelDepthUnsafe(int i, float z, const gfx::Color& color, bool depthTest)
        {
            if (depthTest && !depth.SetDepth(i, z)) return;
            static_cast<gfx::Surface*>(this)->SetPixelUnsafe(i, color);
        }

        /**
         * @brief Sets the depth value at the specified coordinates if the provided value is less than the existing depth.
         *
         * This function sets the depth value at the specified coordinates if the provided value is less than the existing depth.
         * @warning: This function is not safe and does not check if the given coordinates are out of bounds.
         * @note: Begin does not need to have been called for this to work.
         *
         * @param x The x-coordinate of the depth value.
         * @param y The y-coordinate of the depth value.
         * @param z The depth value to set.
         * @return True if the depth was set, false otherwise.
         */
        bool SetDepthUnsafe(int x, int y, float z)
        {
            return depth.SetDepth(x, y, z);
        }

        /**
         * @brief Sets the depth value at the specified index if the provided value is less than the existing depth.
         *
         * Sets the depth value at the specified index if the provided value is less than the existing depth.
         * @warning: This function is not safe and does not check if the given coordinates are out of bounds.
         * @note: Begin does not need to have been called for this to work.
         *
         * @param i The index of the depth value.
         * @param z The depth value to set.
         * @return True if the depth was set, false otherwise.
         */
        bool SetDepthUnsafe(int i, float z)
        {
            return depth.SetDepth(i, z);
        }
    };

}}

#endif //NEXUS_SR_FRAMEBUFFER_HPP