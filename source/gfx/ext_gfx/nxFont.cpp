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

#include "gfx/ext_gfx/nxFont.hpp"
#include "SDL_render.h"

using namespace nexus;

/* Private Implementation TextureGlyph */

_gfx_impl::TextureGlyph::TextureGlyph(nexus::gfx::Renderer& ctx, const nexus::gfx::Surface& surface)
: width(surface.GetWidth()), height(surface.GetHeight())
{
    texture = SDL_CreateTextureFromSurface(ctx, surface.Get());
}

_gfx_impl::TextureGlyph::~TextureGlyph()
{
    SDL_DestroyTexture(texture);
}

float _gfx_impl::TextureGlyph::Draw(nexus::gfx::Renderer& ctx, float x, float y, float scale) const
{
    const nexus::shape2D::RectangleF rectDst(x, y, width * scale, height * scale);
    SDL_RenderCopyF(ctx, texture, nullptr, &rectDst);
    return rectDst.w;
}


/* Private Implementation Font */

_gfx_impl::TextureGlyph& _gfx_impl::Font::CacheGlyph(KeyGlyph keyGlyph)
{
    return glyphCache.emplace(std::piecewise_construct, std::forward_as_tuple(keyGlyph),
        std::forward_as_tuple(this->ctx, RenderGlyph(keyGlyph.GetChar(), keyGlyph.GetRenderMode(), nexus::gfx::White, nexus::gfx::Black))).first->second;
}


/* Public Implementation Font */

void _gfx_impl::Font::Draw(const std::string& text, float x, float y, float scale, const gfx::Color& tint)
{
    for (char c : text)
    {
        auto &glyph = this->GetGlyph(this->GenKeyGlyphFromCurrentState(c));
        SDL_SetTextureColorMod(glyph.texture, tint.r, tint.g, tint.b);
        SDL_SetTextureAlphaMod(glyph.texture, tint.a);
        x += glyph.Draw(this->ctx, x, y, scale);
    }
}

void _gfx_impl::Font::Draw(const std::string& text, const nexus::math::Vec2 position, float scale, const gfx::Color& tint)
{
    this->Draw(text, position.x, position.y, scale);
}

void _gfx_impl::Font::DrawCentered(const std::string& text, float x, float y, float scale, const gfx::Color& tint)
{
    const nexus::math::Vec2 size = nexus::math::Vec2(GetSizeText(text)) * scale * 0.5f;
    this->Draw(text, x - size.x, y - size.y, scale);
}

void _gfx_impl::Font::DrawCentered(const std::string& text, const nexus::math::Vec2& position, float scale, const gfx::Color& tint)
{
    this->DrawCentered(text, position.x, position.y, scale);
}
