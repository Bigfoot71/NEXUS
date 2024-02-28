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

#include "gfx/nxSurface.hpp"

#include "SDL_image.h"
#include "core/nxFileFormat.hpp"
#include "shape/2D/nxRectangle.hpp"
#include "shape/2D/nxTriangle.hpp"
#include "shape/2D/nxPolygon.hpp"
#include "shape/2D/nxCircle.hpp"
#include "core/nxException.hpp"
#include "shape/2D/nxLine.hpp"
#include "shape/2D/nxAABB.hpp"
#include "core/nxRandom.hpp"
#include "gfx/nxPixel.hpp"
#include "math/nxVec2.hpp"
#include "math/nxMath.hpp"

#include <SDL_surface.h>
#include <SDL_pixels.h>
#include <SDL_stdinc.h>
#include <limits>
#include <cmath>

using namespace nexus;

/* Public Static Implementation Surface */

gfx::Surface gfx::Surface::New(int width, int height, const gfx::Color& color)
{
    return gfx::Surface(width, height, color);
}

gfx::Surface gfx::Surface::NewGradientLinear(int width, int height, float direction, const gfx::Color& start, const gfx::Color& end)
{
    gfx::Surface surface;
    surface.Create(width, height);
    surface.DrawGradientLinear({ 0, 0, width, height }, direction, start, end);
    return surface;
}

gfx::Surface gfx::Surface::NewGradientRadial(int width, int height, float density, const gfx::Color& inner, const gfx::Color&outer)
{
    gfx::Surface surface;
    surface.Create(width, height);
    surface.DrawGradientRadial({ 0, 0, width, height }, density, inner, outer);
    return surface;
}

gfx::Surface gfx::Surface::NewGradientSquare(int width, int height, float density, const gfx::Color& inner, const gfx::Color& outer)
{
    gfx::Surface surface;
    surface.Create(width, height);
    surface.DrawGradientSquare({ 0, 0, width, height }, density, inner, outer);
    return surface;
}

gfx::Surface gfx::Surface::NewChecked(int width, int height, int checksX, int checksY, const gfx::Color& col1, const gfx::Color& col2)
{
    gfx::Surface surface;
    surface.Create(width, height);
    surface.DrawChecked({ 0, 0, width, height }, checksX, checksY, col1, col2);
    return surface;
}

gfx::Surface gfx::Surface::NewWhiteNoise(int width, int height, float factor)
{
    gfx::Surface surface;
    surface.Create(width, height);
    surface.DrawWhiteNoise({ 0, 0, width, height }, factor);
    return surface;
}

gfx::Surface gfx::Surface::NewCellular(int width, int height, int tileSize)
{
    gfx::Surface surface;
    surface.Create(width, height);
    surface.DrawCellular({ 0, 0, width, height }, tileSize);
    return surface;
}

/* Public Implementation Surface */

void gfx::Surface::Create(int width, int height, PixelFormat format)
{
    SDL_Surface *temp = SDL_CreateRGBSurfaceWithFormat(0, width, height,
        PixelInfo::CalculateDepth(format), static_cast<int>(format));

    if (temp == nullptr)
    {
        throw core::NexusException(
            "Surface", "Error creating the Surface.",
            "SDL", SDL_GetError());
    }

    Destroy(); surface = temp;
}

void gfx::Surface::Load(const std::string& filePath)
{
    SDL_Surface *temp = IMG_Load(filePath.c_str());

    if (temp == nullptr)
    {
        throw core::NexusException(
            "Surface", "Error loading the image.",
            "SDL_image", IMG_GetError());
    }

    Destroy(); surface = temp;
}

void gfx::Surface::LoadFromMemory(const std::vector<Uint8>& data)
{
    SDL_Surface *temp = IMG_Load_RW(SDL_RWFromConstMem(data.data(), data.size()), 1);

    if (temp == nullptr) 
    {
        core::NexusException("Surface", "The image could not be loaded from memory",
            "SDL_image", IMG_GetError());
    }

    Destroy(); surface = temp;
}

void gfx::Surface::LoadFromMemory(const std::vector<Uint8>& data, core::ImageFormat format)
{
    SDL_RWops *rw = SDL_RWFromConstMem(data.data(), data.size());

    SDL_Surface *temp = nullptr;

    switch (format)
    {
        case core::ImageFormat::BMP:    temp = IMG_LoadBMP_RW(rw);          break;
        case core::ImageFormat::GIF:    temp = IMG_LoadGIF_RW(rw);          break;
        case core::ImageFormat::JPG:    temp = IMG_LoadJPG_RW(rw);          break;
        case core::ImageFormat::LBM:    temp = IMG_LoadLBM_RW(rw);          break;
        case core::ImageFormat::PCX:    temp = IMG_LoadPCX_RW(rw);          break;
        case core::ImageFormat::PNG:    temp = IMG_LoadPNG_RW(rw);          break;
        case core::ImageFormat::PNM:    temp = IMG_LoadPNM_RW(rw);          break;
        case core::ImageFormat::QOI:    temp = IMG_LoadQOI_RW(rw);          break;
        case core::ImageFormat::TGA:    temp = IMG_LoadTGA_RW(rw);          break;
        case core::ImageFormat::XCF:    temp = IMG_LoadXCF_RW(rw);          break;
        case core::ImageFormat::XPM:    temp = IMG_LoadXPM_RW(rw);          break;
        case core::ImageFormat::SVG:    temp = IMG_LoadSVG_RW(rw);          break;
        default:                        temp = IMG_Load_RW(rw, 0);          break;
    }

    SDL_RWclose(rw);

    if (temp == nullptr) 
    {
        core::NexusException("Surface", "The image could not be loaded from memory",
            "SDL_image", IMG_GetError());
    }

    Destroy(); surface = temp;
}

void gfx::Surface::LoadFromMemory(const void* data, size_t size)
{
    SDL_Surface *temp = IMG_Load_RW(SDL_RWFromConstMem(data, size), 1);

    if (temp == nullptr) 
    {
        core::NexusException("Surface", "The image could not be loaded from memory",
            "SDL_image", IMG_GetError());
    }

    Destroy(); surface = temp;
}

