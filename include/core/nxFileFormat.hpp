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

#ifndef NEXUS_CORE_FILE_FORMAT_HPP
#define NEXUS_CORE_FILE_FORMAT_HPP

#include "../platform/nxPlatform.hpp"

namespace nexus { namespace core {

    enum class NEXUS_API ImageFormat
    {
        BMP     = 0,
        GIF     = 1,
        JPEG    = 2,
        JPG     = 2,
        LBM     = 3,
        PCX     = 4,
        PNG     = 5,
        PNM     = 6,
        PPM     = 6,
        PGM     = 6,
        PBM     = 6,
        QOI     = 7,
        TGA     = 8,
        XCF     = 9,
        XPM     = 10,
        SVG     = 11
    };

}}

#endif //NEXUS_CORE_FILE_FORMAT_HPP
