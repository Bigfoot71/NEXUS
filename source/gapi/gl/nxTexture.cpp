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

#include "gapi/gl/nxTexture.hpp"
#include "core/nxException.hpp"
#include "gapi/gl/nxEnums.hpp"
#include "math/nxMath.hpp"

using namespace nexus;

gl::TextureFormat _gl_impl::ConvertPixelFormat(gfx::PixelFormat pf)
{
    switch (pf)
    {
        case gfx::PixelFormat::Index1LSB:   throw core::NexusException("gl::Texture", "gfx::PixelFormat::Index1LSB for gl::Texture not implemented yet.");
        case gfx::PixelFormat::Index1MSB:   throw core::NexusException("gl::Texture", "gfx::PixelFormat::Index1MSB for gl::Texture not implemented yet.");
        case gfx::PixelFormat::Index4LSB:   throw core::NexusException("gl::Texture", "gfx::PixelFormat::Index4LSB for gl::Texture not implemented yet.");
        case gfx::PixelFormat::Index4MSB:   throw core::NexusException("gl::Texture", "gfx::PixelFormat::Index4MSB for gl::Texture not implemented yet.");

        case gfx::PixelFormat::Index8:      return gl::TextureFormat::Grayscale;
        case gfx::PixelFormat::RGB332:      throw core::NexusException("gl::Texture", "gfx::PixelFormat::RGB332 for gl::Texture not implemented yet.");

        case gfx::PixelFormat::XRGB4444:    throw core::NexusException("gl::Texture", "gfx::PixelFormat::XRGB4444 for gl::Texture not implemented yet.");
        case gfx::PixelFormat::XBGR4444:    throw core::NexusException("gl::Texture", "gfx::PixelFormat::XBGR4444 for gl::Texture not implemented yet.");

        case gfx::PixelFormat::XRGB1555:    throw core::NexusException("gl::Texture", "gfx::PixelFormat::XRGB1555 for gl::Texture not implemented yet.");
        case gfx::PixelFormat::XBGR1555:    throw core::NexusException("gl::Texture", "gfx::PixelFormat::XBGR1555 for gl::Texture not implemented yet.");

        case gfx::PixelFormat::ARGB4444:    return gl::TextureFormat::RGBA4444;
        case gfx::PixelFormat::RGBA4444:    return gl::TextureFormat::RGBA4444;
        case gfx::PixelFormat::ABGR4444:    return gl::TextureFormat::RGBA4444;
        case gfx::PixelFormat::BGRA4444:    return gl::TextureFormat::RGBA4444;

        case gfx::PixelFormat::ARGB1555:    return gl::TextureFormat::RGBA5551;
        case gfx::PixelFormat::RGBA5551:    return gl::TextureFormat::RGBA5551;
        case gfx::PixelFormat::ABGR1555:    return gl::TextureFormat::RGBA5551;
        case gfx::PixelFormat::BGRA5551:    return gl::TextureFormat::RGBA5551;

        case gfx::PixelFormat::RGB565:      return gl::TextureFormat::RGB565;
        case gfx::PixelFormat::BGR565:      return gl::TextureFormat::RGB565;

        case gfx::PixelFormat::RGB24:       return gl::TextureFormat::RGB888;
        case gfx::PixelFormat::BGR24:       return gl::TextureFormat::RGB888;

        case gfx::PixelFormat::XRGB8888:    return gl::TextureFormat::RGB888;
        case gfx::PixelFormat::RGBX8888:    return gl::TextureFormat::RGB888;
        case gfx::PixelFormat::XBGR8888:    return gl::TextureFormat::RGB888;
        case gfx::PixelFormat::BGRX8888:    return gl::TextureFormat::RGB888;

        case gfx::PixelFormat::RGBA32:      return gl::TextureFormat::RGBA8888;
        case gfx::PixelFormat::ARGB32:      return gl::TextureFormat::RGBA8888;
        case gfx::PixelFormat::BGRA32:      return gl::TextureFormat::RGBA8888;
        case gfx::PixelFormat::ABGR32:      return gl::TextureFormat::RGBA8888;

        case gfx::PixelFormat::ARGB2101010: throw core::NexusException("gl::Texture", "gfx::PixelFormat::ARGB2101010 for gl::Texture not implemented yet.");

        case gfx::PixelFormat::YV12:        throw core::NexusException("gl::Texture", "gfx::PixelFormat::YV12 for gl::Texture not implemented yet.");
        case gfx::PixelFormat::IYUV:        throw core::NexusException("gl::Texture", "gfx::PixelFormat::IYUV for gl::Texture not implemented yet.");
        case gfx::PixelFormat::YUY2:        throw core::NexusException("gl::Texture", "gfx::PixelFormat::YUY2 for gl::Texture not implemented yet.");
        case gfx::PixelFormat::UYVY:        throw core::NexusException("gl::Texture", "gfx::PixelFormat::UYVY for gl::Texture not implemented yet.");
        case gfx::PixelFormat::YVYU:        throw core::NexusException("gl::Texture", "gfx::PixelFormat::YVYU for gl::Texture not implemented yet.");

        case gfx::PixelFormat::NV12:        throw core::NexusException("gl::Texture", "gfx::PixelFormat::NV12 for gl::Texture not implemented yet.");
        case gfx::PixelFormat::NV21:        throw core::NexusException("gl::Texture", "gfx::PixelFormat::NV21 for gl::Texture not implemented yet.");

        case gfx::PixelFormat::ExternalOES: throw core::NexusException("gl::Texture", "gfx::PixelFormat::ExternalOES for gl::Texture not implemented yet.");

        default: break;
    }

    return gl::TextureFormat(0);
}

