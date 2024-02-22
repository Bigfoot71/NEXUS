#ifndef NEXUS_GFX_BASIC_FONT_HPP
#define NEXUS_GFX_BASIC_FONT_HPP

#include "../shape/2D/nxRectangle.hpp"
#include "../core/nxException.hpp"
#include "../math/nxVec2.hpp"
#include "./nxSurface.hpp"
#include "./nxColor.hpp"

#include <unordered_map>
#include <SDL_stdinc.h>
#include <SDL_ttf.h>
#include <string>

namespace nexus { namespace gfx {

    /**
     * @brief A class for managing fonts and rendering text using SDL2_ttf.
     */
    class NEXUS_API BasicFont
    {
      protected:
        inline static Uint32 fontCount = 0;
        TTF_Font *font;
        int size;

      public:
        /**
         * @brief Get the total count of Font objects currently in use.
         *
         * @return The count of Font objects.
         */
        static Uint32 GetFontCount()
        {
            return fontCount;
        }

      public:
        /**
         * @brief Enumeration for font styles.
         */
        enum class Style : Uint8
        {
            Normal          = TTF_STYLE_NORMAL,         ///< Normal font style.
            Bold            = TTF_STYLE_BOLD,           ///< Bold font style.
            Italic          = TTF_STYLE_ITALIC,         ///< Italic font style.
            Underline       = TTF_STYLE_UNDERLINE,      ///< Underlined font style.
            Strikethrough   = TTF_STYLE_STRIKETHROUGH   ///< Strikethrough font style.
        };

        /**
         * @brief Enumeration for font hinting styles.
         */
        enum class Hinting : Uint8
        {
            Normal          = TTF_HINTING_NORMAL,           ///< Normal hinting.
            Light           = TTF_HINTING_LIGHT,            ///< Light hinting.
            Mono            = TTF_HINTING_MONO,             ///< Monochrome hinting.
            None            = TTF_HINTING_NONE,             ///< No hinting.
            LightSubPixel   = TTF_HINTING_LIGHT_SUBPIXEL    ///< Light subpixel hinting.
        };

        /**
         * @brief Enumeration for text alignment when wrapping text.
         */
        enum class WrappedAlign : Uint8
        {
            Left    = TTF_WRAPPED_ALIGN_LEFT,     ///< Left-aligned wrapped text.
            Center  = TTF_WRAPPED_ALIGN_CENTER,   ///< Center-aligned wrapped text.
            Right   = TTF_WRAPPED_ALIGN_RIGHT     ///< Right-aligned wrapped text.
        };

        /**
         * @brief Enumeration for text writing direction.
         */
        enum class Direction : Uint8
        {
            LTR = TTF_DIRECTION_LTR,        ///< Left to Right text writing direction.
            RTL = TTF_DIRECTION_RTL,        ///< Right to Left text writing direction.
            TTB = TTF_DIRECTION_TTB,        ///< Top to Bottom text writing direction.
            BTT = TTF_DIRECTION_BTT         ///< Bottom to Top text writing direction.
        };

        /**
         * @brief Enumeration for text rendering methods.
         */
        enum class Render : Uint8
        {
            Solid,    ///< Solid text rendering method.
            Shaded,   ///< Shaded text rendering method.
            Blended,  ///< Blended text rendering method.
            LCD       ///< LCD text rendering method.
        };

      public:
        /**
         * @brief Constructs a Font object and loads a font file with the specified size.
         *
         * @param fontPath The path to the font file.
         * @param fontSize The size of the font.
         * @param sdf Set to true for Signed Distance Field (SDF) font rendering.
         */
        BasicFont(const std::string& fontPath, int fontSize, bool sdf = false) : size(fontSize)
        {
            if (!TTF_WasInit()) TTF_Init();

            font = TTF_OpenFont(fontPath.c_str(), fontSize);

            if (font == nullptr)
            {
                throw core::NexusException("gfx::Font", "Loading TTF font failed. SDL_ttf: " + std::string(TTF_GetError()));
            }

            if (sdf) this->SetSDF(sdf);

            fontCount++;
        }

        /**
         * @brief Destroys the Font object and releases resources.
         */
        ~BasicFont()
        {
            if (font != nullptr) TTF_CloseFont(font);
            if ((--fontCount) == 0) TTF_Quit();
        }

        /**
         * @brief Implicit conversion to TTF_Font pointer.
         *
         * @return A pointer to the TTF_Font object.
         */
        operator TTF_Font*() { return font; }

        /**
         * @brief Implicit conversion to constant TTF_Font pointer.
         *
         * @return A constant pointer to the TTF_Font object.
         */
        operator const TTF_Font*() const { return font; }

        /**
         * @brief Enable or disable Signed Distance Field (SDF) rendering for the font.
         *
         * SDF rendering improves font appearance when scaling and rotating.
         */
        void SetSDF(bool value)
        {
            TTF_SetFontSDF(font, static_cast<SDL_bool>(value));
        }

