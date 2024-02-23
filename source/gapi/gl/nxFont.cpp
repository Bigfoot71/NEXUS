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

#include "gapi/gl/nxFont.hpp"
#include "math/nxMath.hpp"

using namespace nexus;

/* Private Implementation TextureGlyph */

_gl_impl::TextureGlyph::TextureGlyph(const gfx::Surface& surface)
: width(surface.GetWidth()), height(surface.GetHeight())
{
    const auto format = _gl_impl::ConvertPixelFormat(surface.GetPixelFormat());

    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // TODO: FIX ME: For a reason unknown to me we need to use Surface::Clone,
    // which creates a copy using SDL_ConvertSurface in exactly the same format.
    // This copy works when uploaded to the GPU, but not the original version. (?)
    const gfx::Surface glyphSurface = surface.Clone();

    Uint32 glInternalFormat, glFormat, glType;
    GetGlTextureFormats(format, &glInternalFormat, &glFormat, &glType);
    glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glType, glyphSurface.GetPixels());

# if defined(GRAPHICS_API_OPENGL_33)
    if (format == gl::TextureFormat::GrayAlpha)
    {
#       if defined(GRAPHICS_API_OPENGL_21)
            constexpr GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ALPHA };
#       elif defined(GRAPHICS_API_OPENGL_33)
            constexpr GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
#       endif

        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    }
#endif

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
}

_gl_impl::TextureGlyph::~TextureGlyph()
{
    glDeleteTextures(1, &id);
}

float _gl_impl::TextureGlyph::Draw(gl::Context& ctx, float x, float y, float scale, const gfx::Color& tint) const
{
    const float w = width * scale;
    const float h = height * scale;

    ctx.SetTexture(id);
    ctx.Begin(gl::DrawMode::Quads);

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
    ctx.SetTexture(0u);

    return w;
}

float _gl_impl::TextureGlyph::Draw3D(gl::Context& ctx, const math::Vec3& position, float scale, const gfx::Color& tint) const
{
    const float w = width * scale;
    const float h = height * scale;

    ctx.SetTexture(id);
    ctx.Begin(gl::DrawMode::Quads);

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
    ctx.SetTexture(0u);

    return w;
}
