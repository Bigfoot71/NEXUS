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

#ifndef NEXUS_GAPI_IMPL_FONT_HPP
#define NEXUS_GAPI_IMPL_FONT_HPP

#include "../../gfx/cmn_ext_gfx_gapi_impl/nxFont.hpp"
#include "../../utils/nxContextual.hpp"
#include "../../gfx/nxBasicFont.hpp"
#include "../../gfx/nxSurface.hpp"
#include "../../gfx/nxPixel.hpp"
#include "../../gfx/nxColor.hpp"
#include "../../math/nxVec2.hpp"
#include "./nxCamera3D.hpp"

// TODO: Create a specific shader for rendering fonts, in particular to manage outline colors

namespace _gapi_impl {

    using KeyGlyph = _ext_gfx_gapi_impl::KeyGlyph;
    using KeyGlyphHash = _ext_gfx_gapi_impl::KeyGlyphHash;

    template <typename T_Context>
    struct TextureGlyph : _ext_gfx_gapi_impl::TextureGlyph<T_Context>
    {
        virtual ~TextureGlyph() = default;
        virtual float Draw(T_Context& ctx, float x, float y, float scale, const nexus::gfx::Color& tint) const = 0;
        virtual float Draw3D(T_Context& ctx, const nexus::math::Vec3& position, float scale, const nexus::gfx::Color& tint) const = 0;
    };

    template <typename T_Context, typename T_TextureGlyph>
    class Font : public _ext_gfx_gapi_impl::Font<T_Context, T_TextureGlyph>
    {
      public:
        using _ext_gfx_gapi_impl::Font<T_Context, T_TextureGlyph>::Font;

      protected:
        /**
         * @brief Caches a glyph for later use.
         *
         * @param keyGlyph The key of the glyph to cache.
         * @return An iterator pointing to the cached glyph.
         */
        T_TextureGlyph& CacheGlyph(KeyGlyph keyGlyph) override;

      public:
        /**
         * @brief Draws the text at the specified position with optional scaling and tint.
         *
         * @param text The text to be drawn.
         * @param x The x-coordinate of the starting position.
         * @param y The y-coordinate of the starting position.
         * @param scale The scale factor for the text.
         * @param tint The color tint for the text.
         */
        void Draw(const std::string& text, float x, float y, float scale = 1.0f, const nexus::gfx::Color& tint = nexus::gfx::White);

        /**
         * @brief Draws the text at the specified position with optional scaling and tint.
         *
         * @param text The text to be drawn.
         * @param position The position of the starting point.
         * @param scale The scale factor for the text.
         * @param tint The color tint for the text.
         */
        void Draw(const std::string& text, const nexus::math::Vec2 position, float scale = 1.0f, const nexus::gfx::Color& tint = nexus::gfx::White);

        /**
         * @brief Draws the text at the specified position with optional rotation, scaling, origin, and tint.
         *
         * @param text The text to be drawn.
         * @param position The position of the starting point.
         * @param origin The origin point for rotation.
         * @param rotation The rotation angle.
         * @param scale The scale factor for the text.
         * @param tint The color tint for the text.
         */
        void Draw(const std::string& text, const nexus::math::Vec2& position, const nexus::math::Vec2& origin, float rotation = 0, float scale = 1.0f, nexus::gfx::Color tint = nexus::gfx::White);

        /**
         * @brief Draws the text centered at the specified position with optional scaling and tint.
         *
         * @param text The text to be drawn.
         * @param x The x-coordinate of the center position.
         * @param y The y-coordinate of the center position.
         * @param scale The scale factor for the text.
         * @param tint The color tint for the text.
         */
        void DrawCentered(const std::string& text, float x, float y, float scale = 1.0f, const nexus::gfx::Color& tint = nexus::gfx::White);

        /**
         * @brief Draws the text centered at the specified position with optional scaling and tint.
         *
         * @param text The text to be drawn.
         * @param position The position of the center.
         * @param scale The scale factor for the text.
         * @param tint The color tint for the text.
         */
        void DrawCentered(const std::string& text, const nexus::math::Vec2& position, float scale = 1.0f, const nexus::gfx::Color& tint = nexus::gfx::White);

        /**
         * @brief Draws the text at the specified position with optional rotation, scaling, and tint.
         *
         * @param text The text to be drawn.
         * @param position The position of the starting point.
         * @param rotation The rotation angle.
         * @param scale The scale factor for the text.
         * @param tint The color tint for the text.
         */
        void Draw(const std::string& text, const nexus::math::Vec2& position, float rotation, float scale = 1.0f, const nexus::gfx::Color& tint = nexus::gfx::White);

        /**
         * @brief Renders 3D text at the specified position with optional rotation, scaling, and tint.
         *
         * @param text The text to be drawn.
         * @param position The 3D position of the starting point.
         * @param rotationAxis The axis of rotation for the text.
         * @param rotationAngle The rotation angle in radians.
         * @param size The desired font size in 3D space.
         * @param color The color tint for the text (default is white).
         */
        void Draw3D(const std::string& text, const nexus::math::Vec3& position, const nexus::math::Vec3& rotationAxis, float rotationAngle, float size, const nexus::gfx::Color& color = nexus::gfx::White);