        /**
         * @brief Query whether Signed Distance Field (SDF) rendering is enabled for the font.
         *
         * @return True if SDF rendering is enabled; false otherwise.
         */
        bool IsSDF() const
        {
            return TTF_GetFontSDF(font);
        }

        /**
         * @brief Get the height of the font.
         *
         * @return The height of the font in pixels.
         */
        int GetHeight() const
        {
            return TTF_FontHeight(font);
        }

        /**
         * @brief Get the ascent of the font.
         *
         * The ascent is the offset from the baseline to the top of the font.
         * This is a positive value, relative to the baseline.
         *
         * @return The ascent of the font in pixels.
         */
        int GetAscent() const
        {
            return TTF_FontAscent(font);
        }

        /**
         * @brief Get the descent of the font.
         *
         * The descent is the offset from the baseline to the bottom of the font.
         * This is a negative value, relative to the baseline.
         *
         * @return The descent of the font in pixels.
         */
        int GetDescent() const
        {
            return TTF_FontDescent(font);
        }

        /**
         * @brief Get the recommended spacing between lines of text for the font.
         *
         * @return The recommended line skip in pixels.
         */
        int GetLineSkip() const
        {
            return TTF_FontLineSkip(font);
        }

        /**
         * @brief Get the current font size.
         *
         * @return The size of the font in pixels.
         */
        int GetSize() const
        {
            return size;
        }

        /**
         * @brief Set the font size to the specified value.
         *
         * @param fontSize The new font size in pixels.
         */
        void SetSize(int fontSize)
        {
            TTF_SetFontSize(font, fontSize);
            size = fontSize;
        }

        /**
         * @brief Get the font style (normal, bold, italic, etc.).
         *
         * @return The current font style.
         */
        Style GetStyle() const
        {
            return static_cast<Style>(TTF_GetFontStyle(font));
        }

        /**
         * @brief Set the font style (normal, bold, italic, etc.).
         *
         * @param style The new font style to set.
         */
        void SetStyle(Style style)
        {
            TTF_SetFontStyle(font, static_cast<int>(style));
        }

        /**
         * @brief Get the font outline size.
         *
         * @return The current font outline size.
         */
        int GetOutline() const
        {
            return TTF_GetFontOutline(font);
        }

        /**
         * @brief Set the font outline size.
         *
         * @param outline The new font outline size to set.
         */
        void SetOutline(int outline)
        {
            TTF_SetFontOutline(font, outline);
        }

        /**
         * @brief Get the hinting mode of the font (normal, light, mono, etc.).
         *
         * @return The current hinting mode of the font.
         */
        Hinting GetHinting() const
        {
            return static_cast<Hinting>(TTF_GetFontHinting(font));
        }

        /**
         * @brief Set the hinting mode of the font (normal, light, mono, etc.).
         *
         * @param hinting The new hinting mode to set.
         */
        void SetHinting(Hinting hinting)
        {
            TTF_SetFontHinting(font, static_cast<int>(hinting));
        }

        /**
         * @brief Get the text wrapping alignment for a font.
         *
         * @return The current text wrapping alignment.
         */
        WrappedAlign GetWrappedAlign() const
        {
            return static_cast<WrappedAlign>(TTF_GetFontWrappedAlign(font));
        }

        /**
         * @brief Set the text wrapping alignment for a font.
         *
         * @param align The new text wrapping alignment to set.
         */
        void SetWrappedAlign(WrappedAlign align)
        {
            TTF_SetFontWrappedAlign(font, static_cast<int>(align));
        }

        /**
         * @brief Get the font kerning value.
         *
         * @return The current kerning value for the font.
         */
        int GetKerning() const
        {
            return TTF_GetFontKerning(font);
        }

        /**
         * @brief Set whether kerning is allowed for the font.
         *
         * Kerning is generally recommended for improved text rendering.
         *
         * @param allowed Set to 1 to allow kerning, or 0 to disable kerning.
         */
        void SetKerning(int allowed)
        {
            TTF_SetFontKerning(font, allowed);
        }

        /**
         * @brief Get the number of faces available in the font.
         *
         * @return The number of faces in the font.
         */
        int GetFacesNum() const
        {
            return TTF_FontFaces(font);
        }

        /**
         * @brief Check if the font is fixed-width.
         *
         * A fixed-width font has all glyphs with the same width.
         *
         * @return True if the font is fixed-width, false otherwise.
         */
        bool FaceIsFixedWidth() const
        {
            return TTF_FontFaceIsFixedWidth(font);
        }

        /**
         * @brief Get the family name of the font.
         *
         * The family name is determined by the contents of the font file.
         *
         * @return The family name of the font.
         */
        const char* GetFaceFamilyName() const
        {
            return TTF_FontFaceFamilyName(font);
        }