/* Private Texture Implementation */

void _gl_impl::Texture::LoadFromMemory(const gfx::Surface& surface)
{
    if (surface.GetWidth() == 0 || surface.GetHeight() == 0)
    {
        throw core::NexusException("gl::Texture", "Data is not valid to load texture");
    }

    mipmaps = 1;
    width = surface.GetWidth();
    height = surface.GetHeight();
    format = _gl_impl::ConvertPixelFormat(surface.GetPixelFormat());
    id = ctx.LoadTexture(surface.GetPixels(), width, height, format, mipmaps);
}

void _gl_impl::Texture::Create(int w, int h, gl::TextureFormat format)
{
    width = w, height = h;
    mipmaps = 1, format = format;
    id = ctx.LoadTexture(nullptr, w, h, format, 1);
}

_gl_impl::Texture::Texture(gl::Context& ctx)
: gl::Contextual(ctx), id(ctx.GetTextureIdDefault()), width(1), height(1)
, mipmaps(1), format(gl::TextureFormat::RGBA8888)
{ }

_gl_impl::Texture::Texture(gl::Context& ctx, const std::string& filePath)
: gl::Contextual(ctx)
{
    LoadFromMemory(gfx::Surface(filePath));
}

_gl_impl::Texture::Texture(gl::Context& ctx, const gfx::Surface& surface)
: gl::Contextual(ctx)
{
    LoadFromMemory(surface);
}

_gl_impl::Texture::Texture(gl::Context& ctx, int w, int h, gl::TextureFormat format)
: gl::Contextual(ctx)
{
    Create(w, h, format);
}

_gl_impl::Texture::Texture(gl::Context& ctx, int w, int h, bool useRenderBuffer)
: Contextual(ctx), id(ctx.LoadTextureDepth(w, h, useRenderBuffer))
, width(w), height(h), mipmaps(1)
{ }

_gl_impl::Texture::Texture(gl::Context& ctx, const gfx::Surface& surface, CubemapLayout layout)
: Contextual(ctx), mipmaps(1)
{
    if (layout == CubemapLayout::AutoDetect)      // Try to automatically guess layout type
    {
        // Check image width/height to determine the type of cubemap provided
        if (surface.GetWidth() > surface.GetHeight())
        {
            if (surface.GetWidth() / 6 == surface.GetHeight())
            {
                layout = CubemapLayout::LineHorizontal;
                width = surface.GetWidth() / 6;
            }
            else if (surface.GetWidth() / 4 == surface.GetHeight() / 3)
            {
                layout = CubemapLayout::CrossFourByThree;
                width = surface.GetWidth() / 4;
            }
            else if (surface.GetWidth() >= static_cast<int>(surface.GetHeight() * 1.85f))
            {
                layout = CubemapLayout::Panorama;
                width = surface.GetWidth() / 4;
            }
        }
        else if (surface.GetHeight() > surface.GetWidth())
        {
            if ((surface.GetHeight() / 6) == surface.GetWidth())
            {
                layout = CubemapLayout::LineVertical;
                width = surface.GetHeight() / 6;
            }
            else if ((surface.GetWidth() / 3) == (surface.GetHeight() / 4))
            {
                layout = CubemapLayout::CrossThreeByFour;
                width = surface.GetWidth() / 3;
            }
        }
    }
    else
    {
        if (layout == CubemapLayout::LineVertical)      width = surface.GetHeight() / 6;
        if (layout == CubemapLayout::LineHorizontal)    width = surface.GetWidth()  / 6;
        if (layout == CubemapLayout::CrossThreeByFour)  width = surface.GetWidth()  / 3;
        if (layout == CubemapLayout::CrossFourByThree)  width = surface.GetWidth()  / 4;
        if (layout == CubemapLayout::Panorama)          width = surface.GetWidth()  / 4;
    }

    height = width;

    // Layout provided or already auto-detected
    if (layout != CubemapLayout::AutoDetect)
    {
        int size = width;
        gfx::Surface faces;
        std::array<shape2D::Rectangle, 6> faceRecs;
        faceRecs.fill({ 0, 0, size, size });

        if (layout == CubemapLayout::LineVertical)
        {
            faces = surface.Clone();     // Image data already follows expected convention
        }
        else if (layout == CubemapLayout::Panorama)
        {
            // TODO: Convert panorama image to square faces...
            // Ref: https://github.com/denivip/panorama/blob/master/panorama.cpp
        }
        else
        {
            if (layout == CubemapLayout::LineHorizontal)
            {
                for (int i = 0; i < 6; i++) faceRecs[i].x = i * size;
            }
            else if (layout == CubemapLayout::CrossThreeByFour)
            {
                faceRecs[0].SetPosition(size, size);
                faceRecs[1].SetPosition(size, 3 * size);
                faceRecs[2].SetPosition(size, 0);
                faceRecs[3].SetPosition(size, 2 * size);
                faceRecs[4].SetPosition(0, size);
                faceRecs[5].SetPosition(2 * size, size);
            }
            else if (layout == CubemapLayout::CrossFourByThree)
            {
                faceRecs[0].SetPosition(2 * size, size);
                faceRecs[1].SetPosition(0, size);
                faceRecs[2].SetPosition(size, 0);
                faceRecs[3].SetPosition(size, 2 * size);
                faceRecs[4].SetPosition(size, size);
                faceRecs[5].SetPosition(3 * size, size);
            }

            // Convert image data to 6 faces in a vertical column, that's the optimum layout for loading
            faces.Create(size, size * 6, surface.GetPixelFormat());
            faces.Fill(gfx::Magenta);

            for (int i = 0; i < 6; i++)
            {
                faces.DrawImage(surface, faceRecs[i], { 0, size*i, size, size });
            }
        }

        // NOTE: Cubemap data is expected to be provided as 6 images in a single data array,
        // one after the other (that's a vertical image), following convention: +X, -X, +Y, -Y, +Z, -Z
        id = ctx.LoadTextureCubemap(faces.GetPixels(), size, ConvertPixelFormat(faces.GetPixelFormat()));
        if (id == 0) NEXUS_LOG(Warning) << "[gl::Texture] Failed to load cubemap image\n";
    }
    else
    {
        NEXUS_LOG(Warning) << "[gl::Texture] Failed to detect cubemap image layout\n";
    }
}

