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

#ifndef NEXUS_EXT_3D_GL_SPRITE_HPP
#define NEXUS_EXT_3D_GL_SPRITE_HPP

#include "../../../gfx/cmn_ext_2D_ext_3D_impl/nxSprite.hpp"
#include "../../../shape/2D/nxRectangle.hpp"
#include "../ext_2D/nxSprite2D.hpp"
#include "../nxTexture.hpp"

namespace nexus { namespace gl {

    /**
     * @brief Extended Sprite class for rendering animated sprites using gfx::Texture.
     */
    class NEXUS_API Sprite3D : public Sprite2D
    {
      public:
        using Sprite2D::Sprite2D;

        /**
         * @brief Draw a billboard in 3D space.
         *
         * @param camera Camera3D object.
         * @param pos Position of the billboard.
         * @param tint Tint color of the billboard.
         * @param instance Sprite instance for animation.
         */
        void DrawBillboard(Camera3D& camera, const math::Vec3& pos, const gfx::Color& tint, const Instance * const instance) const;

        /**
         * @brief Draw a billboard in 3D space with specified size.
         *
         * @param camera Camera3D object.
         * @param pos Position of the billboard.
         * @param size Size of the billboard.
         * @param tint Tint color of the billboard.
         * @param instance Sprite instance for animation.
         */
        void DrawBillboard(Camera3D& camera, const math::Vec3& pos, math::Vec2 size, const gfx::Color& tint, const Instance * const instance) const;

        /**
         * @brief Draw a billboard in 3D space with default color.
         *
         * @param camera Camera3D object.
         * @param pos Position of the billboard.
         * @param tint Tint color of the billboard.
         * @param keyInstance Key of the sprite instance for animation.
         */
        void DrawBillboard(Camera3D& camera, const math::Vec3& pos, const gfx::Color& tint = gfx::White, const std::string& keyInstance = "main") const;

        /**
         * @brief Draw a billboard in 3D space with specified size and default color.
         *
         * @param camera Camera3D object.
         * @param pos Position of the billboard.
         * @param size Size of the billboard.
         * @param tint Tint color of the billboard.
         * @param keyInstance Key of the sprite instance for animation.
         */
        void DrawBillboard(Camera3D& camera, const math::Vec3& pos, math::Vec2 size, const gfx::Color& tint = gfx::White, const std::string& keyInstance = "main") const;
    };

}}

#endif //NEXUS_EXT_3D_GL_SPRITE_HPP