void gfx::Surface::LoadFromMemory(const void* data, size_t size, core::ImageFormat format)
{
    SDL_RWops *rw = SDL_RWFromConstMem(data, size);

    SDL_Surface *temp = nullptr;

    switch (format)
    {
        case core::ImageFormat::BMP:    temp = IMG_LoadBMP_RW(rw);          break;
        case core::ImageFormat::GIF:    temp = IMG_LoadGIF_RW(rw);          break;
        case core::ImageFormat::JPG:    temp = IMG_LoadJPG_RW(rw);          break;
        case core::ImageFormat::LBM:    temp = IMG_LoadLBM_RW(rw);          break;
        case core::ImageFormat::PCX:    temp = IMG_LoadPCX_RW(rw);          break;
        case core::ImageFormat::PNG:    temp = IMG_LoadPNG_RW(rw);          break;
        case core::ImageFormat::PNM:    temp = IMG_LoadPNM_RW(rw);          break;
        case core::ImageFormat::QOI:    temp = IMG_LoadQOI_RW(rw);          break;
        case core::ImageFormat::TGA:    temp = IMG_LoadTGA_RW(rw);          break;
        case core::ImageFormat::XCF:    temp = IMG_LoadXCF_RW(rw);          break;
        case core::ImageFormat::XPM:    temp = IMG_LoadXPM_RW(rw);          break;
        case core::ImageFormat::SVG:    temp = IMG_LoadSVG_RW(rw);          break;
        default:                            temp = IMG_Load_RW(rw, 0);          break;
    }

    SDL_RWclose(rw);

    if (temp == nullptr) 
    {
        core::NexusException("Surface", "The image could not be loaded from memory.",
            "SDL_image", IMG_GetError());
    }

    Destroy(); surface = temp;
}

gfx::Color gfx::Surface::GetPixelUnsafe(const void* pixelAddress) const
{
    return gfx::Color(*static_cast<const Uint32*>(pixelAddress), surface->format);
}

gfx::Color gfx::Surface::GetPixelUnsafe(int byteOffset) const
{
    return GetPixelUnsafe(static_cast<const Uint8*>(surface->pixels) + byteOffset);
}

gfx::Color gfx::Surface::GetPixelUnsafe(int x, int y) const
{
    return GetPixelUnsafe(static_cast<const Uint8*>(surface->pixels) + y * surface->pitch + x * surface->format->BytesPerPixel);
}

gfx::Color gfx::Surface::GetPixelUnsafe(const math::IVec2& position) const
{
    return GetPixelUnsafe(position.x, position.y);
}

gfx::Color gfx::Surface::GetFragUnsafe(const math::Vec2& uv) const
{
    const int x = uv.x * (surface->w-1), y = uv.y * (surface->h-1);
    return GetPixelUnsafe(static_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * surface->format->BytesPerPixel);
}

gfx::Color gfx::Surface::GetPixel(const void* pixelAddress) const
{
    if (surface && pixelAddress >= surface->pixels && pixelAddress < static_cast<const Uint8*>(surface->pixels) + surface->w * surface->h * surface->format->BytesPerPixel)
    {
        return GetPixelUnsafe(pixelAddress);
    }

    return Blank;
}

gfx::Color gfx::Surface::GetPixel(int byteOffset) const
{
    if (surface && byteOffset < surface->w * surface->h * surface->format->BytesPerPixel)
    {
        return GetPixelUnsafe(static_cast<const Uint8*>(surface->pixels) + byteOffset);
    }

    return Blank;
}

gfx::Color gfx::Surface::GetPixel(int x, int y) const
{
    if (surface && x >= 0 && x < surface->w && y >= 0 && y < surface->h)
    {
        return GetPixelUnsafe(x, y);
    }

    return Blank;
}

gfx::Color gfx::Surface::GetPixel(const math::IVec2& position) const
{
    return GetPixel(position.x, position.y);
}

gfx::Color gfx::Surface::GetFrag(const math::Vec2& uv) const
{
    if (surface->pixels && uv.x >= 0.0f && uv.x <= 1.0f && uv.y >= 0.0f && uv.y <= 1.0f)
    {
        return GetFragUnsafe(uv);
    }
    return gfx::Blank;
}

gfx::BlendMode gfx::Surface::GetBlendMode() const
{
    SDL_BlendMode blendMode;

    if (SDL_GetSurfaceBlendMode(surface, &blendMode) < 0)
    {
        throw core::NexusException("gfx::Surface", "Unable to get the BlendMode.",
            "SDL", SDL_GetError());
    }

    return static_cast<BlendMode>(blendMode);
}

void gfx::Surface::SetBlendMode(BlendMode blendMode) const
{
    if (SDL_SetSurfaceBlendMode(surface, static_cast<SDL_BlendMode>(blendMode)) < 0)
    {
        throw core::NexusException("gfx::Surface", "Unable to set the BlendMode.",
            "SDL", SDL_GetError());
    }
}

void gfx::Surface::CopyFrom(const Surface& source)
{
    // Checks that the source is not the same as the current surface
    if (&source == this) return;

    // Copy source attributes
    if (source.surface)
    {
        Destroy();  // Free the current surface if there is one
        surface = SDL_ConvertSurface(source.surface, source.surface->format, source.surface->flags);
        if (!surface) throw core::NexusException("gfx::Surface", "Error copying surface.",
            "SDL", SDL_GetError());
    }
}

gfx::Surface gfx::Surface::Clone(PixelFormat format) const
{
    if (!surface) throw core::NexusException("gfx::Surface", "Surface is not created yet. Cannot clone.");
    return SDL_ConvertSurfaceFormat(surface, format == PixelFormat::Unknown ? surface->format->format : static_cast<Uint32>(format), surface->flags);
}

gfx::Surface gfx::Surface::Crop(shape2D::Rectangle areaToKeep) const
{
    if (!surface) throw core::NexusException("gfx::Surface", "Surface is not created yet. Cannot crop.");

    areaToKeep.SetPosition(areaToKeep.GetPosition().Clamp({ 0, 0 }, GetSize()));
    areaToKeep.SetSize(areaToKeep.GetSize().Clamp({ 0, 0 }, GetSize()));

    if (areaToKeep.GetPosition() == areaToKeep.GetSize())
    {
        throw core::NexusException("gfx::Surface", "The area to be kept for the crop is null.");
    }

    SDL_Surface *newSurface = SDL_CreateRGBSurfaceWithFormat(surface->flags, areaToKeep.w, areaToKeep.h, surface->format->BitsPerPixel, surface->format->format);
    SDL_BlitSurface(surface, &areaToKeep, newSurface, &areaToKeep);

    return newSurface;
}

