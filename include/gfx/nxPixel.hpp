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

#ifndef NEXUS_GFX_PIXEL_HPP
#define NEXUS_GFX_PIXEL_HPP

#include "../core/nxException.hpp"
#include "SDL_pixels.h"
#include "SDL_stdinc.h"
#include <string>
#include <SDL.h>

namespace nexus { namespace gfx {

    enum class PixelFormat : Uint32
    {
        Unknown     = SDL_PIXELFORMAT_UNKNOWN,
        Index1LSB   = SDL_PIXELFORMAT_INDEX1LSB,
        Index1MSB   = SDL_PIXELFORMAT_INDEX1MSB,
        Index4LSB   = SDL_PIXELFORMAT_INDEX4LSB,
        Index4MSB   = SDL_PIXELFORMAT_INDEX4MSB,
        Index8      = SDL_PIXELFORMAT_INDEX8,
        RGB332      = SDL_PIXELFORMAT_RGB332,
        XRGB4444    = SDL_PIXELFORMAT_XRGB4444,
        RGB444      = SDL_PIXELFORMAT_RGB444,
        XBGR4444    = SDL_PIXELFORMAT_XBGR4444,
        BGR444      = SDL_PIXELFORMAT_BGR444,
        XRGB1555    = SDL_PIXELFORMAT_XRGB1555,
        RGB555      = SDL_PIXELFORMAT_RGB555,
        XBGR1555    = SDL_PIXELFORMAT_XBGR1555,
        BGR555      = SDL_PIXELFORMAT_BGR555,
        ARGB4444    = SDL_PIXELFORMAT_ARGB4444,
        RGBA4444    = SDL_PIXELFORMAT_RGBA4444,
        ABGR4444    = SDL_PIXELFORMAT_ABGR4444,
        BGRA4444    = SDL_PIXELFORMAT_BGRA4444,
        ARGB1555    = SDL_PIXELFORMAT_ARGB1555,
        RGBA5551    = SDL_PIXELFORMAT_RGBA5551,
        ABGR1555    = SDL_PIXELFORMAT_ABGR1555,
        BGRA5551    = SDL_PIXELFORMAT_BGRA5551,
        RGB565      = SDL_PIXELFORMAT_RGB565,
        BGR565      = SDL_PIXELFORMAT_BGR565,
        RGB24       = SDL_PIXELFORMAT_RGB24,
        BGR24       = SDL_PIXELFORMAT_BGR24,
        XRGB8888    = SDL_PIXELFORMAT_XRGB8888,
        RGB888      = SDL_PIXELFORMAT_RGB888,
        RGBX8888    = SDL_PIXELFORMAT_RGBX8888,
        XBGR8888    = SDL_PIXELFORMAT_XBGR8888,
        BGR888      = SDL_PIXELFORMAT_BGR888,
        BGRX8888    = SDL_PIXELFORMAT_BGRX8888,
        ARGB8888    = SDL_PIXELFORMAT_ARGB8888,
        RGBA8888    = SDL_PIXELFORMAT_RGBA8888,
        ABGR8888    = SDL_PIXELFORMAT_ABGR8888,
        BGRA8888    = SDL_PIXELFORMAT_BGRA8888,
        ARGB2101010 = SDL_PIXELFORMAT_ARGB2101010,
#   if SDL_BYTEORDER == SDL_BIG_ENDIAN
        RGBA32      = SDL_PIXELFORMAT_RGBA8888,
        ARGB32      = SDL_PIXELFORMAT_ARGB8888,
        BGRA32      = SDL_PIXELFORMAT_BGRA8888,
        ABGR32      = SDL_PIXELFORMAT_ABGR8888,
#   else
        RGBA32      = SDL_PIXELFORMAT_ABGR8888,
        ARGB32      = SDL_PIXELFORMAT_BGRA8888,
        BGRA32      = SDL_PIXELFORMAT_ARGB8888,
        ABGR32      = SDL_PIXELFORMAT_RGBA8888,
#   endif
        YV12        = SDL_PIXELFORMAT_YV12,
        IYUV        = SDL_PIXELFORMAT_IYUV,
        YUY2        = SDL_PIXELFORMAT_YUY2,
        UYVY        = SDL_PIXELFORMAT_UYVY,
        YVYU        = SDL_PIXELFORMAT_YVYU,
        NV12        = SDL_PIXELFORMAT_NV12,
        NV21        = SDL_PIXELFORMAT_NV21,
        ExternalOES = SDL_PIXELFORMAT_EXTERNAL_OES
    };

    struct NEXUS_API PixelInfo
    {
        Uint32 rMask, gMask, bMask, aMask;

        int bpp     = 0;    // Bits per pixel
        int depth   = 0;    // Depth in bits
        int pitch   = 0;    // Pixel spacing in bytes

        PixelInfo(PixelFormat format, Uint32 imageWidth = 0)
        {
            if (!SDL_PixelFormatEnumToMasks(static_cast<Uint32>(format), &bpp, &rMask, &gMask, &bMask, &aMask))
            {
                throw core::NexusException(
                    "PixelInfo", "Error getting pixel information from given format.",
                    "SDL", SDL_GetError());
            }

            depth = CalculateDepth(format);
            pitch = CalculatePitch(static_cast<Uint32>(format), imageWidth);
        }

        Uint32 GetFormatUint32() const
        {
            return SDL_MasksToPixelFormatEnum(bpp, rMask, gMask, bMask, aMask);
        }
        
        PixelFormat GetFormat() const
        {
            return static_cast<PixelFormat>(GetFormatUint32());
        }

        static int CalculatePitch(Uint32 format, Uint32 width)
        {
            int pitch;

            if (SDL_ISPIXELFORMAT_FOURCC(format) || SDL_BITSPERPIXEL(format) >= 8)
            {
                pitch = (width * SDL_BYTESPERPIXEL(format));
            }
            else
            {
                pitch = ((width * SDL_BITSPERPIXEL(format)) + 7) / 8;
            }

            return (pitch + 3) & ~3;    // 4-byte aligning for speed
        }

        static int CalculateDepth(PixelFormat format)
        {
            return SDL_BITSPERPIXEL(static_cast<int>(format));
        }

    };

}}

#endif //NEXUS_GFX_PIXEL_HPP