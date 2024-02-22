#include "gfx/ext_gfx/nxTexture.hpp"

#include "gfx/ext_gfx/nxRenderer.hpp"
#include "math/nxMath.hpp"
#include "shape/2D/nxRectangle.hpp"
#include "gfx/nxPixel.hpp"

using namespace nexus;

/* Private Texture Implementation */

void _gfx_impl::Texture::Create(int w, int h, gfx::PixelFormat format, gfx::Texture::Access access)
{
    data = SDL_CreateTexture(ctx, static_cast<Uint32>(format), static_cast<int>(access), w, h);
    width = w, height = h;
}

void _gfx_impl::Texture::LoadFromImageFile(const std::string& filename)
{
    gfx::Surface loadedSurface(filename);
    data = SDL_CreateTextureFromSurface(ctx, loadedSurface);

    if (data == nullptr)
    {
        throw core::NexusException("gfx::Texture", "Unable to create texture from [" + filename + "].",
            "SDL", SDL_GetError());
    }

    width = loadedSurface.GetWidth();
    height = loadedSurface.GetHeight();
}

void _gfx_impl::Texture::LoadFromSurface(gfx::Surface& surface)
{
    data = SDL_CreateTextureFromSurface(ctx, surface);

    if (data == nullptr)
    {
        throw core::NexusException("gfx::Texture", "Unable to create ",
            "SDL", SDL_GetError());
    }

    width = surface.GetWidth();
    height = surface.GetHeight();
}

_gfx_impl::Texture::Texture(gfx::Renderer& ctx)
: utils::Contextual<gfx::Renderer>(ctx)
{
    Create(1, 1, gfx::PixelFormat::RGBA8888, gfx::TextureAccess::Static);
}

_gfx_impl::Texture::Texture(gfx::Renderer& ctx, gfx::Surface& surface)
: utils::Contextual<gfx::Renderer>(ctx)
{
    LoadFromSurface(surface);
}

_gfx_impl::Texture::Texture(gfx::Renderer& ctx, gfx::Surface&& surface)
: utils::Contextual<gfx::Renderer>(ctx)
{
    LoadFromSurface(surface);
}

_gfx_impl::Texture::Texture(gfx::Renderer& ctx, const std::string& fileName)
: utils::Contextual<gfx::Renderer>(ctx)
{
    LoadFromImageFile(fileName);
}

_gfx_impl::Texture::Texture(gfx::Renderer& ctx, int w, int h, gfx::PixelFormat format, gfx::TextureAccess access)
: utils::Contextual<gfx::Renderer>(ctx)
{
    Create(w, h, format, access);
}

_gfx_impl::Texture::~Texture()
{
    if (data != nullptr)
    {
        SDL_DestroyTexture(data);
    }
}

void _gfx_impl::Texture::Draw(const shape2D::Rectangle& src, const shape2D::RectangleF& dst, const math::Vec2& origin, float rotation, gfx::TextureFlip flip) const
{
    const shape2D::RectangleF dest(dst.x - origin.x, dst.y - origin.y, dst.w, dst.h);
    SDL_RenderCopyExF(ctx, data, &src, &dest, rotation, reinterpret_cast<const SDL_FPoint*>(&origin), static_cast<SDL_RendererFlip>(flip));
}

void _gfx_impl::Texture::Draw(float x, float y) const
{
    const shape2D::RectangleF rectDst = { x, y, static_cast<float>(width), static_cast<float>(height) };
    SDL_RenderCopyF(ctx, data, nullptr, &rectDst);
}

void _gfx_impl::Texture::Draw(const math::Vec2& position) const
{
    Draw(position.x, position.y);
}

void _gfx_impl::Texture::Draw(const math::Vec2& position, float rotation, float scale, gfx::TextureFlip flip) const
{
    const math::Vec2 size = math::Vec2(width, height) * scale;
    const math::Vec2 origin = size * 0.5f;

    const shape2D::RectangleF rectDst = { position.x - origin.x, position.y - origin.y, size.x, size.y };
    SDL_RenderCopyExF(ctx, data, nullptr, &rectDst, rotation, reinterpret_cast<const SDL_FPoint*>(&origin), static_cast<SDL_RendererFlip>(flip));
}