gfx::Surface& gfx::Surface::Resize(int newWidth, int newHeight)
{
    if (!surface) throw core::NexusException("gfx::Surface", "Surface is not created yet. Cannot resize.");

    SDL_Surface* newSurface = SDL_CreateRGBSurface(surface->flags, newWidth, newHeight, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
    if (!newSurface) throw core::NexusException("gfx::Surface", "Error creating the resized surface.",
        "SDL", SDL_GetError());

    SDL_Rect sourceRect = { 0, 0, std::min(newWidth, surface->w), std::min(newHeight, surface->h) };
    SDL_BlitSurface(surface, &sourceRect, newSurface, nullptr);

    SDL_FreeSurface(surface);
    surface = newSurface;

    return *this;
}

gfx::Surface& gfx::Surface::ResizeCanvas(int newWidth, int newHeight, int offsetX, int offsetY, const Color& background)
{
    if (!surface) throw core::NexusException("gfx::Surface", "Surface is not created yet. Cannot resize.");
    if (newWidth == surface->w || newHeight == surface->h) return *this;

    SDL_Surface *newSurface = SDL_CreateRGBSurface(
        surface->flags, newWidth, newHeight, surface->format->BitsPerPixel,
        surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

    if (!newSurface) throw core::NexusException("gfx::Surface", "Error creating the resized surface canvas.",
        "SDL", SDL_GetError());

    shape2D::Rectangle dst(offsetX, offsetY, surface->w, surface->h);
    SDL_BlitSurface(surface, nullptr, newSurface, &dst);

    if (offsetX > 0)
    {
        shape2D::Rectangle dst(0, 0, offsetX, newHeight);
        SDL_FillRect(newSurface, &dst, background);
    }

    if (offsetY > 0)
    {
        shape2D::Rectangle dst(0, 0, newWidth, offsetY);
        SDL_FillRect(newSurface, &dst, background);
    }

    if (offsetX + surface->w < newWidth)
    {
        shape2D::Rectangle dst(offsetX + surface->w, 0, newWidth - (offsetX + surface->w), offsetY);
        SDL_FillRect(newSurface, &dst, background);
    }

    if (offsetY + surface->h < newHeight)
    {
        shape2D::Rectangle dst(0, offsetY + surface->h, offsetX, newHeight - (offsetY + surface->h));
        SDL_FillRect(newSurface, &dst, background);
    }

    SDL_FreeSurface(surface);
    surface = newSurface;

    return *this;
}

gfx::Surface& gfx::Surface::ToPOT(const Color& background)
{
    if (!surface)
    {
        throw core::NexusException("gfx::Surface", "Surface is not created yet. Cannot resize.");
    }

    int potWidth = math::NextPowerOfTwo(surface->w);
    int potHeight = math::NextPowerOfTwo(surface->h);

    if (potWidth != surface->w || potHeight != surface->h)
    {
        ResizeCanvas(potWidth, potHeight, 0, 0, background);
    }

    return *this;
}

gfx::Surface& gfx::Surface::Rotate(float angle)
{
    if (!surface) return *this;

    angle *= math::Deg2Rad;

    Uint8 bytesPerPixel = surface->format->BytesPerPixel;
    Uint8 *originalPixels = static_cast<Uint8*>(surface->pixels);

    Surface rotatedSurface;
    rotatedSurface.Create(surface->h, surface->w, GetPixelFormat());

    Uint8 *rotatedPixels = static_cast<Uint8*>(rotatedSurface.surface->pixels);

    float sinRadius = std::sin(angle);
    float cosRadius = std::cos(angle);

    int width = static_cast<int>(std::abs(surface->w*cosRadius) + std::abs(surface->h*sinRadius));
    int height = static_cast<int>(std::abs(surface->h*cosRadius) + std::abs(surface->w*sinRadius));

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float oldX = ((x - width/2.0f)*cosRadius + (y - height/2.0f)*sinRadius) + surface->w/2.0f;
            float oldY = ((y - height/2.0f)*cosRadius - (x - width/2.0f)*sinRadius) + surface->h/2.0f;

            if ((oldX >= 0) && (oldX < surface->w) && (oldY >= 0) && (oldY < surface->h))
            {
                int x1 = std::floor(oldX);
                int y1 = std::floor(oldY);
                int x2 = std::min(x1 + 1, surface->w - 1);
                int y2 = std::min(y1 + 1, surface->h - 1);

                float px = oldX - x1;
                float py = oldY - y1;

                for (int i = 0; i < bytesPerPixel; i++)
                {
                    float f1 = originalPixels[(y1*surface->w + x1)*bytesPerPixel + i];
                    float f2 = originalPixels[(y1*surface->w + x2)*bytesPerPixel + i];
                    float f3 = originalPixels[(y2*surface->w + x1)*bytesPerPixel + i];
                    float f4 = originalPixels[(y2*surface->w + x2)*bytesPerPixel + i];

                    rotatedPixels[(y*width + x)*bytesPerPixel + i] = static_cast<Uint8>(
                        f1*(1 - px)*(1 - py) + f2*px*(1 - py) + f3*(1 - px)*py + f4*px*py);
                }
            }
        }

        surface = std::exchange(rotatedSurface.surface, nullptr);
    }

    return *this;
}

gfx::Surface& gfx::Surface::RotateCCW()
{
    if (!surface) return *this;

    Uint8 bytesPerPixel = surface->format->BytesPerPixel;
    Uint8* originalPixels = static_cast<Uint8*>(surface->pixels);

    Surface rotatedSurface;
    rotatedSurface.Create(surface->h, surface->w, GetPixelFormat());

    Uint8* rotatedPixels = static_cast<Uint8*>(rotatedSurface.surface->pixels);

    for (int y = 0; y < surface->h; y++)
    {
        for (int x = 0; x < surface->w; x++)
        {
            std::memcpy(rotatedPixels + (x*surface->h + y)*bytesPerPixel, originalPixels + (y*surface->w + (surface->w - x - 1))*bytesPerPixel, bytesPerPixel);
        }
    }

    surface = std::exchange(rotatedSurface.surface, nullptr);

    return *this;
}

gfx::Surface& gfx::Surface::FlipHorizontal()
{
    if (!surface) return *this;

    Uint8 bytesPerPixel = surface->format->BytesPerPixel;
    Uint8 *originalPixels = static_cast<Uint8*>(surface->pixels);

    Surface flippedSurface;
    flippedSurface.Create(surface->h, surface->w, GetPixelFormat());

    Uint8 *flippedPixels = static_cast<Uint8*>(flippedSurface.surface->pixels);

    for (int y = 0; y < surface->h; y++)
    {
        for (int x = 0; x < surface->w; x++)
        {
            std::memcpy(flippedPixels + (y*surface->w + x)*bytesPerPixel, originalPixels + (y*surface->w + (surface->w - 1 - x))*bytesPerPixel, bytesPerPixel);
        }
    }

    surface = std::exchange(flippedSurface.surface, nullptr);

    return *this;
}

