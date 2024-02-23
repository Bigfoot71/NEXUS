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

#ifndef NEXUS_GL_TARGET_TEXTURE
#define NEXUS_GL_TARGET_TEXTURE

#include "../../core/nxException.hpp"
#include "../../gfx/nxColor.hpp"
#include "../../core/nxLog.hpp"
#include "./nxContextual.hpp"
#include "./nxContext.hpp"
#include "./nxTexture.hpp"
#include "./nxEnums.hpp"

namespace _gl_impl
{
    /**
     * @brief Class representing a target texture for OpenGL rendering.
     */
    class TargetTexture : public nexus::gl::Contextual
    {
      private:
        Texture                 texture;            ///< Color buffer attachment texture
        Texture                 depth;              ///< Depth buffer attachment texture
        nexus::shape2D::Rectangle  prevViewport;       ///< Previous viewport to restore
        Uint32                  id;                 ///< OpenGL framebuffer object id
        bool                    active;             ///< Indicates whether the target texture is active.

      public:
        /**
         * @brief Constructs a TargetTexture object.
         * @param ctx The OpenGL context.
         * @param w The width of the texture.
         * @param h The height of the texture.
         * @param format The format of the texture.
         */
        TargetTexture(nexus::gl::Context& ctx, int w, int h, nexus::gl::TextureFormat format);

        /**
         * @brief Destructor for TargetTexture.
         */
        ~TargetTexture();

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
        void Clear(const nexus::gfx::Color& color);

        /**
         * @brief Draws a portion of the texture to another rectangle on the screen.
         * @param src The source rectangle defining the portion of the texture to draw.
         * @param dst The destination rectangle where the texture portion will be drawn.
         * @param origin The origin point for rotation and scaling.
         * @param rotation The rotation angle in radians.
         * @param tint The tint color for the texture.
         */
        void Draw(nexus::shape2D::RectangleF src, const nexus::shape2D::RectangleF& dst, const nexus::math::Vec2& origin = {}, float rotation = 0.0f, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draws the texture at the specified coordinates on the screen.
         * @param x The x-coordinate of the position.
         * @param y The y-coordinate of the position.
         * @param tint The tint color for the texture.
         */
        void Draw(float x, float y, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draws the texture at the specified position on the screen.
         * @param position The position where the texture will be drawn.
         * @param tint The tint color for the texture.
         */
        void Draw(const nexus::math::Vec2& position, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draws the texture at the specified position with rotation and scale.
         * @param position The position where the texture will be drawn.
         * @param rotation The rotation angle in radians.
         * @param scale The scale factor.
         * @param tint The tint color for the texture.
         */
        void Draw(const nexus::math::Vec2& position, float rotation, float scale, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draws the texture at the specified position with rotation, origin, scale, and tint.
         * @param position The position where the texture will be drawn.
         * @param rotation The rotation angle in radians.
         * @param origin The origin point for rotation and scaling.
         * @param scale The scale factor.
         * @param tint The tint color for the texture.
         */
        void Draw(const nexus::math::Vec2& position, float rotation, const nexus::math::Vec2& origin = { 0, 0 }, const nexus::math::Vec2& scale = { 1, 1 }, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draws a portion of the texture at the specified position on the screen.
         * @param src The source rectangle defining the portion of the texture to draw.
         * @param position The position where the texture will be drawn.
         * @param tint The tint color for the texture.
         */
        void Draw(const nexus::shape2D::RectangleF& src, const nexus::math::Vec2& position, const nexus::gfx::Color& tint = nexus::gfx::White) const;

        /**
         * @brief Draws a billboard textured with the target texture using the given camera parameters.
         * @param camera The camera to use for rendering the billboard.
         * @param source The source rectangle defining the portion of the texture to use for the billboard.
         * @param position The position of the billboard in 3D space.
         * @param up The up vector of the billboard.
         * @param size The size of the billboard.
         * @param origin The origin point for rotation and scaling.
         * @param rotation The rotation angle in radians.
         * @param tint The tint color for the billboard.
         */
        void DrawBillboard(const nexus::gl::Camera3D& camera, nexus::shape2D::Rectangle source, const nexus::math::Vec3& position, const nexus::math::Vec3& up, const nexus::math::Vec2& size, const nexus::math::Vec2& origin, float rotation, const nexus::gfx::Color& tint) const;

        /**
         * @brief Draws a billboard textured with the target texture using the given camera parameters.
         * @param camera The camera to use for rendering the billboard.
         * @param position The position of the billboard in 3D space.
         * @param size The size of the billboard.
         * @param tint The tint color for the billboard.
         */
        void DrawBillboard(const nexus::gl::Camera3D& camera, const nexus::math::Vec3& position, float size, const nexus::gfx::Color& tint) const;

        /**
         * @brief Draws a billboard textured with the target texture using the given camera parameters.
         * @param camera The camera to use for rendering the billboard.
         * @param source The source rectangle defining the portion of the texture to use for the billboard.
         * @param position The position of the billboard in 3D space.
         * @param size The size of the billboard.
         * @param tint The tint color for the billboard.
         */
        void DrawBillboard(const nexus::gl::Camera3D& camera, const nexus::shape2D::Rectangle& source, const nexus::math::Vec3& position, const nexus::math::Vec2& size, const nexus::gfx::Color& tint) const;

        /**
         * @brief Checks if the target texture is active.
         * @return True if the target texture is active, false otherwise.
         */
        bool IsActive() const { return active; }

        /**
         * @brief Returns the width of the target texture.
         * @return The width of the target texture.
         */
        int GetWidth() const { return texture.GetWidth(); }

        /**
         * @brief Returns the height of the target texture.
         * @return The height of the target texture.
         */
        int GetHeight() const { return texture.GetHeight(); }

        /**
         * @brief Returns the width of the target texture as a floating-point value.
         * @return The width of the target texture.
         */
        float GetWidthF() const { return texture.GetWidthF(); }

        /**
         * @brief Returns the height of the target texture as a floating-point value.
         * @return The height of the target texture.
         */
        float GetHeightF() const { return texture.GetHeightF(); }

        /**
         * @brief Returns the size of the target texture.
         * @return The size of the target texture as an integer vector.
         */
        nexus::math::IVec2 GetSize() const { return texture.GetSize(); }

        /**
         * @brief Returns the rectangle size of the target texture.
         * @return The rectangle size of the target texture.
         */
        nexus::shape2D::Rectangle GetRectSize() const { return texture.GetRectSize(); }

        /**
         * @brief Returns the OpenGL framebuffer object ID.
         * @return The OpenGL framebuffer object ID.
         */
        Uint32 GetID() const { return id; }
    };
}

namespace nexus { namespace gl {

    /**
     * @brief Class representing a container for TargetTexture objects.
     */
    class NEXUS_API TargetTexture : public Container<_gl_impl::TargetTexture>
    {
      public:
        /**
         * @brief Constructs a TargetTexture object.
         * @param ctx The OpenGL context.
         * @param width The width of the texture.
         * @param height The height of the texture.
         * @param format The format of the texture.
         */
        TargetTexture(gl::Context& ctx, int width, int height, TextureFormat format = TextureFormat::RGBA8888)
        : Container(ctx, width, height, format)
        { }
    };

}}

#endif //NEXUS_GL_TARGET_TEXTURE