_gl_impl::Texture::Texture(gl::Context& ctx, Uint32 id, int width, int height, int mipmaps, gl::TextureFormat format)
: Contextual(ctx), id(id), width(width), height(height), mipmaps(mipmaps), format(format)
{
    if (!glIsTexture(id))
    {
        throw core::NexusException("gl::Texture",
            "The texture ID given to the constructor is not a valid texture ID.");
    }
}

_gl_impl::Texture::~Texture()
{
    if (id > 0 && id != ctx.GetTextureIdDefault())
    {
        ctx.UnloadTexture(id);
    }
}

void _gl_impl::Texture::Update(const gfx::Surface& image)
{
    if (ConvertPixelFormat(image.GetPixelFormat()) != format)
    {
        throw core::NexusException("gl::Texture",
            "Unable to update the Texture, the PixelFormat of the given Surface must be identical to that of the Texture.");
    }

    this->Update(image.GetPixels());
}

void _gl_impl::Texture::Update(const void *pixels)
{
    ctx.UpdateTexture(id, 0, 0, width, height, format, pixels);
}

void _gl_impl::Texture::Update(const gfx::Surface& image, const shape2D::Rectangle& rect)
{
    if (ConvertPixelFormat(image.GetPixelFormat()) != format)
    {
        throw core::NexusException("gl::Texture",
            "Unable to update the Texture, the PixelFormat of the given Surface must be identical to that of the Texture.");
    }

    this->Update(image.GetPixels(), rect);
}

void _gl_impl::Texture::Update(const void *pixels, const shape2D::Rectangle& rect)
{
    ctx.UpdateTexture(id, rect.x, rect.y, rect.w, rect.h, format, pixels);
}

void _gl_impl::Texture::GenMipmaps()
{
    // NOTE: NPOT textures support check inside function
    // On WebGL (OpenGL ES 2.0) NPOT textures support is limited
    ctx.GenTextureMipmaps(id, width, height, format, mipmaps);
}

void _gl_impl::Texture::SetWrap(Wrap wrap)
{
    switch (wrap)
    {
        case Wrap::Repeat:
        {
            // NOTE: It only works if NPOT textures are supported, i.e. OpenGL ES 2.0 could not support it
            ctx.TextureParameters(id, gl::TextureParam::Wrap_S, gl::TextureWrap::Repeat);
            ctx.TextureParameters(id, gl::TextureParam::Wrap_T, gl::TextureWrap::Repeat);
        }
        break;

        case Wrap::Clamp:
        {
            ctx.TextureParameters(id, gl::TextureParam::Wrap_S, gl::TextureWrap::Clamp);
            ctx.TextureParameters(id, gl::TextureParam::Wrap_T, gl::TextureWrap::Clamp);
        }
        break;

        case Wrap::MirrorRepeat:
        {
            ctx.TextureParameters(id, gl::TextureParam::Wrap_S, gl::TextureWrap::MirrorRepeat);
            ctx.TextureParameters(id, gl::TextureParam::Wrap_T, gl::TextureWrap::MirrorRepeat);
        }
        break;

        case Wrap::MirrorClamp:
        {
            ctx.TextureParameters(id, gl::TextureParam::Wrap_S, gl::TextureWrap::MirrorClamp);
            ctx.TextureParameters(id, gl::TextureParam::Wrap_T, gl::TextureWrap::MirrorClamp);
        }
        break;

        default: break;
    }
}

