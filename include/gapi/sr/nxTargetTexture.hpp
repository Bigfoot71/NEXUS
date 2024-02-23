#ifndef NEXUS_SR_TARGET_TEXTURE_HPP
#define NEXUS_SR_TARGET_TEXTURE_HPP

#include "../../gfx/nxPixel.hpp"
#include "./nxFramebuffer.hpp"
#include "./nxContextual.hpp"
#include "./nxCamera3D.hpp"

namespace _sr_impl {

    class NEXUS_API TargetTexture : public nexus::sr::Contextual
    {
      private:
        nexus::sr::Framebuffer     framebuffer;
        nexus::shape2D::Rectangle  prevViewport;
        bool active = false;

      public:
        TargetTexture(nexus::sr::Context& ctx, int w, int h, nexus::gfx::PixelFormat format)
        : nexus::sr::Contextual(ctx), framebuffer(w, h, format)
        { }

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
        void DrawBillboard(const nexus::sr::Camera3D& camera, nexus::shape2D::Rectangle source, const nexus::math::Vec3& position, const nexus::math::Vec3& up, const nexus::math::Vec2& size, const nexus::math::Vec2& origin, float rotation, const nexus::gfx::Color& tint) const;

        /**
         * @brief Draws a billboard textured with the target texture using the given camera parameters.
         * @param camera The camera to use for rendering the billboard.
         * @param position The position of the billboard in 3D space.
         * @param size The size of the billboard.
         * @param tint The tint color for the billboard.
         */
        void DrawBillboard(const nexus::sr::Camera3D& camera, const nexus::math::Vec3& position, float size, const nexus::gfx::Color& tint) const;

        /**
         * @brief Draws a billboard textured with the target texture using the given camera parameters.
         * @param camera The camera to use for rendering the billboard.
         * @param source The source rectangle defining the portion of the texture to use for the billboard.
         * @param position The position of the billboard in 3D space.
         * @param size The size of the billboard.
         * @param tint The tint color for the billboard.
         */
        void DrawBillboard(const nexus::sr::Camera3D& camera, const nexus::shape2D::Rectangle& source, const nexus::math::Vec3& position, const nexus::math::Vec2& size, const nexus::gfx::Color& tint) const;

        /**
         * @brief Checks if the target texture is active.
         * @return True if the target texture is active, false otherwise.
         */
        bool IsActive() const { return active; }

        /**
         * @brief Returns the width of the target texture.
         * @return The width of the target texture.
         */
        int GetWidth() const { return framebuffer.GetWidth(); }

        /**
         * @brief Returns the height of the target texture.
         * @return The height of the target texture.
         */
        int GetHeight() const { return framebuffer.GetHeight(); }

        /**
         * @brief Returns the width of the target texture as a floating-point value.
         * @return The width of the target texture.
         */
        float GetWidthF() const { return framebuffer.GetWidthF(); }

        /**
         * @brief Returns the height of the target texture as a floating-point value.
         * @return The height of the target texture.
         */
        float GetHeightF() const { return framebuffer.GetHeightF(); }

        /**
         * @brief Returns the size of the target texture.
         * @return The size of the target texture as an integer vector.
         */
        nexus::math::IVec2 GetSize() const { return framebuffer.GetSize(); }

        /**
         * @brief Returns the rectangle size of the target texture.
         * @return The rectangle size of the target texture.
         */
        nexus::shape2D::Rectangle GetRectSize() const { return framebuffer.GetRectSize(); }
    };

}

namespace nexus { namespace sr {

    class TargetTexture : public Container<_sr_impl::TargetTexture>
    {
      public:
        TargetTexture(nexus::sr::Context& ctx, int w, int h, nexus::gfx::PixelFormat format = nexus::gfx::PixelFormat::ARGB32)
        : Container(ctx, w, h, format)
        { }
    };

}}

#endif //NEXUS_SR_TARGET_TEXTURE_HPP
