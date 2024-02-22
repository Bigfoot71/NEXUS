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
