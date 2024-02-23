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

#include "gapi/sr/nxFont.hpp"
#include "gapi/sr/nxContextual.hpp"
#include "math/nxMath.hpp"

using namespace nexus;

/* Private Implementation SoftTextureGlyph */

_sr_impl::TextureGlyph::TextureGlyph(gfx::Surface&& surface)
: surface(std::move(surface))
{
    width = this->surface.GetWidth();
    height = this->surface.GetHeight();
}

float _sr_impl::TextureGlyph::Draw(sr::Context& ctx, float x, float y, float scale, const gfx::Color& tint) const
{
    const float w = width * scale;
    const float h = height * scale;

    ctx.SetTexture(&surface);
    ctx.Begin(sr::DrawMode::Quads);

        ctx.Color(tint);

        ctx.TexCoord(0, 0);
        ctx.Vertex(x, y);

        ctx.TexCoord(0, 1);
        ctx.Vertex(x, y + h);

        ctx.TexCoord(1, 1);
        ctx.Vertex(x + w, y + h);

        ctx.TexCoord(1, 0);
        ctx.Vertex(x + w, y);

    ctx.End();
    ctx.SetTexture(nullptr);

    return w;
}

float _sr_impl::TextureGlyph::Draw3D(sr::Context& ctx, const math::Vec3& position, float scale, const gfx::Color& tint) const
{
    const float w = width * scale;
    const float h = height * scale;

    ctx.SetTexture(&surface);
    ctx.Begin(sr::DrawMode::Quads);

        ctx.Color(tint);

        // Front Face
        ctx.Normal(0, 1, 0);

        ctx.TexCoord(0, 1);
        ctx.Vertex(position);

        ctx.TexCoord(0, 0);
        ctx.Vertex(position.x, position.y + h, position.z);

        ctx.TexCoord(1, 0);
        ctx.Vertex(position.x + w, position.y + h, position.z);

        ctx.TexCoord(1, 1);
        ctx.Vertex(position.x + w, position.y, position.z);

        // Back Face
        ctx.Normal(0, -1, 0);

        ctx.TexCoord(0, 1);
        ctx.Vertex(position);

        ctx.TexCoord(1, 1);
        ctx.Vertex(position.x + w, position.y, position.z);

        ctx.TexCoord(1, 0);
        ctx.Vertex(position.x + w, position.y + h, position.z);

        ctx.TexCoord(0, 0);
        ctx.Vertex(position.x, position.y + h, position.z);

    ctx.End();
    ctx.SetTexture(nullptr);

    return w;
}