void _gl_impl::Texture::SetFilter(Filter filter)
{
    switch (filter)
    {
        case Filter::Point:
        {
            if (mipmaps > 1)
            {
                // TextureFilter::MipNearest - tex filter: POINT, mipmaps filter: POINT (sharp switching between mipmaps)
                ctx.TextureParameters(id, gl::TextureParam::MinFilter, gl::TextureFilter::MipNearest);

                // TextureFilter::Nearest - tex filter: POINT (no filter), no mipmaps
                ctx.TextureParameters(id, gl::TextureParam::MagFilter, gl::TextureFilter::Nearest);
            }
            else
            {
                // TextureFilter::Nearest - tex filter: POINT (no filter), no mipmaps
                ctx.TextureParameters(id, gl::TextureParam::MinFilter, gl::TextureFilter::Nearest);
                ctx.TextureParameters(id, gl::TextureParam::MagFilter, gl::TextureFilter::Nearest);
            }
        }
        break;

        case Filter::Bilinear:
        {
            if (mipmaps > 1)
            {
                // TextureFilter::LinearMipNearest - tex filter: BILINEAR, mipmaps filter: POINT (sharp switching between mipmaps)
                // Alternative: TextureFilter::NearestMipLinear - tex filter: POINT, mipmaps filter: BILINEAR (smooth transition between mipmaps)
                ctx.TextureParameters(id, gl::TextureParam::MinFilter, gl::TextureFilter::LinearMipNearest);

                // TextureFilter::Linear - tex filter: BILINEAR, no mipmaps
                ctx.TextureParameters(id, gl::TextureParam::MagFilter, gl::TextureFilter::Linear);
            }
            else
            {
                // gl::TextureFilter::Linear - tex filter: BILINEAR, no mipmaps
                ctx.TextureParameters(id, gl::TextureParam::MinFilter, gl::TextureFilter::Linear);
                ctx.TextureParameters(id, gl::TextureParam::MagFilter, gl::TextureFilter::Linear);
            }
        }
        break;

        case Filter::Trilinear:
        {
            if (mipmaps > 1)
            {
                // TextureFilter::MipLinear - tex filter: BILINEAR, mipmaps filter: BILINEAR (smooth transition between mipmaps)
                ctx.TextureParameters(id, gl::TextureParam::MinFilter, gl::TextureFilter::MipLinear);

                // TextureFilter::Linear - tex filter: BILINEAR, no mipmaps
                ctx.TextureParameters(id, gl::TextureParam::MagFilter, gl::TextureFilter::Linear);
            }
            else
            {
                NEXUS_LOG(Warning) << "[TEXTURE] [ID " << id << "] No mipmaps available for TRILINEAR texture filtering\n";

                // TextureFilter::Linear - tex filter: BILINEAR, no mipmaps
                ctx.TextureParameters(id, gl::TextureParam::MinFilter, gl::TextureFilter::Linear);
                ctx.TextureParameters(id, gl::TextureParam::MagFilter, gl::TextureFilter::Linear);
            }
        }
        break;

        case Filter::Anisotropic_4X: ctx.TextureParameters(id, gl::TextureParam::Anisotropy, 4); break;
        case Filter::Anisotropic_8X: ctx.TextureParameters(id, gl::TextureParam::Anisotropy, 8); break;
        case Filter::Anisotropic_16X: ctx.TextureParameters(id, gl::TextureParam::Anisotropy, 16); break;

        default: break;
    }
}

void _gl_impl::Texture::Draw(shape2D::RectangleF src, const shape2D::RectangleF& dst, const math::Vec2& origin, float rotation, const gfx::Color& tint)  const
{
    bool flipX = false;

    if (src.w < 0) flipX = true, src.w *= -1;
    if (src.h < 0) src.y -= src.h;

    math::Vec2 topLeft, topRight;
    math::Vec2 bottomLeft, bottomRight;

    // Only calculate rotation if needed
    if (rotation == 0.0f)
    {
        float x = dst.x - origin.x;
        float y = dst.y - origin.y;
        topLeft = { x, y };
        topRight = { x + dst.w, y };
        bottomLeft = { x, y + dst.h };
        bottomRight = { x + dst.w, y + dst.h };
    }
    else
    {
        rotation *= math::Deg2Rad;

        float sinRotation = std::sin(rotation);
        float cosRotation = std::cos(rotation);
        float x = dst.x;
        float y = dst.y;
        float dx = -origin.x;
        float dy = -origin.y;

        topLeft.x = x + dx*cosRotation - dy*sinRotation;
        topLeft.y = y + dx*sinRotation + dy*cosRotation;

        topRight.x = x + (dx + dst.w)*cosRotation - dy*sinRotation;
        topRight.y = y + (dx + dst.w)*sinRotation + dy*cosRotation;

        bottomLeft.x = x + dx*cosRotation - (dy + dst.h)*sinRotation;
        bottomLeft.y = y + dx*sinRotation + (dy + dst.h)*cosRotation;

        bottomRight.x = x + (dx + dst.w)*cosRotation - (dy + dst.h)*sinRotation;
        bottomRight.y = y + (dx + dst.w)*sinRotation + (dy + dst.h)*cosRotation;
    }

    ctx.SetTexture(id);
    ctx.Begin(gl::DrawMode::Quads);

        ctx.Color(tint.r, tint.g, tint.b, tint.a);
        ctx.Normal(0.0f, 0.0f, 1.0f);   // Normal vector pointing towards viewer

        // Top-left corner for texture and quad
        if (flipX) ctx.TexCoord((src.x + src.w)/width, src.y/height);
        else ctx.TexCoord(src.x/width, src.y/height);
        ctx.Vertex(topLeft.x, topLeft.y);

        // Bottom-left corner for texture and quad
        if (flipX) ctx.TexCoord((src.x + src.w)/width, (src.y + src.h)/height);
        else ctx.TexCoord(src.x/width, (src.y + src.h)/height);
        ctx.Vertex(bottomLeft.x, bottomLeft.y);

        // Bottom-right corner for texture and quad
        if (flipX) ctx.TexCoord(src.x/width, (src.y + src.h)/height);
        else ctx.TexCoord((src.x + src.w)/width, (src.y + src.h)/height);
        ctx.Vertex(bottomRight.x, bottomRight.y);

        // Top-right corner for texture and quad
        if (flipX) ctx.TexCoord(src.x/width, src.y/height);
        else ctx.TexCoord((src.x + src.w)/width, src.y/height);
        ctx.Vertex(topRight.x, topRight.y);

    ctx.End();
    ctx.SetTexture(0u);
}