        /**
         * @brief Get the style name of the font.
         *
         * The style name is determined by the contents of the font file.
         *
         * @return The style name of the font.
         */
        const char* GetFaceStyleName() const
        {
            return TTF_FontFaceStyleName(font);
        }

        /**
         * @brief Check whether a glyph is provided by the font for a 16-bit codepoint.
         *
         * @param character The 16-bit character code.
         * @return 1 if the glyph is provided, 0 if not.
         */
        int GlyphIsProvided(Uint16 character) const
        {
            return TTF_GlyphIsProvided(font, character);
        }

        /**
         * @brief Check whether a glyph is provided by the font for a 32-bit codepoint.
         *
         * This function covers a larger range of characters compared to GlyphIsProvided.
         *
         * @param character The 32-bit character code.
         * @return 1 if the glyph is provided, 0 if not.
         */
        int GlyphIsProvided32(Uint32 character) const
        {
            return TTF_GlyphIsProvided32(font, character);
        }

        /**
         * @brief Get the metrics (size and advance) of a glyph for a 16-bit character codepoint.
         *
         * @param character The 16-bit character code.
         * @param advance An optional pointer to receive the advance value.
         * @return The rectangle representing the glyph metrics.
         */
        shape2D::Rectangle GetGlyphMetrics(Uint16 character, int* advance = nullptr) const
        {
            shape2D::Rectangle d;
            TTF_GlyphMetrics(font, character, &d.x, &d.w, &d.y, &d.h, advance);
            return d;
        }

        /**
         * @brief Get the metrics (size and advance) of a glyph for a 32-bit character codepoint.
         *
         * @param character The 32-bit character code.
         * @param advance An optional pointer to receive the advance value.
         * @return The rectangle representing the glyph metrics.
         */
        shape2D::Rectangle GetGlyphMetrics32(Uint32 character, int* advance = nullptr) const
        {
            shape2D::Rectangle d;
            TTF_GlyphMetrics32(font, character, &d.x, &d.w, &d.y, &d.h, advance);
            return d;
        }

        /**
         * @brief Get the size of the rendered text for a given string in the current font.
         *
         * @param text The text to measure.
         * @return A 2D vector representing the size of the text.
         */
        math::IVec2 GetSizeText(const std::string& text) const
        {
            math::IVec2 d;
            TTF_SizeText(font, text.c_str(), &d.x, &d.y);
            return d;
        }

        /**
         * @brief Get the size of the rendered UTF-8 text for a given string in the current font.
         *
         * @param text The UTF-8 text to measure.
         * @return A 2D vector representing the size of the text.
         */
        math::IVec2 GetSizeUTF8(const std::string& text) const
        {
            math::IVec2 d;
            TTF_SizeUTF8(font, text.c_str(), &d.x, &d.y);
            return d;
        }

        /**
         * @brief Get the size of the rendered Unicode text for a given string in the current font.
         *
         * @param text The Unicode text to measure.
         * @return A 2D vector representing the size of the text.
         */
        math::IVec2 GetSizeUnicode(const std::basic_string<Uint16>& text) const
        {
            math::IVec2 d;
            TTF_SizeUNICODE(font, text.c_str(), &d.x, &d.y);
            return d;
        }

        /**
         * @brief Calculate how much of a Latin1 string will fit in a given width.
         *
         * This function reports the number of characters that can be rendered before reaching
         * the specified width. It does not need to render the string to do this calculation.
         *
         * @param text The Latin1 string to measure.
         * @param measureWidth The maximum width in which to fit the text.
         * @param extent An optional pointer to receive the latest calculated width.
         * @return The number of characters that fit within the specified width.
         */
        int MeasureText(const std::string& text, int measureWidth, int* extent = nullptr) const
        {
            int count;
            TTF_MeasureText(font, text.c_str(), measureWidth, extent, &count);
            return count;
        }

        /**
         * @brief Calculate how much of a UTF-8 string will fit in a given width.
         *
         * This function reports the number of characters that can be rendered before reaching
         * the specified width. It does not need to render the string to do this calculation.
         *
         * @param text The UTF-8 string to measure.
         * @param measureWidth The maximum width in which to fit the text.
         * @param extent An optional pointer to receive the latest calculated width.
         * @return The number of characters that fit within the specified width.
         */
        int MeasureUTF8(const std::string& text, int measureWidth, int* extent = nullptr) const
        {
            int count;
            TTF_MeasureUTF8(font, text.c_str(), measureWidth, extent, &count);
            return count;
        }

        /**
         * @brief Calculate how much of a Unicode string will fit in a given width.
         *
         * This function reports the number of characters that can be rendered before reaching
         * the specified width. It does not need to render the string to do this calculation.
         *
         * @param text The Unicode string to measure.
         * @param measureWidth The maximum width in which to fit the text.
         * @param extent An optional pointer to receive the latest calculated width.
         * @return The number of characters that fit within the specified width.
         */
        int MeasureUnicode(const std::basic_string<Uint16>& text, int measureWidth, int* extent = nullptr) const
        {
            int count;
            TTF_MeasureUNICODE(font, text.c_str(), measureWidth, extent, &count);
            return count;
        }