gfx::Surface& gfx::Surface::FlipVertical()
{
    if (!surface) return *this;

    const Uint8 bytesPerPixel = surface->format->BytesPerPixel;
    const Uint8 *originalPixels = static_cast<Uint8*>(surface->pixels);

    Surface flippedSurface;
    flippedSurface.Create(surface->h, surface->w, GetPixelFormat());

    Uint8 *flippedPixels = static_cast<Uint8*>(flippedSurface.surface->pixels);

    for (int i = (surface->h - 1), offsetSize = 0; i >= 0; i--)
    {
        std::memcpy(flippedPixels + offsetSize, originalPixels + i * surface->w * bytesPerPixel, surface->w * bytesPerPixel);
        offsetSize += surface->w * bytesPerPixel;
    }

    surface = std::exchange(flippedSurface.surface, nullptr);

    return *this;
}

void gfx::Surface::SaveImage(const std::string& filePath, core::ImageFormat format, int quality) const
{
    int result = 0;

    switch (format)
    {
        case core::ImageFormat::BMP: {
            result = SDL_SaveBMP(surface, filePath.c_str());
        } break;

        case core::ImageFormat::JPG: {
            result = IMG_SaveJPG(surface, filePath.c_str(), quality);
        } break;

        case core::ImageFormat::PNG: {
            result = IMG_SavePNG(surface, filePath.c_str());
        } break;

        default: {
            throw core::NexusException("gfx::Surface",
                "Export format not supported.");
        } break;
    }

    if (result < 0)
    {
        throw core::NexusException("gfx::Surface", "Error saving image.",
            "SDL_image", IMG_GetError());
    }
}

void gfx::Surface::SetPixelUnsafe(void* pixelAddress, const gfx::Color& color) const
{
    *static_cast<Uint32*>(pixelAddress) = color.ToUint32(surface->format);
}

void gfx::Surface::SetPixelUnsafe(int byteOffset, const gfx::Color& color) const
{
    SetPixelUnsafe(static_cast<Uint8*>(surface->pixels) + byteOffset, color);
}

void gfx::Surface::SetPixelUnsafe(int x, int y, const gfx::Color& color) const
{
    SetPixelUnsafe(static_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * surface->format->BytesPerPixel, color);
}

void gfx::Surface::SetPixelUnsafe(const math::IVec2& position, const gfx::Color& color) const
{
    SetPixelUnsafe(position.x, position.y, color);
}

void gfx::Surface::SetFragUnsafe(const math::Vec2 uv, const gfx::Color& color) const
{
    const int x = uv.x * (surface->w-1), y = uv.y * (surface->h-1);
    SetPixelUnsafe(static_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * surface->format->BytesPerPixel, color);
}

bool gfx::Surface::SetPixel(void* pixelAddress, const gfx::Color& color) const
{
    if (surface && pixelAddress >= surface->pixels && pixelAddress < static_cast<const Uint8*>(surface->pixels) + surface->w * surface->h * surface->format->BytesPerPixel)
    {
        SetPixelUnsafe(pixelAddress, color);
        return true;
    }
    return false;
}

bool gfx::Surface::SetPixel(int byteOffset, const gfx::Color& color) const
{
    if (surface && byteOffset < surface->w * surface->h * surface->format->BytesPerPixel)
    {
        SetPixelUnsafe(static_cast<Uint8*>(surface->pixels) + byteOffset, color);
        return true;
    }
    return false;
}

bool gfx::Surface::SetPixel(int x, int y, const gfx::Color& color) const
{
    if (surface && x >= 0 && x < surface->w && y >= 0 && y < surface->h)
    {
        SetPixelUnsafe(x, y, color);
        return true;
    }
    return false;
}

bool gfx::Surface::SetPixel(const math::IVec2& position, const gfx::Color& color) const
{
    return SetPixel(position.x, position.y, color);
}

bool gfx::Surface::SetFrag(const math::Vec2& uv, const gfx::Color& color) const
{
    if (surface->pixels && uv.x >= 0.0f && uv.x <= 1.0f && uv.y >= 0.0f && uv.y <= 1.0f)
    {
        SetFragUnsafe(uv, color);
        return true;
    }
    return false;
}

const gfx::Surface& gfx::Surface::Fill(const gfx::Color& color) const
{
    SDL_FillRect(surface, nullptr, color);
    return *this;
}

const gfx::Surface& gfx::Surface::DrawGradientLinear(shape2D::Rectangle dst, float direction, const gfx::Color& start, const gfx::Color& end) const
{
    dst.SetPosition(dst.GetPosition().Clamp({ 0, 0 }, GetSize()));
    dst.SetSize(dst.GetSize().Clamp({ 0, 0 }, GetSize()));

    direction = (90 - direction) * math::Deg2Rad;
    const float c = std::cos(direction);
    const float s = std::sin(direction);

    const math::Vec4 nStart = start.Normalized();
    const math::Vec4 nEnd = end.Normalized();

    for (int y = dst.y; y < dst.h; y++)
    {
        for (int x = dst.x; x < dst.w; x++)
        {
            // Calculate the relative position of the pixel along the gradient direction
            float pos = (x * c + y * s) / (dst.w * c + dst.h * s);
            float factor = std::clamp(pos, 0.0f, 1.0f);

            SetPixelUnsafe(x, y, nEnd * factor + nStart * (1.0f - factor));
        }
    }

    return *this;
}

const gfx::Surface& gfx::Surface::DrawGradientRadial(shape2D::Rectangle dst, float density, const gfx::Color& inner, const gfx::Color& outer) const
{
    dst.SetPosition(dst.GetPosition().Clamp({ 0, 0 }, GetSize()));
    dst.SetSize(dst.GetSize().Clamp({ 0, 0 }, GetSize()));

    math::IVec2 center(dst.w * 0.5f, dst.h * 0.5f);
    const float radius = std::min(center.x, center.y);

    const math::Vec4 nInner = inner.Normalized();
    const math::Vec4 nOuter = outer.Normalized();

    for (int y = dst.y; y < dst.h; y++)
    {
        for (int x = dst.x; x < dst.w; x++)
        {
            float dist = std::hypotf(x - center.x, y - center.y);
            float factor = (dist - radius * density) / (radius * (1.0f - density));
            factor = std::clamp(factor, 0.0f, 1.0f);

            SetPixelUnsafe(x, y, nOuter * factor + nInner * (1.0f - factor));
        }
    }

    return *this;
}

