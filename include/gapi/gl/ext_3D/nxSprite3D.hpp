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
