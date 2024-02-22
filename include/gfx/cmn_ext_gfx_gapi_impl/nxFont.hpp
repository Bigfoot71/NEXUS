#ifndef NEXUS_CMN_EXT_GFX_GAPI_IMPL_FONT_HPP
#define NEXUS_CMN_EXT_GFX_GAPI_IMPL_FONT_HPP

#include "../../utils/nxContextual.hpp"
#include "../../gfx/nxBasicFont.hpp"
#include "../../gfx/nxSurface.hpp"
#include "../../gfx/nxPixel.hpp"
#include "../../gfx/nxColor.hpp"
#include "../../math/nxVec2.hpp"

/* Basic implementation common to ext_gfx and gapi for glyph caching. */

namespace _ext_gfx_gapi_impl {

    struct KeyGlyph
    {
        Uint16 character;
        Uint8 renderMode;
        Uint8 hinting;
        Uint8 style;
        bool sdf;

        KeyGlyph(Uint16 character,
            nexus::gfx::BasicFont::Render renderMode,
            nexus::gfx::BasicFont::Hinting hinting,
            nexus::gfx::BasicFont::Style style,
            bool sdf)
        : character(character)
        , renderMode(static_cast<Uint8>(renderMode))
        , hinting(static_cast<Uint8>(hinting))
        , style(static_cast<Uint8>(style))
        , sdf(sdf)
        { }

        Uint16 GetChar() const { return character; }

        nexus::gfx::BasicFont::Render GetRenderMode() const
        {
            return static_cast<nexus::gfx::BasicFont::Render>(renderMode);
        }

        nexus::gfx::BasicFont::Hinting GetHinting() const
        {
            return static_cast<nexus::gfx::BasicFont::Hinting>(hinting);
        }

        nexus::gfx::BasicFont::Style GetStyle() const
        {
            return static_cast<nexus::gfx::BasicFont::Style>(style);
        }

        bool IsSDF() const
        {
            return sdf;
        }

        bool operator==(const KeyGlyph& other) const
        {
            return !(
                character   != other.character      ||
                renderMode  != other.renderMode     ||
                hinting     != other.hinting        ||
                style       != other.style          ||
                sdf         != other.sdf
            );
        }
    };

    struct KeyGlyphHash
    {
        size_t operator()(const KeyGlyph& key) const
        {
            size_t hash = 0;

            HashCombine(hash, std::hash<Uint16>()(key.character));
            HashCombine(hash, std::hash<Uint8>()(key.renderMode));
            HashCombine(hash, std::hash<Uint8>()(key.hinting));
            HashCombine(hash, std::hash<Uint8>()(key.style));
            HashCombine(hash, std::hash<bool>()(key.sdf));

            return hash;
        }

    private:
        static void HashCombine(size_t& seed, size_t hash) noexcept
        {
            // The magic number is derived from the golden ratio's
            // fractional part, which has nice scattering properties
            seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    };

    template <typename T_Context>
    struct TextureGlyph
    {
        virtual ~TextureGlyph() = default;
    };

    template <typename T_Context, typename T_TextureGlyph>
    class Font : public nexus::gfx::BasicFont, public nexus::utils::Contextual<T_Context>
    {
        static_assert(std::is_base_of<TextureGlyph<T_Context>, T_TextureGlyph>::value,
            "[Font] <T_Texture> must be derived from _gapi_impl::TextureGlyph<T_Context>");

      protected:
        using MapGlyph = std::unordered_map<KeyGlyph, T_TextureGlyph, KeyGlyphHash>;

      protected:
        MapGlyph glyphCache;    ///< Cache for storing rendered glyphs.
        Render renderMode;      ///< Current Render mode used for render glyphs.

      protected:
        /**
         * @brief Caches a glyph for later use.
         *
         * @note: This function is a pure virtual function
         * because not all T_TextureGlyph will necessarily take
         * the same construction parameters.
         *
         * @param keyGlyph The key of the glyph to cache.
         * @return An iterator pointing to the cached glyph.
         */
        virtual T_TextureGlyph& CacheGlyph(KeyGlyph keyGlyph) = 0;