const gfx::Surface& gfx::Surface::DrawGradientSquare(shape2D::Rectangle dst, float density, const gfx::Color& inner, const gfx::Color& outer) const
{
    dst.SetPosition(dst.GetPosition().Clamp({ 0, 0 }, GetSize()));
    dst.SetSize(dst.GetSize().Clamp({ 0, 0 }, GetSize()));

    math::IVec2 center(dst.w * 0.5f, dst.h * 0.5f);

    const math::Vec4 nInner = inner.Normalized();
    const math::Vec4 nOuter = outer.Normalized();

    for (int y = dst.y; y < dst.h; y++)
    {
        for (int x = dst.x; x < dst.w; x++)
        {
            // Calculate the Manhattan distance from the center
            math::Vec2 dist = (math::Vec2(x, y) - center).Abs();

            // Normalize the distances by the dimensions of the gradient rectangle
            math::Vec2 normalizedDist = dist / center;

            // Calculate the total normalized Manhattan distance
            float manhattanDist = std::max(normalizedDist.x, normalizedDist.y);

            // Subtract the density from the manhattanDist, then divide by (1 - density)
            // This makes the gradient start from the center when density is 0, and from the edge when density is 1
            float factor = (manhattanDist - density) / (1.0f - density);
            factor = std::clamp(factor, 0.0f, 1.0f);

            // Blend the colors based on the calculated factor
            SetPixelUnsafe(x, y, nOuter * factor + nInner * (1.0f - factor));
        }
    }

    return *this;
}

const gfx::Surface& gfx::Surface::DrawChecked(shape2D::Rectangle dst, int checksX, int checksY, const gfx::Color& col1, const gfx::Color& col2) const
{
    dst.SetPosition(dst.GetPosition().Clamp({ 0, 0 }, GetSize()));
    dst.SetSize(dst.GetSize().Clamp({ 0, 0 }, GetSize()));

    int i = 0;
    for (int y = dst.y; y < dst.h; y += checksY)
    {
        for (int x = dst.x; x < dst.w; x += checksX)
        {
            DrawRectangle(x, y, checksX, checksY, i++ % 2 ? col1 : col2);
        }
        i++;
    }

    return *this;
}

const gfx::Surface& gfx::Surface::DrawWhiteNoise(shape2D::Rectangle dst, float factor) const
{
    dst.SetPosition(dst.GetPosition().Clamp({ 0, 0 }, GetSize()));
    dst.SetSize(dst.GetSize().Clamp({ 0, 0 }, GetSize()));

    core::RandomGenerator gen;

    for (int y = dst.y; y < dst.h; y++)
    {
        for (int x = dst.x; x < dst.w; x++)
        {
            SetPixelUnsafe(x, y, gen.Random(0.0f, 1.0f) < factor ? gfx::White : gfx::Black);
        }
    }

    return *this;
}

const gfx::Surface& gfx::Surface::DrawCellular(shape2D::Rectangle dst, int tileSize) const
{
    dst.SetPosition(dst.GetPosition().Clamp({ 0, 0 }, GetSize()));
    dst.SetSize(dst.GetSize().Clamp({ 0, 0 }, GetSize()));

    core::RandomGenerator gen;

    int seedsPerRow = dst.w / tileSize;
    int seedsPerCol = dst.h / tileSize;
    int seedCount = seedsPerRow * seedsPerCol;

    math::Vec2 *seeds = new math::Vec2[seedCount];

    for (int i = 0; i < seedCount; i++)
    {
        int y = (i / seedsPerRow) * tileSize + gen.Random(0, tileSize - 1);
        int x = (i % seedsPerRow) * tileSize + gen.Random(0, tileSize - 1);
        seeds[i] = math::Vec2(x, y);
    }

    for (int y = dst.y; y < dst.h; y++)
    {
        int tileY = y / tileSize;

        for (int x = dst.x; x < dst.w; x++)
        {
            int tileX = x / tileSize;

            float minDistance = 65536.0f; //(float)strtod("Inf", NULL);

            // Check all adjacent tiles
            for (int i = -1; i < 2; i++)
            {
                if (tileX + i < 0 || tileX + i >= seedsPerRow)
                {
                    continue;
                }

                for (int j = -1; j < 2; j++)
                {
                    if (tileY + j < 0 || tileY + j >= seedsPerCol)
                    {
                        continue;
                    }

                    const math::Vec2 &neighborSeed = seeds[(tileY + j) * seedsPerRow + tileX + i];

                    float dist = std::hypotf(x - neighborSeed.x, y - neighborSeed.y);
                    minDistance = std::min(minDistance, dist);
                }
            }

            // I made this up, but it seems to give good results at all tile sizes
            Uint8 intensity = std::min(minDistance * 256.0f / tileSize, 255.0f);
            SetPixelUnsafe(x, y, { intensity, intensity, intensity, 255 });
        }
    }

    delete[] seeds;

    return *this;
}

const gfx::Surface& gfx::Surface::DrawLine(int x1, int y1, int x2, int y2, const gfx::Color& color) const
{
    const float dx = x2 - x1;
    const float dy = y2 - y1;

    if (dx == 0 && dy == 0)
    {
        SetPixel(x1, y1, color);
        return *this;
    }

    const float adx = std::fabs(dx);
    const float ady = std::fabs(dy);

    if (adx > ady)
    {
        const float slope = dy / dx;

        int xMin, xMax;
        if (x1 < x2)
        {
            xMin = x1, xMax = x2;
        }
        else
        {
            xMin = x2, xMax = x1;
        }

        for (int x = xMin; x <= xMax; x++)
        {
            const int y = y1 + (x - x1) * slope;
            SetPixel(x, y, color);
        }
    }
    else
    {
        const float slope = dx / dy;

        int yMin, yMax;
        if (y1 < y2)
        {
            yMin = y1, yMax = y2;
        }
        else
        {
            yMin = y2, yMax = y1;
        }

        for (int y = yMin; y <= yMax; y++)
        {
            const int x = x1 + (y - y1) * slope;
            SetPixel(x, y, color);
        }
    }

    return *this;
}

const gfx::Surface& gfx::Surface::DrawLine(const math::Vec2& start, const math::Vec2& end, const Color& color) const
{
    return DrawLine(start.x, start.y, end.x, end.y, color);
}

const gfx::Surface& gfx::Surface::DrawLine(const shape2D::Line& line, const gfx::Color& color) const
{
    return DrawLine(std::round(line.start.x), std::round(line.start.y), std::round(line.end.x), std::round(line.end.y), color);
}