void _gfx_impl::Texture::Draw(const math::Vec2& position, float rotation, const math::Vec2& origin, const math::Vec2& scale, gfx::TextureFlip flip) const
{
    const shape2D::RectangleF rectDst = { position.x - origin.x, position.y - origin.y, width * scale.x, height * scale.y };
    SDL_RenderCopyExF(ctx, data, nullptr, &rectDst, rotation, reinterpret_cast<const SDL_FPoint*>(&origin), static_cast<SDL_RendererFlip>(flip));
}

void _gfx_impl::Texture::Draw(const shape2D::Rectangle& src, const math::Vec2& position, gfx::TextureFlip flip) const
{
    const shape2D::RectangleF rectDst = { position.x, position.y, static_cast<float>(width), static_cast<float>(height) };
    SDL_RenderCopyExF(ctx, data, &src, &rectDst, 0.0f, nullptr, static_cast<SDL_RendererFlip>(flip));
}

void _gfx_impl::Texture::DrawNinePatch(gfx::NinePatchInfo ninePatchInfo, const shape2D::RectangleF& dest, const math::Vec2& origin, float rotation, const gfx::Color& tint) const
{
    float patchWidth = std::max(dest.w, 0.0f);
    float patchHeight = std::max(dest.h, 0.0f);

    if (ninePatchInfo.source.w < 0) ninePatchInfo.source.x -= ninePatchInfo.source.w;
    if (ninePatchInfo.source.h < 0) ninePatchInfo.source.y -= ninePatchInfo.source.h;

    switch (ninePatchInfo.layout)
    {
        case gfx::NinePatchInfo::Layout::ThreePatchHorizontal:
            patchHeight = ninePatchInfo.source.h;
            break;

        case gfx::NinePatchInfo::Layout::ThreePatchVertical:
            patchWidth = ninePatchInfo.source.w;
            break;

        default:
            break;
    }

    bool drawCenter = true;
    bool drawMiddle = true;
    shape2D::AABB bounds = ninePatchInfo.bounds;

    // Adjust the lateral (left and right) border widths in case patchWidth < texture.width
    if (ninePatchInfo.layout != gfx::NinePatchInfo::Layout::ThreePatchVertical && patchWidth <= bounds.min.x + bounds.max.x)
    {
        drawCenter = false;
        bounds.min.x = bounds.min.x / (bounds.min.x + bounds.max.x) * patchWidth;
        bounds.max.x = patchWidth - bounds.min.x;
    }

    // Adjust the lateral (top and bottom) border heights in case patchHeight < texture.height
    if (ninePatchInfo.layout != gfx::NinePatchInfo::Layout::ThreePatchHorizontal && patchHeight <= bounds.min.y + bounds.max.y)
    {
        drawMiddle = false;
        bounds.min.y = (bounds.min.y/(bounds.min.y + bounds.max.y))*patchHeight;
        bounds.max.y = patchHeight - bounds.min.y;
    }

    math::Vec2 vertA, vertB, vertC, vertD;

    if (rotation != 0)
    {
        rotation *= math::Deg2Rad;
        vertA = dest.GetPosition().Rotated(rotation) - origin;
        vertB = (dest.GetPosition() + bounds.min).Rotated(rotation) - origin;
        vertC = (dest.GetPosition() + (math::Vec2(patchWidth, patchHeight) - bounds.max)).Rotated(rotation) - origin;
        vertD = dest.GetPosition() + math::Vec2(patchWidth, patchHeight).Rotated(rotation) - origin;
    }
    else
    {
        vertA = dest.GetPosition() - origin;
        vertB = (dest.GetPosition() + bounds.min) - origin;
        vertC = (dest.GetPosition() + (math::Vec2(patchWidth, patchHeight) - bounds.max)) - origin;
        vertD = dest.GetPosition() + math::Vec2(patchWidth, patchHeight) - origin;
    }

    math::Vec2 coordA, coordB, coordC, coordD;
    coordA.x = static_cast<float>(ninePatchInfo.source.x) / width;
    coordA.y = static_cast<float>(ninePatchInfo.source.y) / height;
    coordB.x = static_cast<float>(ninePatchInfo.source.x + bounds.min.x) / width;
    coordB.y = static_cast<float>(ninePatchInfo.source.y + bounds.min.y) / height;
    coordC.x = static_cast<float>(ninePatchInfo.source.x + ninePatchInfo.source.w  - bounds.max.x) / width;
    coordC.y = static_cast<float>(ninePatchInfo.source.y + ninePatchInfo.source.h - bounds.max.y) / height;
    coordD.x = static_cast<float>(ninePatchInfo.source.x + ninePatchInfo.source.w) / width;
    coordD.y = static_cast<float>(ninePatchInfo.source.y + ninePatchInfo.source.h) / height;

    switch (ninePatchInfo.layout)
    {
        case gfx::NinePatchInfo::Layout::NinePatch:
        {
            // TOP-LEFT QUAD
            ctx.DrawQuad(
                shape2D::Vertex{ { vertA.x, vertB.y }, tint, { coordA.x, coordB.y } },
                shape2D::Vertex{ { vertB.x, vertB.y }, tint, { coordB.x, coordB.y } },
                shape2D::Vertex{ { vertB.x, vertA.y }, tint, { coordB.x, coordA.y } },
                shape2D::Vertex{ { vertA.x, vertA.y }, tint, { coordA.x, coordA.y } },
                data);

            if (drawCenter)
            {
                // TOP-CENTER QUAD
                ctx.DrawQuad(
                    shape2D::Vertex{ { vertB.x, vertB.y }, tint, { coordB.x, coordB.y } },
                    shape2D::Vertex{ { vertC.x, vertB.y }, tint, { coordC.x, coordB.y } },
                    shape2D::Vertex{ { vertC.x, vertA.y }, tint, { coordC.x, coordA.y } },
                    shape2D::Vertex{ { vertB.x, vertA.y }, tint, { coordB.x, coordA.y } },
                    data);
            }

            // TOP-RIGHT QUAD
            ctx.DrawQuad(
                shape2D::Vertex{ { vertC.x, vertB.y }, tint, { coordC.x, coordB.y } },
                shape2D::Vertex{ { vertD.x, vertB.y }, tint, { coordD.x, coordB.y } },
                shape2D::Vertex{ { vertD.x, vertA.y }, tint, { coordD.x, coordA.y } },
                shape2D::Vertex{ { vertC.x, vertA.y }, tint, { coordC.x, coordA.y } },
                data);

            if (drawMiddle)
            {
                // MIDDLE-LEFT QUAD
                ctx.DrawQuad(
                    shape2D::Vertex{ { vertA.x, vertC.y }, tint, { coordA.x, coordC.y } },
                    shape2D::Vertex{ { vertB.x, vertC.y }, tint, { coordB.x, coordC.y } },
                    shape2D::Vertex{ { vertB.x, vertB.y }, tint, { coordB.x, coordB.y } },
                    shape2D::Vertex{ { vertA.x, vertB.y }, tint, { coordA.x, coordB.y } },
                    data);

                if (drawCenter)
                {
                    // MIDDLE-CENTER QUAD
                    ctx.DrawQuad(
                        shape2D::Vertex{ { vertB.x, vertC.y }, tint, { coordB.x, coordC.y } },
                        shape2D::Vertex{ { vertC.x, vertC.y }, tint, { coordC.x, coordC.y } },
                        shape2D::Vertex{ { vertC.x, vertB.y }, tint, { coordC.x, coordB.y } },
                        shape2D::Vertex{ { vertB.x, vertB.y }, tint, { coordB.x, coordB.y } },
                        data);
                }

                // MIDDLE-RIGHT QUAD
                ctx.DrawQuad(
                    shape2D::Vertex{ { vertC.x, vertC.y }, tint, { coordC.x, coordC.y } },
                    shape2D::Vertex{ { vertD.x, vertC.y }, tint, { coordD.x, coordC.y } },
                    shape2D::Vertex{ { vertD.x, vertB.y }, tint, { coordD.x, coordB.y } },
                    shape2D::Vertex{ { vertC.x, vertB.y }, tint, { coordC.x, coordB.y } },
                    data);
            }

            // BOTTOM-LEFT QUAD
            ctx.DrawQuad(
                shape2D::Vertex{ { vertA.x, vertD.y }, tint, { coordA.x, coordD.y } },
                shape2D::Vertex{ { vertB.x, vertD.y }, tint, { coordB.x, coordD.y } },
                shape2D::Vertex{ { vertB.x, vertC.y }, tint, { coordB.x, coordC.y } },
                shape2D::Vertex{ { vertA.x, vertC.y }, tint, { coordA.x, coordC.y } },
                data);

            if (drawCenter)
            {
                // BOTTOM-CENTER QUAD
                ctx.DrawQuad(
                    shape2D::Vertex{ { vertB.x, vertD.y }, tint, { coordB.x, coordD.y } },
                    shape2D::Vertex{ { vertC.x, vertD.y }, tint, { coordC.x, coordD.y } },
                    shape2D::Vertex{ { vertC.x, vertC.y }, tint, { coordC.x, coordC.y } },
                    shape2D::Vertex{ { vertB.x, vertC.y }, tint, { coordB.x, coordC.y } },
                    data);
            }

            // BOTTOM-RIGHT QUAD
            ctx.DrawQuad(
                shape2D::Vertex{ { vertC.x, vertD.y }, tint, { coordC.x, coordD.y } },
                shape2D::Vertex{ { vertD.x, vertD.y }, tint, { coordD.x, coordD.y } },
                shape2D::Vertex{ { vertD.x, vertC.y }, tint, { coordD.x, coordC.y } },
                shape2D::Vertex{ { vertC.x, vertC.y }, tint, { coordC.x, coordC.y } },
                data);
        }
        break;

        case gfx::NinePatchInfo::Layout::ThreePatchVertical:
        {
            // TOP QUAD
            ctx.DrawQuad(
                shape2D::Vertex{ { vertA.x, vertB.y }, tint, { coordA.x, coordB.y } },
                shape2D::Vertex{ { vertD.x, vertB.y }, tint, { coordD.x, coordB.y } },
                shape2D::Vertex{ { vertD.x, vertA.y }, tint, { coordD.x, coordA.y } },
                shape2D::Vertex{ { vertA.x, vertA.y }, tint, { coordA.x, coordA.y } },
                data);

            if (drawCenter)
            {
                // MIDDLE QUAD
                ctx.DrawQuad(
                    shape2D::Vertex{ { vertA.x, vertC.y }, tint, { coordA.x, coordC.y } },
                    shape2D::Vertex{ { vertD.x, vertC.y }, tint, { coordD.x, coordC.y } },
                    shape2D::Vertex{ { vertD.x, vertB.y }, tint, { coordD.x, coordB.y } },
                    shape2D::Vertex{ { vertA.x, vertB.y }, tint, { coordA.x, coordB.y } },
                    data);
            }

            // BOTTOM QUAD
            ctx.DrawQuad(
                shape2D::Vertex{ { vertA.x, vertD.y }, tint, { coordA.x, coordD.y } },
                shape2D::Vertex{ { vertD.x, vertD.y }, tint, { coordD.x, coordD.y } },
                shape2D::Vertex{ { vertD.x, vertC.y }, tint, { coordD.x, coordC.y } },
                shape2D::Vertex{ { vertA.x, vertC.y }, tint, { coordA.x, coordC.y } },
                data);
        }
        break;

        case gfx::NinePatchInfo::Layout::ThreePatchHorizontal:
        {
            // LEFT QUAD
            ctx.DrawQuad(
                shape2D::Vertex{ { vertA.x, vertD.y }, tint, { coordA.x, coordD.y } },
                shape2D::Vertex{ { vertB.x, vertD.y }, tint, { coordB.x, coordD.y } },
                shape2D::Vertex{ { vertB.x, vertA.y }, tint, { coordB.x, coordA.y } },
                shape2D::Vertex{ { vertA.x, vertA.y }, tint, { coordA.x, coordA.y } },
                data);

            if (drawCenter)
            {
                // CENTER QUAD
                ctx.DrawQuad(
                    shape2D::Vertex{ { coordB.x, coordD.y }, tint, { vertB.x, vertD.y } },
                    shape2D::Vertex{ { coordC.x, coordD.y }, tint, { vertC.x, vertD.y } },
                    shape2D::Vertex{ { coordC.x, coordA.y }, tint, { vertC.x, vertA.y } },
                    shape2D::Vertex{ { coordB.x, coordA.y }, tint, { vertB.x, vertA.y } },
                    data);
            }

            // RIGHT QUAD
            ctx.DrawQuad(
                shape2D::Vertex{ { coordC.x, coordD.y }, tint, { vertC.x, vertD.y } },
                shape2D::Vertex{ { coordD.x, coordD.y }, tint, { vertD.x, vertD.y } },
                shape2D::Vertex{ { coordD.x, coordA.y }, tint, { vertD.x, vertA.y } },
                shape2D::Vertex{ { coordC.x, coordA.y }, tint, { vertC.x, vertA.y } },
                data);
        }
        break;
    }
}