        /**
         * @brief Renders text using the specified render mode.
         *
         * @param text The text to render.
         * @param renderMode The rendering mode to use (default: Render::Solid).
         * @param fg The foreground color of the text (default: White).
         * @param nexus The background color behind the text (default: Black).
         * @return The rendered surface containing the text.
         */
        Surface RenderText(const std::string& text, Render renderMode = Render::Solid, const Color& fg = White, const Color& nexus = Black)
        {
            Surface surface;
            switch (renderMode)
            {
                case Render::Solid:     surface = TTF_RenderText_Solid(font, text.c_str(), fg);         break;
                case Render::Shaded:    surface = TTF_RenderText_Shaded(font, text.c_str(), fg, nexus);    break;
                case Render::Blended:   surface = TTF_RenderText_Blended(font, text.c_str(), fg);       break;
                case Render::LCD:       surface = TTF_RenderText_LCD(font, text.c_str(), fg, nexus);       break;
            }
            return surface;
        }

        /**
         * @brief Renders UTF-8 text using the specified render mode.
         *
         * @param text The UTF-8 text to render.
         * @param renderMode The rendering mode to use (default: Render::Solid).
         * @param fg The foreground color of the text (default: White).
         * @param nexus The background color behind the text (default: Black).
         * @return The rendered surface containing the text.
         */
        Surface RenderUTF8(const std::string& text, Render renderMode = Render::Solid, const Color& fg = White, const Color& nexus = Black)
        {
            Surface surface;
            switch (renderMode)
            {
                case Render::Solid:     surface = TTF_RenderUTF8_Solid(font, text.c_str(), fg);         break;
                case Render::Shaded:    surface = TTF_RenderUTF8_Shaded(font, text.c_str(), fg, nexus);    break;
                case Render::Blended:   surface = TTF_RenderUTF8_Blended(font, text.c_str(), fg);       break;
                case Render::LCD:       surface = TTF_RenderUTF8_LCD(font, text.c_str(), fg, nexus);       break;
            }
            return surface;
        }

        /**
         * @brief Renders Unicode text using the specified render mode.
         *
         * @param text The Unicode text to render.
         * @param renderMode The rendering mode to use (default: Render::Solid).
         * @param fg The foreground color of the text (default: White).
         * @param nexus The background color behind the text (default: Black).
         * @return The rendered surface containing the text.
         */
        Surface RenderUnicode(const std::basic_string<Uint16>& text, Render renderMode = Render::Solid, const Color& fg = White, const Color& nexus = Black)
        {
            Surface surface;
            switch (renderMode)
            {
                case Render::Solid:     surface = TTF_RenderUNICODE_Solid(font, text.c_str(), fg);      break;
                case Render::Shaded:    surface = TTF_RenderUNICODE_Shaded(font, text.c_str(), fg, nexus); break;
                case Render::Blended:   surface = TTF_RenderUNICODE_Blended(font, text.c_str(), fg);    break;
                case Render::LCD:       surface = TTF_RenderUNICODE_LCD(font, text.c_str(), fg, nexus);    break;
            }
            return surface;
        }

        /**
         * @brief Renders wrapped text using the specified render mode.
         *
         * @param text The text to render.
         * @param wrapLength The maximum width of the text before wrapping.
         * @param renderMode The rendering mode to use (default: Render::Solid).
         * @param fg The foreground color of the text (default: White).
         * @param nexus The background color behind the text (default: Black).
         * @return The rendered surface containing the wrapped text.
         */
        Surface RenderTextWrapped(const std::string& text, Uint32 wrapLength, Render renderMode = Render::Solid, const Color& fg = White, const Color& nexus = Black)
        {
            Surface surface;
            switch (renderMode)
            {
                case Render::Solid:     surface = TTF_RenderText_Solid_Wrapped(font, text.c_str(), fg, wrapLength);         break;
                case Render::Shaded:    surface = TTF_RenderText_Shaded_Wrapped(font, text.c_str(), fg, nexus, wrapLength);    break;
                case Render::Blended:   surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), fg, wrapLength);       break;
                case Render::LCD:       surface = TTF_RenderText_LCD_Wrapped(font, text.c_str(), fg, nexus, wrapLength);       break;
            }
            return surface;
        }