const gfx::Surface& gfx::Surface::DrawRectangle(int x, int y, int w, int h, const Color& color) const
{
    shape2D::Rectangle rect(x, y, w, h);
    SDL_FillRect(surface, &rect, color.ToUint32(surface->format));
    return *this;
}

const gfx::Surface& gfx::Surface::DrawRectangle(const shape2D::Rectangle& rect, const Color& color) const
{
    SDL_FillRect(surface, &rect, color.ToUint32(surface->format));
    return *this;
}

const gfx::Surface& gfx::Surface::DrawRectangle(const shape2D::RectangleF& rect, const Color& color) const
{
    return DrawRectangle(shape2D::Rectangle(std::round(rect.x), std::round(rect.y), std::round(rect.w), std::round(rect.h)), color);
}

const gfx::Surface& gfx::Surface::DrawRectangleLines(int x, int y, int w, int h, int thick, const Color& color) const
{
    return DrawRectangleLines(shape2D::Rectangle{ x, y, w, h }, thick, color);
}

const gfx::Surface& gfx::Surface::DrawRectangleLines(const shape2D::Rectangle& rect, int thick, const Color& color) const
{
    DrawRectangle(shape2D::Rectangle(rect.x, rect.y, rect.w, thick), color);
    DrawRectangle(shape2D::Rectangle(rect.x, rect.y + thick, thick, rect.h - thick*2), color);
    DrawRectangle(shape2D::Rectangle(rect.x + rect.w - thick, rect.y + thick, thick, rect.h - thick*2), color);
    DrawRectangle(shape2D::Rectangle(rect.x, rect.y + rect.h - thick, rect.w, thick), color);

    return *this;
}

const gfx::Surface& gfx::Surface::DrawRectangleLines(const shape2D::RectangleF& rect, int thick, const Color& color) const
{
    DrawRectangle(shape2D::RectangleF(rect.x, rect.y, rect.w, thick), color);
    DrawRectangle(shape2D::RectangleF(rect.x, rect.y + thick, thick, rect.h - thick*2), color);
    DrawRectangle(shape2D::RectangleF(rect.x + rect.w - thick, rect.y + thick, thick, rect.h - thick*2), color);
    DrawRectangle(shape2D::RectangleF(rect.x, rect.y + rect.h - thick, rect.w, thick), color);

    return *this;
}

const gfx::Surface& gfx::Surface::DrawAABB(const shape2D::AABB& aabb, const Color& color) const
{
    return DrawRectangleLines(shape2D::RectangleF((aabb.min + aabb.max) * 0.5f, aabb.max - aabb.min), 1, color);
}

const gfx::Surface& gfx::Surface::DrawCircle(int cx, int cy, int radius, const Color& color) const
{
    int x = 0, y = radius;
    int decesionParameter = 3 - 2 * radius;

    while (y >= x)
    {
        DrawRectangle(shape2D::Rectangle(cx - x, cy + y, x * 2, 1), color);
        DrawRectangle(shape2D::Rectangle(cx - x, cy - y, x * 2, 1), color);
        DrawRectangle(shape2D::Rectangle(cx - y, cy + x, y * 2, 1), color);
        DrawRectangle(shape2D::Rectangle(cx - y, cy - x, y * 2, 1), color);
        x++;

        if (decesionParameter > 0)
        {
            y--;
            decesionParameter = decesionParameter + 4 * (x - y) + 10;
        }
        else
        {
            decesionParameter = decesionParameter + 4 * x + 6;
        }
    }

    return *this;
}

const gfx::Surface& gfx::Surface::DrawCircle(const shape2D::Circle& circle, const Color& color) const
{
    return DrawCircle(std::round(circle.center.x), std::round(circle.center.y), std::round(circle.radius), color);
}

const gfx::Surface& gfx::Surface::DrawCircleLines(int cx, int cy, int radius, const Color& color) const
{
    int x = 0, y = radius;
    int decesionParameter = 3 - 2 * radius;

    while (y >= x)
    {
        SetPixel(cx + x, cy + y, color);
        SetPixel(cx - x, cy + y, color);
        SetPixel(cx + x, cy - y, color);
        SetPixel(cx - x, cy - y, color);
        SetPixel(cx + y, cy + x, color);
        SetPixel(cx - y, cy + x, color);
        SetPixel(cx + y, cy - x, color);
        SetPixel(cx - y, cy - x, color);
        x++;

        if (decesionParameter > 0)
        {
            y--;
            decesionParameter = decesionParameter + 4 * (x - y) + 10;
        }
        else
        {
            decesionParameter = decesionParameter + 4 * x + 6;
        }
    }

    return *this;
}

const gfx::Surface& gfx::Surface::DrawCircleLines(const shape2D::Circle& circle, const Color& color) const
{
    return DrawCircleLines(std::round(circle.center.x), std::round(circle.center.y), std::round(circle.radius), color);
}

const gfx::Surface& gfx::Surface::DrawPolygon(const shape2D::Polygon& poly, const gfx::Color& color) const
{
    shape2D::AABB bounds = poly.GetAABB();

    bounds.min = bounds.min.Clamp({ 0, 0 }, GetSize());
    bounds.max = bounds.max.Clamp({ 0, 0 }, GetSize());

    if (bounds.min == bounds.max) return *this;

    for (int y = bounds.min.y; y < bounds.max.y; y++)
    {
        for (int x = bounds.min.x; x < bounds.max.x; x++)
        {
            if (poly.CollisionPoint(math::IVec2{ x, y }))
            {
                SetPixelUnsafe(x, y, color);
            }
        }
    }

    return *this;
}

const gfx::Surface& gfx::Surface::DrawPolygonLines(const shape2D::Polygon& poly, const Color& color) const
{
    for (Uint32 i = 0, j = 1; i < poly.size(); i++)
    {
        DrawLine(poly.GetEdge(i), color);
    }

    return *this;
}

const gfx::Surface& gfx::Surface::DrawTriangle(const math::Vec2& v0, const math::Vec2& v1, const math::Vec2& v2, const gfx::Color& color) const
{
    return DrawTriangle({ v0, v1, v2 }, color);
}