void _gl_impl::Texture::Draw(float x, float y, const gfx::Color& tint) const
{
    Draw({ 0, 0, static_cast<float>(width), static_cast<float>(height) },
            { x, y, static_cast<float>(width), static_cast<float>(height) },
            { 0, 0 }, 0.0f, tint);
}

void _gl_impl::Texture::Draw(const math::Vec2& position, const gfx::Color& tint) const
{
    Draw({ 0, 0, static_cast<float>(width), static_cast<float>(height) },
            { position.x, position.y, static_cast<float>(width), static_cast<float>(height) },
            { 0, 0 }, 0.0f, tint);
}

void _gl_impl::Texture::Draw(const math::Vec2& position, float rotation, float scale, const gfx::Color& tint) const
{
    Draw({ 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height) },
            { position.x, position.y, width*scale, width*scale },
            { width/2.0f * scale, height/2.0f * scale },
            rotation, tint);
}

void _gl_impl::Texture::Draw(const math::Vec2& position, float rotation, const math::Vec2& origin, const math::Vec2& scale, const gfx::Color& tint) const
{
    Draw({ 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height) },
            { position.x, position.y, width*scale.x, height*scale.y },
            origin, rotation, tint);
}

void _gl_impl::Texture::Draw(const shape2D::RectangleF& src, const math::Vec2& position, const gfx::Color& tint) const
{
    Draw({ 0, 0, static_cast<float>(width), static_cast<float>(height) }, { position.x, position.y, static_cast<float>(width), static_cast<float>(height) }, { 0, 0 }, 0, tint);
}