        /**
         * @brief Renders 3D text facing the camera, at the specified position with optional scaling and tint.
         *
         * @param text The text to be drawn.
         * @param camera The 3D camera to face.
         * @param position The 3D position of the text.
         * @param size The desired font size in 3D space.
         * @param color The color tint for the text (default is white).
         */
        void Draw3D(const std::string& text, const Camera3D& camera, const nexus::math::Vec3& position, float size, const nexus::gfx::Color& color = nexus::gfx::White);
    };


    /* Private Implementation Font */

    template <typename T_Context, typename T_TextureGlyph>
    T_TextureGlyph& Font<T_Context, T_TextureGlyph>::CacheGlyph(KeyGlyph keyGlyph)
    {
        return this->glyphCache.emplace(std::piecewise_construct, std::forward_as_tuple(keyGlyph),
            std::forward_as_tuple(this->RenderGlyph(keyGlyph.GetChar(), keyGlyph.GetRenderMode(), nexus::gfx::White, nexus::gfx::Black))).first->second;
    }


    /* Public Implementation Font */

    template <typename T_Context, typename T_TextureGlyph>
    void Font<T_Context, T_TextureGlyph>::Draw(const std::string& text, float x, float y, float scale, const nexus::gfx::Color& tint)
    {
        for (Uint16 c : text)
        {
            // Render the glyph and increment x for the next glyph
            x += this->GetGlyph(this->GenKeyGlyphFromCurrentState(c)).Draw(this->ctx, x, y, scale, tint);
        }
    }

    template <typename T_Context, typename T_TextureGlyph>
    void Font<T_Context, T_TextureGlyph>::Draw(const std::string& text, const nexus::math::Vec2 position, float scale, const nexus::gfx::Color& tint)
    {
        Draw(text, position.x, position.y, scale, tint);
    }

    template <typename T_Context, typename T_TextureGlyph>
    void Font<T_Context, T_TextureGlyph>::Draw(const std::string& text, const nexus::math::Vec2& position, const nexus::math::Vec2& origin, float rotation, float scale, nexus::gfx::Color tint)
    {
        this->ctx.PushMatrix();
            this->ctx.Translate(position.x, position.y, 0.0f);
            this->ctx.Rotate(rotation, 0.0f, 0.0f, 1.0f);
            this->ctx.Translate(-origin.x, -origin.y, 0.0f);
            Draw(text, { 0, 0 }, scale, tint);
        this->ctx.PopMatrix();
    }

    template <typename T_Context, typename T_TextureGlyph>
    void Font<T_Context, T_TextureGlyph>::DrawCentered(const std::string& text, float x, float y, float scale, const nexus::gfx::Color& tint)
    {
        const nexus::math::Vec2 size = nexus::math::Vec2(this->GetSizeText(text)) * scale * 0.5f;
        Draw(text, x - size.x, y - size.y, scale, tint);
    }

    template <typename T_Context, typename T_TextureGlyph>
    void Font<T_Context, T_TextureGlyph>::DrawCentered(const std::string& text, const nexus::math::Vec2& position, float scale, const nexus::gfx::Color& tint)
    {
        DrawCentered(text, position.x, position.y, scale, tint);
    }

    template <typename T_Context, typename T_TextureGlyph>
    void Font<T_Context, T_TextureGlyph>::Draw(const std::string& text, const nexus::math::Vec2& position, float rotation, float scale, const nexus::gfx::Color& tint)
    {
        const nexus::math::Vec2 origin = this->GetSizeText(text) * (scale * 0.5f);

        this->ctx.PushMatrix();
            this->ctx.Translate(position.x, position.y, 0.0f);
            if (rotation) this->ctx.Rotate(rotation, 0.0f, 0.0f, 1.0f);
            this->ctx.Translate(-origin.x, -origin.y, 0.0f);
            Draw(text, { 0, 0 }, scale, tint);
        this->ctx.PopMatrix();
    }

    template <typename T_Context, typename T_TextureGlyph>
    void Font<T_Context, T_TextureGlyph>::Draw3D(const std::string& text, const nexus::math::Vec3& position, const nexus::math::Vec3& rotationAxis, float rotationAngle, float size, const nexus::gfx::Color& color)
    {
        const float scale = size / this->size;
        const nexus::math::Vec2 textSize = this->GetSizeText(text);
        const nexus::math::Vec2 origin = textSize * (scale * 0.5f);

        float x = 0.0f;

        this->ctx.PushMatrix();

            this->ctx.Translate(position);
            if (rotationAngle) this->ctx.Rotate(rotationAngle, rotationAxis);
            this->ctx.Translate(-origin.x, -origin.y, 0);

            for (Uint16 c : text)
            {
                // Render the glyph and increment x for the next glyph
                x += this->GetGlyph(this->GenKeyGlyphFromCurrentState(c)).Draw3D(this->ctx, { x, 0, 0 }, scale, color);
            }

        this->ctx.PopMatrix();
    }

    template <typename T_Context, typename T_TextureGlyph>
    void Font<T_Context, T_TextureGlyph>::Draw3D(const std::string& text, const Camera3D& camera, const nexus::math::Vec3& position, float size, const nexus::gfx::Color& color)
    {
        const nexus::math::Vec3 toCam = camera.position - position;
        const float rotationAngle = std::atan2(-toCam.z, toCam.x) * nexus::math::Rad2Deg + 90.0f;
        Draw3D(text, position, camera.up, rotationAngle, size, color);
    }

}

#endif //NEXUS_GAPI_IMPL_FONT_HPP