const gfx::Surface& gfx::Surface::DrawTriangle(const shape2D::Triangle& tri, const gfx::Color& color) const
{
    shape2D::AABB aabb = tri.GetAABB();
    aabb.min.Clamp({ 0, 0 }, GetSize());
    aabb.max.Clamp({ 0, 0 }, GetSize());

    if (aabb.min == aabb.max) return *this;

    for (int y = aabb.min.y; y < aabb.max.y; y++)
    {
        for (int x = aabb.min.x; x < aabb.max.x; x++)
        {
            if (tri.CollisionPoint(math::Vec2(x, y)))
            {
                SetPixelUnsafe(x, y, color);
            }
        }
    }

    return *this;
}

const gfx::Surface& gfx::Surface::DrawTriangleLines(const math::Vec2& v0, const math::Vec2& v1, const math::Vec2& v2, const gfx::Color& color) const
{
    DrawLine(v0, v1, color);
    DrawLine(v1, v2, color);
    DrawLine(v2, v0, color);

    return *this;
}

const gfx::Surface& gfx::Surface::DrawTriangleLines(const shape2D::Triangle& tri, const gfx::Color& color) const
{
    return DrawTriangleLines(tri.a, tri.b, tri.c, color);
}

const gfx::Surface& gfx::Surface::DrawTriangleColors(const shape2D::Vertex& v0, const shape2D::Vertex& v1, const shape2D::Vertex& v2) const
{
    // Get integer 2D position coordinates
    const math::IVec2 iV0(v0.position.x, v0.position.y);
    const math::IVec2 iV1(v1.position.x, v1.position.y);
    const math::IVec2 iV2(v2.position.x, v2.position.y);

    // Check if vertices are in clockwise order or degenerate, in which case the triangle cannot be rendered
    if ((iV1.x - iV0.x) * (iV2.y - iV0.y) - (iV2.x - iV0.x) * (iV1.y - iV0.y) >= 0.0f) return *this;

    // Calculate the 2D bounding box of the triangle clamped to the surface dimensions
    const math::Vector2<Uint16> min = iV0.Min(iV1.Min(iV2)).Clamp(
        { 0, 0 }, { surface->w - 1, surface->h -1 });

    const math::Vector2<Uint16> max = iV0.Max(iV1.Max(iV2)).Clamp(
        { 0, 0 }, { surface->w - 1, surface->h -1 });

    // If triangle is entirely outside the surface we can stop now
    if (min == max) return *this;

    // Calculate original edge weights relative to bounds.min
    // Will be used to obtain barycentric coordinates by incrementing then averaging them
    int w0Row = (min.x - iV1.x) * (iV2.y - iV1.y) - (iV2.x - iV1.x) * (min.y - iV1.y);
    int w1Row = (min.x - iV2.x) * (iV0.y - iV2.y) - (iV0.x - iV2.x) * (min.y - iV2.y);
    int w2Row = (min.x - iV0.x) * (iV1.y - iV0.y) - (iV1.x - iV0.x) * (min.y - iV0.y);

    // Calculate weight increment steps for each edge
    const math::IVec2 sW0(iV2.y - iV1.y, iV1.x - iV2.x);
    const math::IVec2 sW1(iV0.y - iV2.y, iV2.x - iV0.x);
    const math::IVec2 sW2(iV1.y - iV0.y, iV0.x - iV1.x);

    // Normalize colors for barycentric interpolation
    const math::Vec4 nColV0 = v0.color.Normalized();
    const math::Vec4 nColV1 = v1.color.Normalized();
    const math::Vec4 nColV2 = v2.color.Normalized();

    // Fill the triangle with vertex colors
    for (Uint16 y = min.y; y <= max.y; y++)
    {
        const Uint32 yOffset = y * this->GetPitch();
        int w0 = w0Row, w1 = w1Row, w2 = w2Row;

        for (Uint16 x = min.x; x <= max.x; x++)
        {
            if ((w0 | w1 | w2) >= 0)
            {
                const Uint32 byteOffset = yOffset + x * this->GetBytesPerPixel();

                const float invSum = 1.0f / (w0 + w1 + w2);
                const float aW0 = w0 * invSum, aW1 = w1 * invSum, aW2 = w2 * invSum;

                gfx::Color out = nColV0 * aW0 + nColV1 * aW1 + nColV2 * aW2;

                if (out.a && out.a != 255)
                {
                    const gfx::Color dst = this->GetPixelUnsafe(byteOffset);
                    const Uint16 alpha = static_cast<Uint16>(out.a) + 1;
                    const Uint16 invAlpha = 256 - alpha;

                    out.a = static_cast<Uint8>((alpha * 256 + dst.a * invAlpha) >> 8);
                    out.r = static_cast<Uint8>((out.r * alpha + dst.r * invAlpha) >> 8);
                    out.g = static_cast<Uint8>((out.g * alpha + dst.g * invAlpha) >> 8);
                    out.b = static_cast<Uint8>((out.b * alpha + dst.b * invAlpha) >> 8);
                }

                if (out.a)
                {
                    this->SetPixelUnsafe(byteOffset, out);
                }
            }

            w0 += sW0.x, w1 += sW1.x, w2 += sW2.x;
        }

        w0Row += sW0.y, w1Row += sW1.y, w2Row += sW2.y;
    }

    return *this;
}

