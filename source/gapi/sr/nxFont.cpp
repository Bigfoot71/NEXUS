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