        /**
         * @brief Retrieves the requested glyph, caches it if not found.
         *
         * @param keyGlyph The key of the glyph to retrieve.
         * @return An iterator pointing to the retrieved glyph.
         */
        T_TextureGlyph& GetGlyph(KeyGlyph keyGlyph);

        /**
        * @brief Generates the glyph key using the provided character along with the current rendering mode and font style.
        *
        * This function generates the key for a glyph by combining the
        * provided character with the current rendering mode and font style.
        *
        * @param character The character used to generate the glyph key.
        * @return The glyph key generated based on the given character and the current rendering mode and font style.
        */
        KeyGlyph GenKeyGlyphFromCurrentState(Uint16 character) const;

      public:
        Font(T_Context& ctx, const std::string& fontPath, int fontSize, bool sdf = false, Render renderMode = Render::Blended);

        virtual ~Font() = default;

        /**
         * @brief Caches a glyph according to the current state of the font.
         *
         * @param c The character for which to cache the glyph.
         */
        void CacheGlyph(Uint16 c);

        /**
         * @brief Cache all printable ASCII glyphs according to the current state of the Font.
         */
        void CacheAllGlyphs();

        /**
         * @brief Clears the glyph cache.
         */
        void ClearCache();

        /**
         * @brief Gets the current render mode of the Font.
         *
         * @return The current render mode.
         */
        Render GetRenderMode() const;

        /**
         * @brief Sets the render mode of the Font.
         *
         * @param renderMode The render mode to set.
         */
        void SetRenderMode(Render renderMode) const;
    };


    /* Private Implementation Font */

    template <typename T_Context, typename T_TextureGlyph>
    T_TextureGlyph& Font<T_Context, T_TextureGlyph>::GetGlyph(KeyGlyph keyGlyph)
    {
        auto it = this->glyphCache.find(keyGlyph);

        if (it == this->glyphCache.end())
        {
            return CacheGlyph(keyGlyph);
        }

        return it->second;
    }

    template <typename T_Context, typename T_TextureGlyph>
    KeyGlyph Font<T_Context, T_TextureGlyph>::GenKeyGlyphFromCurrentState(Uint16 character) const
    {
        return KeyGlyph(character, this->renderMode,
            this->GetHinting(), this->GetStyle(),
            this->IsSDF());
    }

    /* Public Implementation Font */

    template <typename T_Context, typename T_TextureGlyph>
    Font<T_Context, T_TextureGlyph>::Font(T_Context& ctx, const std::string& fontPath, int fontSize, bool sdf, Render renderMode)
    : nexus::gfx::BasicFont(fontPath, fontSize, sdf)
    , nexus::utils::Contextual<T_Context>(ctx)
    , renderMode(renderMode)
    { }

    template <typename T_Context, typename T_TextureGlyph>
    void Font<T_Context, T_TextureGlyph>::CacheGlyph(Uint16 c)
    {
        this->CacheGlyph(GenKeyGlyphFromCurrentState(c));
    }

    template <typename T_Context, typename T_TextureGlyph>
    void Font<T_Context, T_TextureGlyph>::CacheAllGlyphs()
    {
        for (Uint16 c = 32; c <= 126; c++)
        {
            this->CacheGlyph(GenKeyGlyphFromCurrentState(c));
        }
    }

    template <typename T_Context, typename T_TextureGlyph>
    void Font<T_Context, T_TextureGlyph>::ClearCache()
    {
        glyphCache.clear();
    }

    template <typename T_Context, typename T_TextureGlyph>
    nexus::gfx::BasicFont::Render Font<T_Context, T_TextureGlyph>::GetRenderMode() const
    {
        return renderMode;
    }

    template <typename T_Context, typename T_TextureGlyph>
    void Font<T_Context, T_TextureGlyph>::SetRenderMode(Render renderMode) const
    {
        this->renderMode = renderMode;
    }

}

#endif //NEXUS_CMN_EXT_GFX_GAPI_IMPL_FONT_HPP