void _gl_impl::Texture::DrawNinePatch(gl::NinePatchInfo ninePatchInfo, const shape2D::RectangleF& dest, const math::Vec2& origin, float rotation, const gfx::Color& tint)
{
    float patchWidth = std::max(dest.w, 0.0f);
    float patchHeight = std::max(dest.h, 0.0f);

    if (ninePatchInfo.source.w < 0) ninePatchInfo.source.x -= ninePatchInfo.source.w;
    if (ninePatchInfo.source.h < 0) ninePatchInfo.source.y -= ninePatchInfo.source.h;

    switch (ninePatchInfo.layout)
    {
        case gl::NinePatchInfo::Layout::ThreePatchHorizontal:
            patchHeight = ninePatchInfo.source.h;
            break;

        case gl::NinePatchInfo::Layout::ThreePatchVertical:
            patchWidth = ninePatchInfo.source.w;
            break;

        default:
            break;
    }

    bool drawCenter = true;
    bool drawMiddle = true;
    shape2D::AABB bounds = ninePatchInfo.bounds;

    // Adjust the lateral (left and right) border widths in case patchWidth < texture.width
    if (ninePatchInfo.layout != gl::NinePatchInfo::Layout::ThreePatchVertical && patchWidth <= bounds.min.x + bounds.max.x)
    {
        drawCenter = false;
        bounds.min.x = bounds.min.x / (bounds.min.x + bounds.max.x) * patchWidth;
        bounds.max.x = patchWidth - bounds.min.x;
    }

    // Adjust the lateral (top and bottom) border heights in case patchHeight < texture.height
    if (ninePatchInfo.layout != gl::NinePatchInfo::Layout::ThreePatchHorizontal && patchHeight <= bounds.min.y + bounds.max.y)
    {
        drawMiddle = false;
        bounds.min.y = (bounds.min.y/(bounds.min.y + bounds.max.y))*patchHeight;
        bounds.max.y = patchHeight - bounds.min.y;
    }

    math::Vec2 vertA, vertB, vertC, vertD;
    vertA.x = 0.0f;                             // outer left
    vertA.y = 0.0f;                             // outer top
    vertB.x = bounds.min.x;                     // inner left
    vertB.y = bounds.min.y;                     // inner top
    vertC.x = patchWidth  - bounds.max.x;       // inner right
    vertC.y = patchHeight - bounds.max.y;       // inner bottom
    vertD.x = patchWidth;                       // outer right
    vertD.y = patchHeight;                      // outer bottom

    math::Vec2 coordA, coordB, coordC, coordD;
    coordA.x = static_cast<float>(ninePatchInfo.source.x) / width;
    coordA.y = static_cast<float>(ninePatchInfo.source.y) / height;
    coordB.x = static_cast<float>(ninePatchInfo.source.x + bounds.min.x) / width;
    coordB.y = static_cast<float>(ninePatchInfo.source.y + bounds.min.y) / height;
    coordC.x = static_cast<float>(ninePatchInfo.source.x + ninePatchInfo.source.w  - bounds.max.x) / width;
    coordC.y = static_cast<float>(ninePatchInfo.source.y + ninePatchInfo.source.h - bounds.max.y) / height;
    coordD.x = static_cast<float>(ninePatchInfo.source.x + ninePatchInfo.source.w) / width;
    coordD.y = static_cast<float>(ninePatchInfo.source.y + ninePatchInfo.source.h) / height;

    ctx.SetTexture(id);
    ctx.PushMatrix();

        ctx.Translate(dest.x, dest.y, 0.0f);
        ctx.Rotate(rotation, 0.0f, 0.0f, 1.0f);
        ctx.Translate(-origin.x, -origin.y, 0.0f);

        ctx.Begin(gl::DrawMode::Quads);

            ctx.Color(tint);

            switch (ninePatchInfo.layout)
            {
                case gl::NinePatchInfo::Layout::NinePatch:
                {
                    // TOP-LEFT QUAD
                    ctx.TexCoord(coordA.x, coordB.y); ctx.Vertex(vertA.x, vertB.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordB.x, coordB.y); ctx.Vertex(vertB.x, vertB.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordB.x, coordA.y); ctx.Vertex(vertB.x, vertA.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordA.x, coordA.y); ctx.Vertex(vertA.x, vertA.y);  // Top-left corner for texture and quad

                    if (drawCenter)
                    {
                        // TOP-CENTER QUAD
                        ctx.TexCoord(coordB.x, coordB.y); ctx.Vertex(vertB.x, vertB.y);  // Bottom-left corner for texture and quad
                        ctx.TexCoord(coordC.x, coordB.y); ctx.Vertex(vertC.x, vertB.y);  // Bottom-right corner for texture and quad
                        ctx.TexCoord(coordC.x, coordA.y); ctx.Vertex(vertC.x, vertA.y);  // Top-right corner for texture and quad
                        ctx.TexCoord(coordB.x, coordA.y); ctx.Vertex(vertB.x, vertA.y);  // Top-left corner for texture and quad
                    }

                    // TOP-RIGHT QUAD
                    ctx.TexCoord(coordC.x, coordB.y); ctx.Vertex(vertC.x, vertB.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordD.x, coordB.y); ctx.Vertex(vertD.x, vertB.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordD.x, coordA.y); ctx.Vertex(vertD.x, vertA.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordC.x, coordA.y); ctx.Vertex(vertC.x, vertA.y);  // Top-left corner for texture and quad

                    if (drawMiddle)
                    {
                        // MIDDLE-LEFT QUAD
                        ctx.TexCoord(coordA.x, coordC.y); ctx.Vertex(vertA.x, vertC.y);  // Bottom-left corner for texture and quad
                        ctx.TexCoord(coordB.x, coordC.y); ctx.Vertex(vertB.x, vertC.y);  // Bottom-right corner for texture and quad
                        ctx.TexCoord(coordB.x, coordB.y); ctx.Vertex(vertB.x, vertB.y);  // Top-right corner for texture and quad
                        ctx.TexCoord(coordA.x, coordB.y); ctx.Vertex(vertA.x, vertB.y);  // Top-left corner for texture and quad

                        if (drawCenter)
                        {
                            // MIDDLE-CENTER QUAD
                            ctx.TexCoord(coordB.x, coordC.y); ctx.Vertex(vertB.x, vertC.y);  // Bottom-left corner for texture and quad
                            ctx.TexCoord(coordC.x, coordC.y); ctx.Vertex(vertC.x, vertC.y);  // Bottom-right corner for texture and quad
                            ctx.TexCoord(coordC.x, coordB.y); ctx.Vertex(vertC.x, vertB.y);  // Top-right corner for texture and quad
                            ctx.TexCoord(coordB.x, coordB.y); ctx.Vertex(vertB.x, vertB.y);  // Top-left corner for texture and quad
                        }

                        // MIDDLE-RIGHT QUAD
                        ctx.TexCoord(coordC.x, coordC.y); ctx.Vertex(vertC.x, vertC.y);  // Bottom-left corner for texture and quad
                        ctx.TexCoord(coordD.x, coordC.y); ctx.Vertex(vertD.x, vertC.y);  // Bottom-right corner for texture and quad
                        ctx.TexCoord(coordD.x, coordB.y); ctx.Vertex(vertD.x, vertB.y);  // Top-right corner for texture and quad
                        ctx.TexCoord(coordC.x, coordB.y); ctx.Vertex(vertC.x, vertB.y);  // Top-left corner for texture and quad
                    }

                    // BOTTOM-LEFT QUAD
                    ctx.TexCoord(coordA.x, coordD.y); ctx.Vertex(vertA.x, vertD.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordB.x, coordD.y); ctx.Vertex(vertB.x, vertD.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordB.x, coordC.y); ctx.Vertex(vertB.x, vertC.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordA.x, coordC.y); ctx.Vertex(vertA.x, vertC.y);  // Top-left corner for texture and quad

                    if (drawCenter)
                    {
                        // BOTTOM-CENTER QUAD
                        ctx.TexCoord(coordB.x, coordD.y); ctx.Vertex(vertB.x, vertD.y);  // Bottom-left corner for texture and quad
                        ctx.TexCoord(coordC.x, coordD.y); ctx.Vertex(vertC.x, vertD.y);  // Bottom-right corner for texture and quad
                        ctx.TexCoord(coordC.x, coordC.y); ctx.Vertex(vertC.x, vertC.y);  // Top-right corner for texture and quad
                        ctx.TexCoord(coordB.x, coordC.y); ctx.Vertex(vertB.x, vertC.y);  // Top-left corner for texture and quad
                    }

                    // BOTTOM-RIGHT QUAD
                    ctx.TexCoord(coordC.x, coordD.y); ctx.Vertex(vertC.x, vertD.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordD.x, coordD.y); ctx.Vertex(vertD.x, vertD.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordD.x, coordC.y); ctx.Vertex(vertD.x, vertC.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordC.x, coordC.y); ctx.Vertex(vertC.x, vertC.y);  // Top-left corner for texture and quad

                }
                break;

                case gl::NinePatchInfo::Layout::ThreePatchVertical:
                {
                    // TOP QUAD
                    ctx.TexCoord(coordA.x, coordB.y); ctx.Vertex(vertA.x, vertB.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordD.x, coordB.y); ctx.Vertex(vertD.x, vertB.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordD.x, coordA.y); ctx.Vertex(vertD.x, vertA.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordA.x, coordA.y); ctx.Vertex(vertA.x, vertA.y);  // Top-left corner for texture and quad

                    if (drawCenter)
                    {
                        // MIDDLE QUAD
                        ctx.TexCoord(coordA.x, coordC.y); ctx.Vertex(vertA.x, vertC.y);  // Bottom-left corner for texture and quad
                        ctx.TexCoord(coordD.x, coordC.y); ctx.Vertex(vertD.x, vertC.y);  // Bottom-right corner for texture and quad
                        ctx.TexCoord(coordD.x, coordB.y); ctx.Vertex(vertD.x, vertB.y);  // Top-right corner for texture and quad
                        ctx.TexCoord(coordA.x, coordB.y); ctx.Vertex(vertA.x, vertB.y);  // Top-left corner for texture and quad
                    }

                    // BOTTOM QUAD
                    ctx.TexCoord(coordA.x, coordD.y); ctx.Vertex(vertA.x, vertD.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordD.x, coordD.y); ctx.Vertex(vertD.x, vertD.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordD.x, coordC.y); ctx.Vertex(vertD.x, vertC.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordA.x, coordC.y); ctx.Vertex(vertA.x, vertC.y);  // Top-left corner for texture and quad
                }
                break;

                case gl::NinePatchInfo::Layout::ThreePatchHorizontal:
                {
                    // LEFT QUAD
                    ctx.TexCoord(coordA.x, coordD.y); ctx.Vertex(vertA.x, vertD.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordB.x, coordD.y); ctx.Vertex(vertB.x, vertD.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordB.x, coordA.y); ctx.Vertex(vertB.x, vertA.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordA.x, coordA.y); ctx.Vertex(vertA.x, vertA.y);  // Top-left corner for texture and quad

                    if (drawCenter)
                    {
                        // CENTER QUAD
                        ctx.TexCoord(coordB.x, coordD.y); ctx.Vertex(vertB.x, vertD.y);  // Bottom-left corner for texture and quad
                        ctx.TexCoord(coordC.x, coordD.y); ctx.Vertex(vertC.x, vertD.y);  // Bottom-right corner for texture and quad
                        ctx.TexCoord(coordC.x, coordA.y); ctx.Vertex(vertC.x, vertA.y);  // Top-right corner for texture and quad
                        ctx.TexCoord(coordB.x, coordA.y); ctx.Vertex(vertB.x, vertA.y);  // Top-left corner for texture and quad
                    }

                    // RIGHT QUAD
                    ctx.TexCoord(coordC.x, coordD.y); ctx.Vertex(vertC.x, vertD.y);  // Bottom-left corner for texture and quad
                    ctx.TexCoord(coordD.x, coordD.y); ctx.Vertex(vertD.x, vertD.y);  // Bottom-right corner for texture and quad
                    ctx.TexCoord(coordD.x, coordA.y); ctx.Vertex(vertD.x, vertA.y);  // Top-right corner for texture and quad
                    ctx.TexCoord(coordC.x, coordA.y); ctx.Vertex(vertC.x, vertA.y);  // Top-left corner for texture and quad
                }
                break;
            }

        ctx.End();

    ctx.PopMatrix();
    ctx.SetTexture(0u);
}

