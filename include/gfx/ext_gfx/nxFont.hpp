#ifndef NEXUS_EXT_GFX_FONT_HPP
#define NEXUS_EXT_GFX_FONT_HPP

#include "../cmn_ext_gfx_gapi_impl/nxFont.hpp"
#include "../../math/nxVec2.hpp"
#include "../nxBasicFont.hpp"
#include "./nxRenderer.hpp"
#include "./nxTexture.hpp"

#include <unordered_map>
#include <SDL_render.h>

namespace _gfx_impl {

    using KeyGlyph = _ext_gfx_gapi_impl::KeyGlyph;
    using KeyGlyphHash = _ext_gfx_gapi_impl::KeyGlyphHash;

    struct TextureGlyph : _ext_gfx_gapi_impl::TextureGlyph<nexus::gfx::Renderer>
    {
        SDL_Texture *texture;
        Uint16 width, height;

        TextureGlyph(nexus::gfx::Renderer& ctx, const nexus::gfx::Surface& surface);
        ~TextureGlyph();

        float Draw(nexus::gfx::Renderer& ctx, float x, float y, float scale) const;
    };

    class Font : public _ext_gfx_gapi_impl::Font<nexus::gfx::Renderer, TextureGlyph>
    {
      public:
        using _ext_gfx_gapi_impl::Font<nexus::gfx::Renderer, TextureGlyph>::Font;

      public:
        /**
         * @brief Enumeration for font styles.
         */
        using Style = BasicFont::Style;

        /**
         * @brief Enumeration for font hinting styles.
         */
        using Hinting = BasicFont::Hinting;

        /**
         * @brief Enumeration for text alignment when wrapping text.
         */
        using WrappedAlign = BasicFont::WrappedAlign;

        /**
         * @brief Enumeration for text writing direction.
         */
        using Direction = BasicFont::Direction;

        /**
         * @brief Enumeration for text rendering methods.
         */
        using Render = BasicFont::Render;

      protected:
        /**
         * @brief Caches a glyph for later use.
         *
         * @param keyGlyph The key of the glyph to cache.
         * @return An iterator pointing to the cached glyph.
         */
        TextureGlyph& CacheGlyph(KeyGlyph keyGlyph) override;

      public:
        /**
         * @brief Draws the text at the specified position with optional scaling.
         *
         * @param text The text to be drawn.
         * @param x The x-coordinate of the starting position.
         * @param y The y-coordinate of the starting position.
         * @param scale The scale factor for the text.
         * @param tint The color tint applied to the text.
         */
        void Draw(const std::string& text, float x, float y, float scale = 1.0f, const nexus::gfx::Color& tint = nexus::gfx::White);

        /**
         * @brief Draws the text at the specified position with optional scaling.
         *
         * @param text The text to be drawn.
         * @param position The position of the starting point.
         * @param scale The scale factor for the text.
         * @param tint The color tint applied to the text.
         */
        void Draw(const std::string& text, const nexus::math::Vec2 position, float scale = 1.0f, const nexus::gfx::Color& tint = nexus::gfx::White);

        /**
         * @brief Draws the text centered at the specified position with optional scaling.
         *
         * @param text The text to be drawn.
         * @param x The x-coordinate of the center position.
         * @param y The y-coordinate of the center position.
         * @param scale The scale factor for the text.
         * @param tint The color tint applied to the text.
         */
        void DrawCentered(const std::string& text, float x, float y, float scale = 1.0f, const nexus::gfx::Color& tint = nexus::gfx::White);

        /**
         * @brief Draws the text centered at the specified position with optional scaling.
         *
         * @param text The text to be drawn.
         * @param position The position of the center.
         * @param scale The scale factor for the text.
         * @param tint The color tint applied to the text.
         */
        void DrawCentered(const std::string& text, const nexus::math::Vec2& position, float scale = 1.0f, const nexus::gfx::Color& tint = nexus::gfx::White);
    };

}

namespace nexus { namespace gfx {

    /**
     * @brief Basic derivation of the gfx::Font class enabling text rendering in a Renderer with glyph caching.
     */
    class NEXUS_API Font : public utils::Container<_gfx_impl::Font>
    {
      public:
        /**
         * @brief Constructor for the Font class.
         *
         * @param renderer Reference to the Renderer object.
         * @param fontPath Path to the font file.
         * @param fontSize Size of the font.
         * @param sdf If true, enables Signed Distance Field (SDF) rendering.
         */
        Font(Renderer& ctx, const std::string& fontPath, int fontSize, bool sdf = false)
        : Container(ctx, fontPath, fontSize, sdf)
        { }
    };

}}

#endif //NEXUS_EXT_GFX_FONT_HPP