        /**
         * @brief Renders wrapped UTF-8 text using the specified render mode.
         *
         * @param text The UTF-8 text to render.
         * @param wrapLength The maximum width of the text before wrapping.
         * @param renderMode The rendering mode to use (default: Render::Solid).
         * @param fg The foreground color of the text (default: White).
         * @param nexus The background color behind the text (default: Black).
         * @return The rendered surface containing the wrapped text.
         */
        Surface RenderUTF8Wrapped(const std::string& text, Uint32 wrapLength, Render renderMode = Render::Solid, const Color& fg = White, const Color& nexus = Black)
        {
            Surface surface;
            switch (renderMode)
            {
                case Render::Solid:     surface = TTF_RenderUTF8_Solid_Wrapped(font, text.c_str(), fg, wrapLength);         break;
                case Render::Shaded:    surface = TTF_RenderUTF8_Shaded_Wrapped(font, text.c_str(), fg, nexus, wrapLength);    break;
                case Render::Blended:   surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), fg, wrapLength);       break;
                case Render::LCD:       surface = TTF_RenderUTF8_LCD_Wrapped(font, text.c_str(), fg, nexus, wrapLength);       break;
            }
            return surface;
        }

        /**
         * @brief Renders wrapped Unicode text using the specified render mode.
         *
         * @param text The Unicode text to render.
         * @param wrapLength The maximum width of the text before wrapping.
         * @param renderMode The rendering mode to use (default: Render::Solid).
         * @param fg The foreground color of the text (default: White).
         * @param nexus The background color behind the text (default: Black).
         * @return The rendered surface containing the wrapped text.
         */
        Surface RenderUnicodeWrapped(const std::basic_string<Uint16>& text, Uint32 wrapLength, Render renderMode = Render::Solid, const Color& fg = White, const Color& nexus = Black)
        {
            Surface surface;
            switch (renderMode)
            {
                case Render::Solid:     surface = TTF_RenderUNICODE_Solid_Wrapped(font, text.c_str(), fg, wrapLength);      break;
                case Render::Shaded:    surface = TTF_RenderUNICODE_Shaded_Wrapped(font, text.c_str(), fg, nexus, wrapLength); break;
                case Render::Blended:   surface = TTF_RenderUNICODE_Blended_Wrapped(font, text.c_str(), fg, wrapLength);    break;
                case Render::LCD:       surface = TTF_RenderUNICODE_LCD_Wrapped(font, text.c_str(), fg, nexus, wrapLength);    break;
            }
            return surface;
        }

        /**
         * @brief Renders the glyph for the specified character using the specified render mode.
         *
         * @param character The Unicode character to render.
         * @param renderMode The rendering mode to use (default: Render::Solid).
         * @param fg The foreground color of the text (default: White).
         * @param nexus The background color behind the text (default: Black).
         * @return The rendered surface containing the glyph.
         */
        Surface RenderGlyph(Uint16 character, Render renderMode = Render::Solid, const Color& fg = White, const Color& nexus = Black)
        {
            Surface surface;
            switch (renderMode)
            {
                case Render::Solid:     surface = TTF_RenderGlyph_Solid(font, character, fg);       break;
                case Render::Shaded:    surface = TTF_RenderGlyph_Shaded(font, character, fg, nexus);  break;
                case Render::Blended:   surface = TTF_RenderGlyph_Blended(font, character, fg);     break;
                case Render::LCD:       surface = TTF_RenderGlyph_LCD(font, character, fg, nexus);     break;
            }
            return surface;
        }

        /**
         * @brief Renders the glyph for the specified character (as a 32-bit Unicode value) using the specified render mode.
         *
         * @param character The 32-bit Unicode character to render.
         * @param renderMode The rendering mode to use (default: Render::Solid).
         * @param fg The foreground color of the text (default: White).
         * @param nexus The background color behind the text (default: Black).
         * @return The rendered surface containing the glyph.
         */
        Surface RenderGlyph32(Uint32 character, Render renderMode = Render::Solid, const Color& fg = White, const Color& nexus = Black)
        {
            Surface surface;
            switch (renderMode)
            {
                case Render::Solid:     surface = TTF_RenderGlyph32_Solid(font, character, fg);       break;
                case Render::Shaded:    surface = TTF_RenderGlyph32_Shaded(font, character, fg, nexus);  break;
                case Render::Blended:   surface = TTF_RenderGlyph32_Blended(font, character, fg);     break;
                case Render::LCD:       surface = TTF_RenderGlyph32_LCD(font, character, fg, nexus);     break;
            }
            return surface;
        }

        /**
         * @brief Render a Latin1 text string to a new surface with the specified color.
         *
         * @param text The Latin1 text to render.
         * @param color The color of the rendered text.
         * @return A new Surface containing the rendered text.
         */
        Surface RenderTextSolid(const std::string& text, const Color& color)
        {
            return TTF_RenderText_Solid(font, text.c_str(), color);
        }

        /**
         * @brief Render a UTF-8 text string to a new surface with the specified color.
         *
         * @param text The UTF-8 text to render.
         * @param color The color of the rendered text.
         * @return A new Surface containing the rendered text.
         */
        Surface RenderUTF8Solid(const std::string& text, const Color& color)
        {
            return TTF_RenderUTF8_Solid(font, text.c_str(), color);
        }

        /**
         * @brief Render a Unicode text string to a new surface with the specified color.
         *
         * @param text The Unicode text to render.
         * @param color The color of the rendered text.
         * @return A new Surface containing the rendered text.
         */
        Surface RenderUnicodeSolid(const std::basic_string<Uint16>& text, const Color& color)
        {
            return TTF_RenderUNICODE_Solid(font, text.c_str(), color);
        }

        /**
         * @brief Render a Latin1 text string wrapped within a specified width to a new surface with the specified colors.
         *
         * @param text The Latin1 text to render.
         * @param wrapLength The maximum width for text wrapping.
         * @param color The color of the rendered text.
         * @return A new Surface containing the wrapped and rendered text.
         */
        Surface RenderTextSolidWrapped(const std::string& text, Uint32 wrapLength, const Color& color)
        {
            return TTF_RenderText_Solid_Wrapped(font, text.c_str(), color, wrapLength);
        }

        /**
         * @brief Render a UTF-8 text string wrapped within a specified width to a new surface with the specified colors.
         *
         * @param text The UTF-8 text to render.
         * @param wrapLength The maximum width for text wrapping.
         * @param color The color of the rendered text.
         * @return A new Surface containing the wrapped and rendered text.
         */
        Surface RenderUTF8SolidWrapped(const std::string& text, Uint32 wrapLength, const Color& color)
        {
            return TTF_RenderUTF8_Solid_Wrapped(font, text.c_str(), color, wrapLength);
        }

        /**
         * @brief Render a Unicode text string wrapped within a specified width to a new surface with the specified colors.
         *
         * @param text The Unicode text to render.
         * @param wrapLength The maximum width for text wrapping.
         * @param color The color of the rendered text.
         * @return A new Surface containing the wrapped and rendered text.
         */
        Surface RenderUnicodeSolidWrapped(const std::basic_string<Uint16>& text, Uint32 wrapLength, const Color& color)
        {
            return TTF_RenderUNICODE_Solid_Wrapped(font, text.c_str(), color, wrapLength);
        }

        /**
         * @brief Render a glyph for a Latin1 character to a new surface with the specified color.
         *
         * @param character The Latin1 character for which to render the glyph.
         * @param color The color of the rendered glyph.
         * @return A new Surface containing the rendered glyph.
         */
        Surface RenderGlyphSolid(Uint16 character, const Color& color)
        {
            return TTF_RenderGlyph_Solid(font, character, color);
        }

        /**
         * @brief Render a glyph for a Unicode character to a new surface with the specified color.
         *
         * @param character The Unicode character for which to render the glyph.
         * @param color The color of the rendered glyph.
         * @return A new Surface containing the rendered glyph.
         */
        Surface RenderGlyph32Solid(Uint32 character, const Color& color)
        {
            return TTF_RenderGlyph32_Solid(font, character, color);
        }

        /**
         * @brief Render a Latin1 text string with shading (text on a colored background) to a new surface.
         *
         * @param text The Latin1 text to render.
         * @param fg The color of the rendered text.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered shaded text.
         */
        Surface RenderTextShaded(const std::string& text, const Color& fg, const Color& nexus)
        {
            return TTF_RenderText_Shaded(font, text.c_str(), fg, nexus);
        }

        /**
         * @brief Render a UTF-8 text string with shading (text on a colored background) to a new surface.
         *
         * @param text The UTF-8 text to render.
         * @param fg The color of the rendered text.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered shaded text.
         */
        Surface RenderUTF8Shaded(const std::string& text, const Color& fg, const Color& nexus)
        {
            return TTF_RenderUTF8_Shaded(font, text.c_str(), fg, nexus);
        }

        /**
         * @brief Render a Unicode text string with shading (text on a colored background) to a new surface.
         *
         * @param text The Unicode text to render.
         * @param fg The color of the rendered text.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered shaded text.
         */
        Surface RenderUnicodeShaded(const std::basic_string<Uint16>& text, const Color& fg, const Color& nexus)
        {
            return TTF_RenderUNICODE_Shaded(font, text.c_str(), fg, nexus);
        }

        /**
         * @brief Render a Latin1 text string with shading and wrapping (text on a colored background) to a new surface.
         *
         * @param text The Latin1 text to render.
         * @param wrapLength The maximum width for text wrapping.
         * @param fg The color of the rendered text.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered shaded and wrapped text.
         */
        Surface RenderTextShadedWrapped(const std::string& text, Uint32 wrapLength, const Color& fg, const Color& nexus)
        {
            return TTF_RenderText_Shaded_Wrapped(font, text.c_str(), fg, nexus, wrapLength);
        }

        /**
         * @brief Render a UTF-8 text string with shading and wrapping (text on a colored background) to a new surface.
         *
         * @param text The UTF-8 text to render.
         * @param wrapLength The maximum width for text wrapping.
         * @param fg The color of the rendered text.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered shaded and wrapped text.
         */
        Surface RenderUTF8ShadedWrapped(const std::string& text, Uint32 wrapLength, const Color& fg, const Color& nexus)
        {
            return TTF_RenderUTF8_Shaded_Wrapped(font, text.c_str(), fg, nexus, wrapLength);
        }

        /**
         * @brief Render a Unicode text string with shading and wrapping (text on a colored background) to a new surface.
         *
         * @param text The Unicode text to render.
         * @param wrapLength The maximum width for text wrapping.
         * @param fg The color of the rendered text.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered shaded and wrapped text.
         */
        Surface RenderUnicodeShadedWrapped(const std::basic_string<Uint16>& text, Uint32 wrapLength, const Color& fg, const Color& nexus)
        {
            return TTF_RenderUNICODE_Shaded_Wrapped(font, text.c_str(), fg, nexus, wrapLength);
        }

        /**
         * @brief Render a glyph for a Latin1 character with shading (text on a colored background) to a new surface.
         *
         * @param character The Latin1 character for which to render the glyph.
         * @param fg The color of the rendered glyph.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered shaded glyph.
         */
        Surface RenderGlyphShaded(Uint16 character, const Color& fg, const Color& nexus)
        {
            return TTF_RenderGlyph_Shaded(font, character, fg, nexus);
        }

        /**
         * @brief Render a glyph for a Unicode character with shading (text on a colored background) to a new surface.
         *
         * @param character The Unicode character for which to render the glyph.
         * @param fg The color of the rendered glyph.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered shaded glyph.
         */
        Surface RenderGlyphShaded32(Uint32 character, const Color& fg, const Color& nexus)
        {
            return TTF_RenderGlyph32_Shaded(font, character, fg, nexus);
        }

        /**
         * @brief Render a Latin1 text string with blending (anti-aliased) to a new surface.
         *
         * @param text The Latin1 text to render.
         * @param color The color of the rendered text.
         * @return A new Surface containing the rendered blended text.
         */
        Surface RenderTextBlended(const std::string& text, const Color& color)
        {
            return TTF_RenderText_Blended(font, text.c_str(), color);
        }

        /**
         * @brief Render a UTF-8 text string with blending (anti-aliased) to a new surface.
         *
         * @param text The UTF-8 text to render.
         * @param color The color of the rendered text.
         * @return A new Surface containing the rendered blended text.
         */
        Surface RenderUTF8Blended(const std::string& text, const Color& color)
        {
            return TTF_RenderUTF8_Blended(font, text.c_str(), color);
        }

        /**
         * @brief Render a Unicode text string with blending (anti-aliased) to a new surface.
         *
         * @param text The Unicode text to render.
         * @param color The color of the rendered text.
         * @return A new Surface containing the rendered blended text.
         */
        Surface RenderUnicodeBlended(const std::basic_string<Uint16>& text, const Color& color)
        {
            return TTF_RenderUNICODE_Blended(font, text.c_str(), color);
        }

        /**
         * @brief Render a Latin1 text string with blending and wrapping (text on a colored background) to a new surface.
         *
         * @param text The Latin1 text to render.
         * @param wrapLength The maximum width for text wrapping.
         * @param color The color of the rendered text.
         * @return A new Surface containing the rendered blended and wrapped text.
         */
        Surface RenderTextBlendedWrapped(const std::string& text, Uint32 wrapLength, const Color& color)
        {
            return TTF_RenderText_Solid_Wrapped(font, text.c_str(), color, wrapLength);
        }

        /**
         * @brief Render a UTF-8 text string with blending and wrapping (text on a colored background) to a new surface.
         *
         * @param text The UTF-8 text to render.
         * @param wrapLength The maximum width for text wrapping.
         * @param color The color of the rendered text.
         * @return A new Surface containing the rendered blended and wrapped text.
         */
        Surface RenderUTF8BlendedWrapped(const std::string& text, Uint32 wrapLength, const Color& color)
        {
            return TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, wrapLength);
        }

        /**
         * @brief Render a Unicode text string with blending and wrapping (text on a colored background) to a new surface.
         *
         * @param text The Unicode text to render.
         * @param wrapLength The maximum width for text wrapping.
         * @param color The color of the rendered text.
         * @return A new Surface containing the rendered blended and wrapped text.
         */
        Surface RenderUnicodeBlendedWrapped(const std::basic_string<Uint16>& text, Uint32 wrapLength, const Color& color)
        {
            return TTF_RenderUNICODE_Blended_Wrapped(font, text.c_str(), color, wrapLength);
        }

        /**
         * @brief Render a glyph for a Latin1 character with blending (text on a colored background) to a new surface.
         *
         * @param character The Latin1 character for which to render the glyph.
         * @param color The color of the rendered glyph.
         * @return A new Surface containing the rendered blended glyph.
         */
        Surface RenderGlyphBlended(Uint16 character, const Color& color)
        {
            return TTF_RenderGlyph_Blended(font, character, color);
        }

        /**
         * @brief Render a glyph for a Unicode character with blending (text on a colored background) to a new surface.
         *
         * @param character The Unicode character for which to render the glyph.
         * @param color The color of the rendered glyph.
         * @return A new Surface containing the rendered blended glyph.
         */
        Surface RenderGlyph32Blended(Uint32 character, const Color& color)
        {
            return TTF_RenderGlyph32_Blended(font, character, color);
        }

        /**
         * @brief Render a Latin1 text string with LCD filtering (text on a colored background) to a new surface.
         *
         * @param text The Latin1 text to render.
         * @param fg The color of the rendered text.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered text with LCD filtering.
         */
        Surface RenderTextLCD(const std::string& text, const Color& fg, const Color& nexus)
        {
            return TTF_RenderText_LCD(font, text.c_str(), fg, nexus);
        }

        /**
         * @brief Render a UTF-8 text string with LCD filtering (text on a colored background) to a new surface.
         *
         * @param text The UTF-8 text to render.
         * @param fg The color of the rendered text.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered text with LCD filtering.
         */
        Surface RenderUTF8LCD(const std::string& text, const Color& fg, const Color& nexus)
        {
            return TTF_RenderUTF8_LCD(font, text.c_str(), fg, nexus);
        }

        /**
         * @brief Render a Unicode text string with LCD filtering (text on a colored background) to a new surface.
         *
         * @param text The Unicode text to render.
         * @param fg The color of the rendered text.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered text with LCD filtering.
         */
        Surface RenderUnicodeLCD(const std::basic_string<Uint16>& text, const Color& fg, const Color& nexus)
        {
            return TTF_RenderUNICODE_LCD(font, text.c_str(), fg, nexus);
        }

        /**
         * @brief Render a Latin1 text string with LCD filtering and wrapping (text on a colored background) to a new surface.
         *
         * @param text The Latin1 text to render.
         * @param wrapLength The maximum width for text wrapping.
         * @param fg The color of the rendered text.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered text with LCD filtering and wrapping.
         */
        Surface RenderTextLCDWrapped(const std::string& text, Uint32 wrapLength, const Color& fg, const Color& nexus)
        {
            return TTF_RenderText_LCD_Wrapped(font, text.c_str(), fg, nexus, wrapLength);
        }

        /**
         * @brief Render a UTF-8 text string with LCD filtering and wrapping (text on a colored background) to a new surface.
         *
         * @param text The UTF-8 text to render.
         * @param wrapLength The maximum width for text wrapping.
         * @param fg The color of the rendered text.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered text with LCD filtering and wrapping.
         */
        Surface RenderUTF8LCDWrapped(const std::string& text, Uint32 wrapLength, const Color& fg, const Color& nexus)
        {
            return TTF_RenderUTF8_LCD_Wrapped(font, text.c_str(), fg, nexus, wrapLength);
        }

        /**
         * @brief Render a Unicode text string with LCD filtering and wrapping (text on a colored background) to a new surface.
         *
         * @param text The Unicode text to render.
         * @param wrapLength The maximum width for text wrapping.
         * @param fg The color of the rendered text.
         * @param nexus The color of the background behind the text.
         * @return A new Surface containing the rendered text with LCD filtering and wrapping.
         */
        Surface RenderUnicodeLCDWrapped(const std::basic_string<Uint16>& text, Uint32 wrapLength, const Color& fg, const Color& nexus)
        {
            return TTF_RenderUNICODE_LCD_Wrapped(font, text.c_str(), fg, nexus, wrapLength);
        }

        /**
         * @brief Render a glyph for a Latin1 character with LCD filtering (text on a colored background) to a new surface.
         *
         * @param character The Latin1 character for which to render the glyph.
         * @param fg The color of the rendered glyph.
         * @param nexus The color of the background behind the glyph.
         * @return A new Surface containing the rendered glyph with LCD filtering.
         */
        Surface RenderGlyphLCD(Uint16 character, const Color& fg, const Color& nexus)
        {
            return TTF_RenderGlyph_LCD(font, character, fg, nexus);
        }

        /**
         * @brief Render a glyph for a Unicode character with LCD filtering (text on a colored background) to a new surface.
         *
         * @param character The Unicode character for which to render the glyph.
         * @param fg The color of the rendered glyph.
         * @param nexus The color of the background behind the glyph.
         * @return A new Surface containing the rendered glyph with LCD filtering.
         */
        Surface RenderGlyphLCD32(Uint32 character, const Color& fg, const Color& nexus)
        {
            return TTF_RenderGlyph32_LCD(font, character, fg, nexus);
        }
    };

}}

#endif //NEXUS_GFX_BASIC_FONT_HPP