void _gl_impl::Texture::DrawBillboard(const gl::Camera3D& camera, shape2D::Rectangle source, const math::Vec3& position, const math::Vec3& up, const math::Vec2& size, const math::Vec2& origin, float rotation, const gfx::Color& tint) const
{
    bool flipX = source.w < 0;
    bool flipY = source.h < 0;

    source.w = std::abs(source.w);
    source.h = std::abs(source.h);

    // NOTE: Billboard size will maintain source rectangle aspect ratio, size will represent billboard width
    math::Vec2 sizeRatio(size.x * static_cast<float>(source.w) / source.h, size.y);

    math::Mat4 matView = math::Mat4::LookAt(camera.position, camera.target, camera.up);

    math::Vec3 right(matView.m[0], matView.m[4], matView.m[8]);
    //math::Vec3 up = { matView.m[1], matView.m[5], matView.m[9] };

    math::Vec3 rightScaled = right * (sizeRatio.x / 2);
    math::Vec3 upScaled = up * (sizeRatio.y / 2);

    math::Vec3 p1 = rightScaled + upScaled;
    math::Vec3 p2 = rightScaled - upScaled;

    math::Vec3 topLeft = -p2;
    math::Vec3 topRight = p1;
    math::Vec3 bottomRight = p2;
    math::Vec3 bottomLeft = -p1;

    if (rotation != 0.0f)
    {
        rotation *= math::Deg2Rad;

        float sinRotation = std::sin(rotation);
        float cosRotation = std::cos(rotation);

        // NOTE: (-1, 1) is the range where origin.x, origin.y is inside the texture
        float rotateAboutX = sizeRatio.x * origin.x / 2;
        float rotateAboutY = sizeRatio.y * origin.y / 2;

        float xtvalue, ytvalue;
        float rotatedX, rotatedY;

        xtvalue = right.Dot(topLeft) - rotateAboutX;                            // Project points to x and y coordinates on the billboard plane
        ytvalue = up.Dot(topLeft) - rotateAboutY;
        rotatedX = xtvalue * cosRotation - ytvalue * sinRotation + rotateAboutX;    // Rotate about the point origin
        rotatedY = xtvalue * sinRotation + ytvalue * cosRotation + rotateAboutY;
        topLeft = (up * rotatedY) + (right * rotatedX);                             // Translate back to cartesian coordinates

        xtvalue = right.Dot(topRight) - rotateAboutX;
        ytvalue = up.Dot(topRight) - rotateAboutY;
        rotatedX = xtvalue * cosRotation - ytvalue * sinRotation + rotateAboutX;
        rotatedY = xtvalue * sinRotation + ytvalue * cosRotation + rotateAboutY;
        topRight = (up * rotatedY) + (right * rotatedX);

        xtvalue = right.Dot(bottomRight) - rotateAboutX;
        ytvalue = up.Dot(bottomRight) - rotateAboutY;
        rotatedX = xtvalue * cosRotation - ytvalue * sinRotation + rotateAboutX;
        rotatedY = xtvalue * sinRotation + ytvalue * cosRotation + rotateAboutY;
        bottomRight = (up * rotatedY) + (right * rotatedX);

        xtvalue = right.Dot( bottomLeft) - rotateAboutX;
        ytvalue = up.Dot(bottomLeft) - rotateAboutY;
        rotatedX = xtvalue * cosRotation - ytvalue * sinRotation + rotateAboutX;
        rotatedY = xtvalue * sinRotation + ytvalue * cosRotation + rotateAboutY;
        bottomLeft = (up * rotatedY) + (right * rotatedX);
    }

    // Translate points to the draw center (position)
    topLeft = topLeft + position;
    topRight = topRight + position;
    bottomRight = bottomRight + position;
    bottomLeft = bottomLeft + position;

    ctx.SetTexture(id);
    ctx.Begin(gl::DrawMode::Quads);

        ctx.Color(tint.r, tint.g, tint.b, tint.a);

        // Bottom-left corner for texture and quad
        float u = flipX ? static_cast<float>(source.x + source.w) / width : static_cast<float>(source.x) / width;
        float v = flipY ? static_cast<float>(source.y + source.h) / height : static_cast<float>(source.y) / height;
        ctx.TexCoord(u,v);
        ctx.Vertex(topLeft.x, topLeft.y, topLeft.z);

        // Top-left corner for texture and quad
        u = flipX ? static_cast<float>(source.x + source.w) / width : static_cast<float>(source.x) / width;
        v = !flipY ? static_cast<float>(source.y + source.h) / height : static_cast<float>(source.y) / height;
        ctx.TexCoord(u,v);
        ctx.Vertex(bottomLeft.x, bottomLeft.y, bottomLeft.z);

        // Top-right corner for texture and quad
        u = !flipX ? static_cast<float>(source.x + source.w) / width : static_cast<float>(source.x) / width;
        v = !flipY ? static_cast<float>(source.y + source.h) / height : static_cast<float>(source.y) / height;
        ctx.TexCoord(u, v);
        ctx.Vertex(bottomRight.x, bottomRight.y, bottomRight.z);

        // Bottom-right corner for texture and quad
        u = !flipX ? static_cast<float>(source.x + source.w) / width : static_cast<float>(source.x) / width;
        v = flipY ? static_cast<float>(source.y + source.h) / height : static_cast<float>(source.y) / height;
        ctx.TexCoord(u, v);
        ctx.Vertex(topRight.x, topRight.y, topRight.z);

    ctx.End();
    ctx.SetTexture(0u);
}

void _gl_impl::Texture::DrawBillboard(const gl::Camera3D& camera, const math::Vec3& position, float size, float rotation, const gfx::Color& tint) const
{
    DrawBillboard(camera, { 0, 0, width, height }, position, { size, size }, rotation, tint);
}

void _gl_impl::Texture::DrawBillboard(const gl::Camera3D& camera, const shape2D::Rectangle& source, const math::Vec3& position, const math::Vec2& size, float rotation, const gfx::Color& tint) const
{
    DrawBillboard(camera, source, position, { 0.0f, 1.0f, 0.0f }, size, {}, rotation, tint);
}
