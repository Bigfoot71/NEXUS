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

#include "gfx/ext_gfx/nxStreamingTexture.hpp"

using namespace nexus;

void _gfx_impl::StreamingTexture::Lock(const shape2D::Rectangle& area)
{
    void *pixels = nullptr;
    int pitch = 0;

    if (SDL_LockTexture(data, &area, &pixels, &pitch) < 0)
    {
        throw core::NexusException("sr::StreamingTexture", "Unable to lock the StreamingTexture."
            "SDL", SDL_GetError());
    }

    this->lockedArea = area;

    this->surface = SDL_CreateRGBSurfaceFrom(pixels, area.w, area.h, format->BitsPerPixel, pitch,
        format->Rmask, format->Gmask, format->Bmask, format->Amask);
}

void _gfx_impl::StreamingTexture::Unlock()
{
    SDL_FreeSurface(this->surface);
    SDL_UnlockTexture(this->data);
    this->surface = nullptr;
}

void _gfx_impl::StreamingTexture::DrawImage(const Surface& other, int x, int y, int ox, int oy)
{
    shape2D::Rectangle rectDst(x - ox, y - oy, other.GetWidth(), other.GetHeight());

    if (SDL_BlitSurface(static_cast<SDL_Surface*>(other), nullptr, surface, &rectDst) < 0)
    {
        throw core::NexusException(
            "StreamingTexture", "Unable to draw surface onto StreamingTexture.",
            "SDL", SDL_GetError());
    }
}

void _gfx_impl::StreamingTexture::DrawImage(const Surface& other, const math::IVec2& position, const math::IVec2& origin)
{
    this->DrawImage(other, position.x, position.y, origin.x, origin.y);
}

void _gfx_impl::StreamingTexture::DrawImageScaled(const Surface& other, float x, float y, float sx, float sy, float ox, float oy)
{
    shape2D::Rectangle rectDst(
        std::round(x - ox * sx), std::round(y - oy * sy),
        std::round(other.GetWidth() * sx), std::round(other.GetHeight() * sy));

    if (SDL_BlitScaled(static_cast<SDL_Surface*>(other), nullptr, surface, &rectDst) < 0)
    {
        throw core::NexusException(
            "StreamingTexture", "Unable to draw surface onto StreamingTexture with scaling.",
            "SDL", SDL_GetError());
    }
}

void _gfx_impl::StreamingTexture::DrawImageScaled(const Surface& other, const math::Vec2& position, const math::Vec2& scale, const math::Vec2& origin)
{
    this->DrawImageScaled(other, position.x, position.y, scale.x, scale.y, origin.x, origin.y);
}

void _gfx_impl::StreamingTexture::DrawImage(const Surface& other, const shape2D::Rectangle& rectSrc, shape2D::Rectangle rectDst)
{
    if (rectSrc.GetSize() != rectDst.GetSize())
    {
        if (SDL_BlitScaled(static_cast<SDL_Surface*>(other), &rectSrc, surface, &rectDst) < 0)
        {
            throw core::NexusException(
                "StreamingTexture", "Unable to draw surface onto StreamingTexture with scaling.",
                "SDL", SDL_GetError());
        }
    }
    else
    {
        if (SDL_BlitSurface(static_cast<SDL_Surface*>(other), &rectSrc, surface, &rectDst) < 0)
        {
            throw core::NexusException(
                "StreamingTexture", "Unable to draw surface onto StreamingTexture.",
                "SDL", SDL_GetError());
        } 
    }
}