const gfx::Surface& gfx::Surface::DrawTriangleImage(const shape2D::Vertex& v0, const shape2D::Vertex& v1, const shape2D::Vertex& v2, const gfx::Surface& image) const
{
    // Get integer 2D position coordinates
    const math::Vector2<int> iV0(v0.position.x, v0.position.y);
    const math::Vector2<int> iV1(v1.position.x, v1.position.y);
    const math::Vector2<int> iV2(v2.position.x, v2.position.y);

    // Check if vertices are in clockwise order or degenerate, in which case the triangle cannot be rendered
    if ((iV1.x - iV0.x) * (iV2.y - iV0.y) - (iV2.x - iV0.x) * (iV1.y - iV0.y) >= 0.0f) return *this;

    // Calculate the 2D bounding box of the triangle clamped to the surface dimensions
    const math::Vector2<Uint16> min = iV0.Min(iV1.Min(iV2)).Clamp(
        { 0, 0 }, math::Vector2<Uint16>(surface->w - 1, surface->h -1));

    const math::Vector2<Uint16> max = iV0.Max(iV1.Max(iV2)).Clamp(
        { 0, 0 }, math::Vector2<Uint16>(surface->w - 1, surface->h -1));

    // Calculate original edge weights relative to bounds.min
    // Will be used to obtain barycentric coordinates by incrementing then averaging them
    int w0Row = (min.x - iV1.x) * (iV2.y - iV1.y) - (iV2.x - iV1.x) * (min.y - iV1.y);
    int w1Row = (min.x - iV2.x) * (iV0.y - iV2.y) - (iV0.x - iV2.x) * (min.y - iV2.y);
    int w2Row = (min.x - iV0.x) * (iV1.y - iV0.y) - (iV1.x - iV0.x) * (min.y - iV0.y);

    // Calculate weight increment steps for each edge
    const math::IVec2 sW0(iV2.y - iV1.y, iV1.x - iV2.x);
    const math::IVec2 sW1(iV0.y - iV2.y, iV2.x - iV0.x);
    const math::IVec2 sW2(iV1.y - iV0.y, iV0.x - iV1.x);

    // Normalize colors for barycentric interpolation
    const math::Vec4 nColV0 = v0.color.Normalized();
    const math::Vec4 nColV1 = v1.color.Normalized();
    const math::Vec4 nColV2 = v2.color.Normalized();

    // Fill the triangle with the image based on the provided parameters
    for (Uint16 y = min.y; y <= max.y; y++)
    {
        const Uint32 yOffset = y * this->GetPitch();
        int w0 = w0Row, w1 = w1Row, w2 = w2Row;

        for (Uint16 x = min.x; x <= max.x; x++)
        {
            if ((w0 | w1 | w2) >= 0)
            {
                const Uint32 byteOffset = yOffset + x * this->GetBytesPerPixel();

                const float invSum = 1.0f / (w0 + w1 + w2);
                const float aW0 = w0 * invSum, aW1 = w1 * invSum, aW2 = w2 * invSum;

                gfx::Color out = image.GetFrag(v0.texcoord * aW0 + v1.texcoord * aW1 + v2.texcoord * aW2);
                out *= static_cast<gfx::Color>(nColV0 * aW0 + nColV1 * aW1 + nColV2 * aW2);

                if (out.a && out.a != 255)
                {
                    const gfx::Color dst = this->GetPixelUnsafe(byteOffset);
                    const Uint16 alpha = static_cast<Uint16>(out.a) + 1;
                    const Uint16 invAlpha = 256 - alpha;

                    out.a = static_cast<Uint8>((alpha * 256 + dst.a * invAlpha) >> 8);
                    out.r = static_cast<Uint8>((out.r * alpha + dst.r * invAlpha) >> 8);
                    out.g = static_cast<Uint8>((out.g * alpha + dst.g * invAlpha) >> 8);
                    out.b = static_cast<Uint8>((out.b * alpha + dst.b * invAlpha) >> 8);
                }

                if (out.a)
                {
                    this->SetPixelUnsafe(byteOffset, out);
                }
            }

            w0 += sW0.x, w1 += sW1.x, w2 += sW2.x;
        }

        w0Row += sW0.y, w1Row += sW1.y, w2Row += sW2.y;
    }

    return *this;
}

const gfx::Surface& gfx::Surface::DrawQuadColors(const shape2D::Vertex& v0, const shape2D::Vertex& v1, const shape2D::Vertex& v2, const shape2D::Vertex& v3) const
{
    DrawTriangleColors(v0, v1, v2);
    DrawTriangleColors(v2, v3, v0);

    return *this;
}

const gfx::Surface& gfx::Surface::DrawQuadImage(const shape2D::Vertex& v0, const shape2D::Vertex& v1, const shape2D::Vertex& v2, const shape2D::Vertex& v3, const gfx::Surface& image) const
{
    DrawTriangleImage(v0, v1, v2, image);
    DrawTriangleImage(v2, v3, v0, image);

    return *this;
}

const gfx::Surface& gfx::Surface::DrawMesh(const shape2D::Mesh& mesh, const gfx::Surface* image) const
{
    if (!image)
    {
        for (int i = 0; i < mesh.vertices.size(); i += 3)
        {
            DrawTriangleColors(mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2]);
        }
    }
    else
    {
        for (int i = 0; i < mesh.vertices.size(); i += 3)
        {
            DrawTriangleImage(mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2], *image);
        }
    }

    return *this;
}

gfx::Surface& gfx::Surface::DrawImage(const Surface& other, int x, int y, int ox, int oy)
{
    shape2D::Rectangle rectDst(x - ox, y - oy, other.GetWidth(), other.GetHeight());

    const bool lockedBefore = IsLocked();
    if (lockedBefore) Unlock();

    if (SDL_BlitSurface(other.surface, nullptr, surface, &rectDst) < 0)
    {
        throw core::NexusException(
            "Surface", "Unable to draw surface towards each other.",
            "SDL", SDL_GetError());
    }

    if (lockedBefore) Lock();

    return *this;
}

gfx::Surface& gfx::Surface::DrawImage(const Surface& other, const math::IVec2& position, const math::IVec2& origin)
{
    return this->DrawImage(other, position.x, position.y, origin.x, origin.y);
}

gfx::Surface& gfx::Surface::DrawImageScaled(const Surface& other, float x, float y, float sx, float sy, float ox, float oy)
{
    shape2D::Rectangle rectDst(
        std::round(x - ox * sx), std::round(y - oy * sy),
        std::round(other.GetWidth() * sx), std::round(other.GetHeight() * sy));

    const bool lockedBefore = IsLocked();
    if (lockedBefore) Unlock();

    if (SDL_BlitScaled(other.surface, nullptr, surface, &rectDst) < 0)
    {
        throw core::NexusException(
            "Surface", "Unable to draw surface towards each other.",
            "SDL", SDL_GetError());
    }

    if (lockedBefore) Lock();

    return *this;
}

gfx::Surface& gfx::Surface::DrawImageScaled(const Surface& other, const math::Vec2& position, const math::Vec2& scale, const math::Vec2& origin)
{
    return this->DrawImageScaled(other, position.x, position.y, scale.x, scale.y, origin.x, origin.y);
}

gfx::Surface& gfx::Surface::DrawImage(const Surface& other, const shape2D::Rectangle& rectSrc, shape2D::Rectangle rectDst)
{
    const bool lockedBefore = IsLocked();
    if (lockedBefore) Unlock();

    if (rectSrc.GetSize() != rectDst.GetSize())
    {
        if (SDL_BlitScaled(other.surface, &rectSrc, surface, &rectDst) < 0)
        {
            throw core::NexusException(
                "Surface", "Unable to draw surface towards each other.",
                "SDL", SDL_GetError());
        }
    }
    else
    {
        if (SDL_BlitSurface(other.surface, &rectSrc, surface, &rectDst) < 0)
        {
            throw core::NexusException(
                "Surface", "Unable to draw surface towards each other.",
                "SDL", SDL_GetError());
        } 
    }

    if (lockedBefore) Lock();

    return *this;
}
