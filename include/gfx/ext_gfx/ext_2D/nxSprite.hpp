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

#ifndef NEXUS_EXT_GFX_SPRITE_HPP
#define NEXUS_EXT_GFX_SPRITE_HPP

#include "../../../shape/2D/nxRectangle.hpp"
#include "../../cmn_ext_2D_ext_3D_impl/nxSprite.hpp"
#include "../nxTexture.hpp"

namespace nexus { namespace gfx {

    /**
     * @brief Extended Sprite class for rendering animated sprites using gfx::Texture.
     */
    class NEXUS_API Sprite : public _gfx_impl::Sprite<Texture>
    {
      public:
        using _gfx_impl::Sprite<Texture>::Sprite;

        /**
         * @brief Draw the sprite at the specified position using the given instance.
         * 
         * @param pos The position to draw the sprite.
         * @param instance A pointer to the instance providing animation information.
         */
        void Draw(const math::Vec2& pos, const Instance * const instance);

        /**
         * @brief Draw the sprite at the specified position with scale, rotation, and UV origin using the given instance.
         * 
         * @param pos The position to draw the sprite.
         * @param scale The scale factor.
         * @param rotation The rotation angle.
         * @param uvOrigin The UV origin.
         * @param instance A pointer to the instance providing animation information.
         */
        void Draw(const math::Vec2& pos, float scale, float rotation, const math::Vec2& uvOrigin, const Instance * const instance);

        /**
         * @brief Draw the sprite at the specified position with scale, rotation, and UV origin using the given instance.
         * 
         * @param pos The position to draw the sprite.
         * @param sx The scale factor along the x-axis.
         * @param sy The scale factor along the y-axis.
         * @param rotation The rotation angle.
         * @param uvOrigin The UV origin.
         * @param instance A pointer to the instance providing animation information.
         */
        void Draw(const math::Vec2& pos, float sx, float sy, float rotation, const math::Vec2& uvOrigin, const Instance * const instance);

        /**
         * @brief Draw the sprite within the specified destination rectangle, origin, and rotation using the given instance.
         * 
         * @param dest The destination rectangle to draw the sprite.
         * @param origin The origin point.
         * @param rot The rotation angle.
         * @param instance A pointer to the instance providing animation information.
         */
        void Draw(shape2D::RectangleF dest, const math::Vec2& origin, float rot, const Instance * const instance);

        /**
         * @brief Draw the sprite at the specified position using the specified animation instance key.
         * 
         * @param pos The position to draw the sprite.
         * @param keyInstance The key identifying the animation instance (default is "main").
         */
        void Draw(const nexus::math::Vec2& pos, const std::string& keyInstance = "main");

        /**
         * @brief Draw the sprite at the specified position with scale, rotation, and UV origin using the specified animation instance key.
         * 
         * @param pos The position to draw the sprite.
         * @param scale The scale factor.
         * @param rotation The rotation angle.
         * @param uvOrigin The UV origin.
         * @param keyInstance The key identifying the animation instance (default is "main").
         */
        void Draw(const nexus::math::Vec2& pos, float scale, float rotation, const nexus::math::Vec2& uvOrigin = { 0.5f, 0.5f }, const std::string& keyInstance = "main");

        /**
         * @brief Draw the sprite at the specified position with scale, rotation, and UV origin using the specified animation instance key.
         * 
         * @param pos The position to draw the sprite.
         * @param sx The scale factor along the x-axis.
         * @param sy The scale factor along the y-axis.
         * @param rotation The rotation angle.
         * @param uvOrigin The UV origin.
         * @param keyInstance The key identifying the animation instance (default is "main").
         */
        void Draw(const nexus::math::Vec2& pos, float sx, float sy, float rotation, const nexus::math::Vec2& uvOrigin = { 0.5f, 0.5f }, const std::string& keyInstance = "main");

        /**
         * @brief Draw the sprite within the specified destination rectangle, origin, and rotation using the specified animation instance key.
         * 
         * @param dest The destination rectangle to draw the sprite.
         * @param origin The origin point.
         * @param rot The rotation angle.
         * @param keyInstance The key identifying the animation instance (default is "main").
         */
        void Draw(const nexus::shape2D::Rectangle& dest, const nexus::math::Vec2& origin, float rot, const std::string& keyInstance = "main");
    };

}}

#endif //NEXUS_EXT_GFX_SPRITE_HPP
