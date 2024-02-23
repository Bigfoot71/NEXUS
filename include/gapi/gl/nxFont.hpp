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

#ifndef NEXUS_GL_FONT_HPP
#define NEXUS_GL_FONT_HPP

#include "../cmn_impl/nxFont.hpp"

#include "../../gfx/nxBasicFont.hpp"
#include "../../gfx/nxSurface.hpp"
#include "../../gfx/nxPixel.hpp"
#include "../../gfx/nxColor.hpp"
#include "../../math/nxVec2.hpp"
#include "./nxContextual.hpp"
#include "./nxContext.hpp"
#include "./nxTexture.hpp"
#include "./nxEnums.hpp"

namespace _gl_impl {

    struct TextureGlyph : _gapi_impl::TextureGlyph<nexus::gl::Context>
    {
        Uint32 id;
        Uint16 width, height;

        TextureGlyph(const nexus::gfx::Surface& surface);
        ~TextureGlyph();

        float Draw(nexus::gl::Context& ctx, float x, float y, float scale, const nexus::gfx::Color& tint) const override;
        float Draw3D(nexus::gl::Context& ctx, const nexus::math::Vec3& position, float scale, const nexus::gfx::Color& tint) const override;
    };

}

namespace nexus { namespace gl {

    /**
     * @brief Basic derivation of the gfx::BasicFont class enabling text rendering in a gl::Context with glyph caching.
     */
    class NEXUS_API Font : public Container<_gapi_impl::Font<Context, _gl_impl::TextureGlyph>>
    {
      public:
        /**
         * @brief Enumeration for font styles.
         */
        using Style = gfx::BasicFont::Style;

        /**
         * @brief Enumeration for font hinting styles.
         */
        using Hinting = gfx::BasicFont::Hinting;

        /**
         * @brief Enumeration for text alignment when wrapping text.
         */
        using WrappedAlign = gfx::BasicFont::WrappedAlign;

        /**
         * @brief Enumeration for text writing direction.
         */
        using Direction = gfx::BasicFont::Direction;

        /**
         * @brief Enumeration for text rendering methods.
         */
        using Render = gfx::BasicFont::Render;

      public:
        /**
         * @brief Constructor for the Font class.
         *
         * @param ctx The OpenGL context.
         * @param renderer Reference to the Renderer object.
         * @param fontPath Path to the font file.
         * @param fontSize Size of the font.
         * @param sdf If true, enables Signed Distance Field (SDF) rendering.
         */
        Font(Context& ctx, const std::string& fontPath, int fontSize, bool sdf = false)
        : Container(ctx, fontPath, fontSize, sdf)
        { }
    };

}}

#endif //NEXUS_GL_FONT_HPP