void _gfx_impl::Texture::Update(const void* pixels, int pitch, const shape2D::Rectangle& dest)
{
    if (SDL_UpdateTexture(data, dest != shape2D::Rectangle{} ? &dest : nullptr, pixels, pitch) < 0)
    {
        throw core::NexusException("gfx::Texture", "Unable to update texture.",
            "SDL", SDL_GetError());
    }
}

void _gfx_impl::Texture::Update(const gfx::Surface& surface, shape2D::Rectangle dest)
{
    if (dest == shape2D::Rectangle{})
    {
        dest.w = std::min(surface.GetWidth(), width);
        dest.h = std::min(surface.GetHeight(), height);
    }

    if (SDL_UpdateTexture(data, &dest, surface.GetPixels(), surface.GetPitch()) < 0)
    {
        throw core::NexusException("gfx::Texture", "Unable to update texture.",
            "SDL", SDL_GetError());
    }
}

gfx::PixelFormat _gfx_impl::Texture::GetFormat() const
{
    Uint32 format;
    SDL_QueryTexture(data, &format, nullptr, nullptr, nullptr);
    return static_cast<gfx::PixelFormat>(format);
}

gfx::TextureAccess _gfx_impl::Texture::GetAccess() const
{
    int access;
    SDL_QueryTexture(data, nullptr, &access, nullptr, nullptr);
    return static_cast<gfx::TextureAccess>(access);
}

gfx::BlendMode _gfx_impl::Texture::GetBlendMode() const
{
    SDL_BlendMode blendMode;

    if (SDL_GetTextureBlendMode(data, &blendMode) < 0)
    {
        throw core::NexusException("gfx::Texture", "Unable to get the BlendMode.",
            "SDL", SDL_GetError());
    }

    return static_cast<gfx::BlendMode>(blendMode);
}

void _gfx_impl::Texture::SetBlendMode(gfx::BlendMode blendMode)
{
    if (SDL_SetTextureBlendMode(data, static_cast<SDL_BlendMode>(blendMode)) < 0)
    {
        throw core::NexusException("gfx::Texture", "Unable to set the BlendMode.",
            "SDL", SDL_GetError());
    }
}

gfx::Color _gfx_impl::Texture::GetTint() const
{
    nexus::gfx::Color tint;
    SDL_GetTextureColorMod(data, &tint.r, &tint.g, &tint.b);
    SDL_GetTextureAlphaMod(data, &tint.a);
    return tint;
}

void _gfx_impl::Texture::SetTint(const nexus::gfx::Color& tint)
{
    SDL_SetTextureColorMod(data, tint.r, tint.g, tint.b);
    SDL_SetTextureAlphaMod(data, tint.a);
}
