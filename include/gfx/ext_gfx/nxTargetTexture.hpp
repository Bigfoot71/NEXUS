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

#ifndef NEXUS_EXT_GFX_TARGET_TEXTURE_HPP
#define NEXUS_EXT_GFX_TARGET_TEXTURE_HPP

#include "./nxTexture.hpp"

namespace _gfx_impl {

    /**
     * @brief Class representing a target texture for easy management of TargetTextures.
     */
    class TargetTexture : public Texture
    {
      public:
        bool active; ///< Flag indicating if the target texture is active.

      public:
        /**
         * @brief Constructs a TargetTexture object.
         * @param ctx The renderer context.
         * @param w The width of the texture.
         * @param h The height of the texture.
         * @param format The pixel format of the texture.
         */
        TargetTexture(nexus::gfx::Renderer& ctx, int w, int h, nexus::gfx::PixelFormat format);

        /**
         * @brief Destructor for TargetTexture.
         */
        ~TargetTexture();

        /**
         * @brief Checks if the target texture is active.
         * @return True if the target texture is active, false otherwise.
         */
        bool IsActive() const;

        /**
         * @brief Begins rendering to the target texture.
         * @return Reference to this TargetTexture object.
         */
        TargetTexture& Begin();

        /**
         * @brief Ends rendering to the target texture.
         * @return Reference to this TargetTexture object.
         */
        TargetTexture& End();

        /**
         * @brief Clears the target texture with a specified color, even if it is not currently active.
         * 
         * This function clears the contents of the target texture with the specified color, regardless of whether the texture is currently active or not.
         * Note that while this function can be used to clear the texture when it is not active, it is typically more efficient to clear the texture when it is already active, especially when chaining multiple operations.
         * 
         * @param color The color used to clear the target texture.
         */
        void Clear();
    };

}

namespace nexus { namespace gfx {

    /**
     * @brief Class representing a container for TargetTexture objects.
     */
    class NEXUS_API TargetTexture : public utils::Container<_gfx_impl::TargetTexture>
    {
      public:
        /**
         * @brief Constructs a StreamingTexture object.
         * @param ctx The renderer context.
         * @param w The width of the texture.
         * @param h The height of the texture.
         * @param format The pixel format of the texture.
         */
        TargetTexture(gfx::Renderer& ctx, int w, int h, nexus::gfx::PixelFormat format = gfx::PixelFormat::RGBA32)
        : Container(ctx, w, h, format)
        { }
    };

}}

#endif //NEXUS_EXT_GFX_TARGET_TEXTURE_HPP
