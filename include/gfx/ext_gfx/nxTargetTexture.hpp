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
