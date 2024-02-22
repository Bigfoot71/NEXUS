#include "gapi/gl/nxUtils.hpp"
#include "gapi/gl/nxConfig.hpp"
#include "gapi/gl/nxContext.hpp"
#include "gapi/gl/nxExtensions.hpp"

using namespace nexus;

// Get current OpenGL version
gl::Version gl::GetVersion(void)
{
    Version glVersion;

#   if defined(GRAPHICS_API_OPENGL_11)
        glVersion = GlVersion::OpenGL_11;
#   endif

#   if defined(GRAPHICS_API_OPENGL_21)
        glVersion = GlVersion::OpenGL_21;
#   elif defined(GRAPHICS_API_OPENGL_43)
        glVersion = GlVersion::OpenGL_43;
#   elif defined(GRAPHICS_API_OPENGL_33)
        glVersion = Version::OpenGL_33;
#   endif

#   if defined(GRAPHICS_API_OPENGL_ES3)
        glVersion = GlVersion::OpenGL_ES_30;
#   elif defined(GRAPHICS_API_OPENGL_ES2)
        glVersion = GlVersion::OpenGL_ES_20;
#   endif

    return glVersion;
}

// Get name string for pixel format
const char* gl::GetTextureFormatName(TextureFormat format)
{
    switch (format)
    {
        case TextureFormat::Grayscale:        return "GRAYSCALE";     break;      ///< 8 bit per pixel (no alpha)
        case TextureFormat::GrayAlpha:        return "GRAY_ALPHA";    break;      ///< 8*2 bpp (2 channels)
        case TextureFormat::RGB565:           return "RGB565";        break;      ///< 16 bpp
        case TextureFormat::RGB888:           return "RGB888";        break;      ///< 24 bpp
        case TextureFormat::RGBA5551:         return "RGBA5551";      break;      ///< 16 bpp (1 bit alpha)
        case TextureFormat::RGBA4444:         return "RGBA4444";      break;      ///< 16 bpp (4 bit alpha)
        case TextureFormat::RGBA8888:         return "RGBA8888";      break;      ///< 32 bpp
        case TextureFormat::R32:              return "R32";           break;      ///< 32 bpp (1 channel - float)
        case TextureFormat::RGB323232:        return "RGB323232";     break;      ///< 32*3 bpp (3 channels - float)
        case TextureFormat::RGBA32323232:     return "RGBA32323232";  break;      ///< 32*4 bpp (4 channels - float)
        case TextureFormat::R16:              return "R16";           break;      ///< 16 bpp (1 channel - half float)
        case TextureFormat::RGB161616:        return "RGB161616";     break;      ///< 16*3 bpp (3 channels - half float)
        case TextureFormat::RGBA16161616:     return "RGBA16161616";  break;      ///< 16*4 bpp (4 channels - half float)
        case TextureFormat::DXT1_RGB:         return "DXT1_RGB";      break;      ///< 4 bpp (no alpha)
        case TextureFormat::DXT1_RGBA:        return "DXT1_RGBA";     break;      ///< 4 bpp (1 bit alpha)
        case TextureFormat::DXT3_RGBA:        return "DXT3_RGBA";     break;      ///< 8 bpp
        case TextureFormat::DXT5_RGBA:        return "DXT5_RGBA";     break;      ///< 8 bpp
        case TextureFormat::ETC1_RGB:         return "ETC1_RGB";      break;      ///< 4 bpp
        case TextureFormat::ETC2_RGB:         return "ETC2_RGB";      break;      ///< 4 bpp
        case TextureFormat::ETC2_EAC_RGBA:    return "ETC2_RGBA";     break;      ///< 8 bpp
        case TextureFormat::PVRT_RGB:         return "PVRT_RGB";      break;      ///< 4 bpp
        case TextureFormat::PVRT_RGBA:        return "PVRT_RGBA";     break;      ///< 4 bpp
        case TextureFormat::ASTC_4x4_RGBA:    return "ASTC_4x4_RGBA"; break;      ///< 8 bpp
        case TextureFormat::ASTC_8x8_RGBA:    return "ASTC_8x8_RGBA"; break;      ///< 2 bpp
        default:                            return "UNKNOWN";       break;
    }
}

// Get pixel data size in bytes (image or texture)
// NOTE: Size depends on pixel format
int gl::GetPixelDataSize(int width, int height, TextureFormat format)
{
    int dataSize = 0;       // Size in bytes
    int bpp = 0;            // Bits per pixel

    switch (format)
    {
        case TextureFormat::Grayscale:        bpp = 8; break;
        case TextureFormat::GrayAlpha:
        case TextureFormat::RGB565:
        case TextureFormat::RGBA5551:
        case TextureFormat::RGBA4444:         bpp = 16; break;
        case TextureFormat::RGBA8888:         bpp = 32; break;
        case TextureFormat::RGB888:           bpp = 24; break;
        case TextureFormat::R32:              bpp = 32; break;
        case TextureFormat::RGB323232:        bpp = 32*3; break;
        case TextureFormat::RGBA32323232:     bpp = 32*4; break;
        case TextureFormat::R16:              bpp = 16; break;
        case TextureFormat::RGB161616:        bpp = 16*3; break;
        case TextureFormat::RGBA16161616:     bpp = 16*4; break;
        case TextureFormat::DXT1_RGB:
        case TextureFormat::DXT1_RGBA:
        case TextureFormat::ETC1_RGB:
        case TextureFormat::ETC2_RGB:
        case TextureFormat::PVRT_RGB:
        case TextureFormat::PVRT_RGBA:        bpp = 4; break;
        case TextureFormat::DXT3_RGBA:
        case TextureFormat::DXT5_RGBA:
        case TextureFormat::ETC2_EAC_RGBA:
        case TextureFormat::ASTC_4x4_RGBA:    bpp = 8; break;
        case TextureFormat::ASTC_8x8_RGBA:    bpp = 2; break;
        default: break;
    }

    dataSize = width*height*bpp/8;  // Total data size in bytes

    // Most compressed formats works on 4x4 blocks,
    // if texture is smaller, minimum dataSize is 8 or 16
    if ((width < 4) && (height < 4))
    {
        if ((format >= TextureFormat::DXT1_RGB) && (format < TextureFormat::DXT3_RGBA)) dataSize = 8;
        else if ((format >= TextureFormat::DXT3_RGBA) && (format < TextureFormat::ASTC_8x8_RGBA)) dataSize = 16;
    }

    return dataSize;
}

// Get OpenGL internal formats and data type from gl::TextureFormat
void gl::GetGlTextureFormats(gl::TextureFormat format, Uint32 *glInternalFormat, Uint32 *glFormat, Uint32 *glType)
{
    *glInternalFormat = 0;
    *glFormat = 0;
    *glType = 0;

    switch (format)
    {
#   if defined(GRAPHICS_API_OPENGL_11) || defined(GRAPHICS_API_OPENGL_21) || defined(GRAPHICS_API_OPENGL_ES2)
        // NOTE: on OpenGL ES 2.0 (WebGL), internalFormat must match format and options allowed are: GL_LUMINANCE, GL_RGB, GL_RGBA
        case TextureFormat::Grayscale: *glInternalFormat = GL_LUMINANCE; *glFormat = GL_LUMINANCE; *glType = GL_UNSIGNED_BYTE; break;
        case TextureFormat::GrayAlpha: *glInternalFormat = GL_LUMINANCE_ALPHA; *glFormat = GL_LUMINANCE_ALPHA; *glType = GL_UNSIGNED_BYTE; break;
        case TextureFormat::RGB565: *glInternalFormat = GL_RGB; *glFormat = GL_RGB; *glType = GL_UNSIGNED_SHORT_5_6_5; break;
        case TextureFormat::RGB888: *glInternalFormat = GL_RGB; *glFormat = GL_RGB; *glType = GL_UNSIGNED_BYTE; break;
        case TextureFormat::RGBA5551: *glInternalFormat = GL_RGBA; *glFormat = GL_RGBA; *glType = GL_UNSIGNED_SHORT_5_5_5_1; break;
        case TextureFormat::RGBA4444: *glInternalFormat = GL_RGBA; *glFormat = GL_RGBA; *glType = GL_UNSIGNED_SHORT_4_4_4_4; break;
        case TextureFormat::RGBA8888: *glInternalFormat = GL_RGBA; *glFormat = GL_RGBA; *glType = GL_UNSIGNED_BYTE; break;
        #if !defined(GRAPHICS_API_OPENGL_11)
        #if defined(GRAPHICS_API_OPENGL_ES3)
        case TextureFormat::R32: if (GetExtensions().texFloat32) *glInternalFormat = GL_R32F_EXT; *glFormat = GL_RED_EXT; *glType = GL_FLOAT; break;
        case TextureFormat::RGB323232: if (GetExtensions().texFloat32) *glInternalFormat = GL_RGB32F_EXT; *glFormat = GL_RGB; *glType = GL_FLOAT; break;
        case TextureFormat::RGBA32323232: if (GetExtensions().texFloat32) *glInternalFormat = GL_RGBA32F_EXT; *glFormat = GL_RGBA; *glType = GL_FLOAT; break;
        case TextureFormat::R16: if (GetExtensions().texFloat16) *glInternalFormat = GL_R16F_EXT; *glFormat = GL_RED_EXT; *glType = GL_HALF_FLOAT; break;
        case TextureFormat::RGB161616: if (GetExtensions().texFloat16) *glInternalFormat = GL_RGB16F_EXT; *glFormat = GL_RGB; *glType = GL_HALF_FLOAT; break;
        case TextureFormat::RGBA16161616: if (GetExtensions().texFloat16) *glInternalFormat = GL_RGBA16F_EXT; *glFormat = GL_RGBA; *glType = GL_HALF_FLOAT; break;
        #else
        case TextureFormat::R32: if (GetExtensions().texFloat32) *glInternalFormat = GL_LUMINANCE; *glFormat = GL_LUMINANCE; *glType = GL_FLOAT; break;            // NOTE: Requires extension OES_texture_float
        case TextureFormat::RGB323232: if (GetExtensions().texFloat32) *glInternalFormat = GL_RGB; *glFormat = GL_RGB; *glType = GL_FLOAT; break;                  // NOTE: Requires extension OES_texture_float
        case TextureFormat::RGBA32323232: if (GetExtensions().texFloat32) *glInternalFormat = GL_RGBA; *glFormat = GL_RGBA; *glType = GL_FLOAT; break;             // NOTE: Requires extension OES_texture_float
        #if defined(GRAPHICS_API_OPENGL_21)
        case TextureFormat::R16: if (GetExtensions().texFloat16) *glInternalFormat = GL_LUMINANCE; *glFormat = GL_LUMINANCE; *glType = GL_HALF_FLOAT_ARB; break;
        case TextureFormat::RGB161616: if (GetExtensions().texFloat16) *glInternalFormat = GL_RGB; *glFormat = GL_RGB; *glType = GL_HALF_FLOAT_ARB; break;
        case TextureFormat::RGBA16161616: if (GetExtensions().texFloat16) *glInternalFormat = GL_RGBA; *glFormat = GL_RGBA; *glType = GL_HALF_FLOAT_ARB; break;
        #else // defined(GRAPHICS_API_OPENGL_ES2)
        case TextureFormat::R16: if (GetExtensions().texFloat16) *glInternalFormat = GL_LUMINANCE; *glFormat = GL_LUMINANCE; *glType = GL_HALF_FLOAT_OES; break;   // NOTE: Requires extension OES_texture_half_float
        case TextureFormat::RGB161616: if (GetExtensions().texFloat16) *glInternalFormat = GL_RGB; *glFormat = GL_RGB; *glType = GL_HALF_FLOAT_OES; break;         // NOTE: Requires extension OES_texture_half_float
        case TextureFormat::RGBA16161616: if (GetExtensions().texFloat16) *glInternalFormat = GL_RGBA; *glFormat = GL_RGBA; *glType = GL_HALF_FLOAT_OES; break;    // NOTE: Requires extension OES_texture_half_float
        #endif
        #endif
        #endif
#   elif defined(GRAPHICS_API_OPENGL_33)
        case TextureFormat::Grayscale: *glInternalFormat = GL_R8; *glFormat = GL_RED; *glType = GL_UNSIGNED_BYTE; break;
        case TextureFormat::GrayAlpha: *glInternalFormat = GL_RG8; *glFormat = GL_RG; *glType = GL_UNSIGNED_BYTE; break;
        case TextureFormat::RGB565: *glInternalFormat = GL_RGB565; *glFormat = GL_RGB; *glType = GL_UNSIGNED_SHORT_5_6_5; break;
        case TextureFormat::RGB888: *glInternalFormat = GL_RGB8; *glFormat = GL_RGB; *glType = GL_UNSIGNED_BYTE; break;
        case TextureFormat::RGBA5551: *glInternalFormat = GL_RGB5_A1; *glFormat = GL_RGBA; *glType = GL_UNSIGNED_SHORT_5_5_5_1; break;
        case TextureFormat::RGBA4444: *glInternalFormat = GL_RGBA4; *glFormat = GL_RGBA; *glType = GL_UNSIGNED_SHORT_4_4_4_4; break;
        case TextureFormat::RGBA8888: *glInternalFormat = GL_RGBA8; *glFormat = GL_RGBA; *glType = GL_UNSIGNED_BYTE; break;
        case TextureFormat::R32: if (GetExtensions().texFloat32) *glInternalFormat = GL_R32F; *glFormat = GL_RED; *glType = GL_FLOAT; break;
        case TextureFormat::RGB323232: if (GetExtensions().texFloat32) *glInternalFormat = GL_RGB32F; *glFormat = GL_RGB; *glType = GL_FLOAT; break;
        case TextureFormat::RGBA32323232: if (GetExtensions().texFloat32) *glInternalFormat = GL_RGBA32F; *glFormat = GL_RGBA; *glType = GL_FLOAT; break;
        case TextureFormat::R16: if (GetExtensions().texFloat16) *glInternalFormat = GL_R16F; *glFormat = GL_RED; *glType = GL_HALF_FLOAT; break;
        case TextureFormat::RGB161616: if (GetExtensions().texFloat16) *glInternalFormat = GL_RGB16F; *glFormat = GL_RGB; *glType = GL_HALF_FLOAT; break;
        case TextureFormat::RGBA16161616: if (GetExtensions().texFloat16) *glInternalFormat = GL_RGBA16F; *glFormat = GL_RGBA; *glType = GL_HALF_FLOAT; break;
#   endif
#   if !defined(GRAPHICS_API_OPENGL_11)
        case TextureFormat::DXT1_RGB: if (GetExtensions().texCompDXT) *glInternalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; break;
        case TextureFormat::DXT1_RGBA: if (GetExtensions().texCompDXT) *glInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
        case TextureFormat::DXT3_RGBA: if (GetExtensions().texCompDXT) *glInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
        case TextureFormat::DXT5_RGBA: if (GetExtensions().texCompDXT) *glInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
        case TextureFormat::ETC1_RGB: if (GetExtensions().texCompETC1) *glInternalFormat = GL_ETC1_RGB8_OES; break;                      // NOTE: Requires OpenGL ES 2.0 or OpenGL 4.3
        case TextureFormat::ETC2_RGB: if (GetExtensions().texCompETC2) *glInternalFormat = GL_COMPRESSED_RGB8_ETC2; break;               // NOTE: Requires OpenGL ES 3.0 or OpenGL 4.3
        case TextureFormat::ETC2_EAC_RGBA: if (GetExtensions().texCompETC2) *glInternalFormat = GL_COMPRESSED_RGBA8_ETC2_EAC; break;     // NOTE: Requires OpenGL ES 3.0 or OpenGL 4.3
        case TextureFormat::PVRT_RGB: if (GetExtensions().texCompPVRT) *glInternalFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG; break;    // NOTE: Requires PowerVR GPU
        case TextureFormat::PVRT_RGBA: if (GetExtensions().texCompPVRT) *glInternalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG; break;  // NOTE: Requires PowerVR GPU
        case TextureFormat::ASTC_4x4_RGBA: if (GetExtensions().texCompASTC) *glInternalFormat = GL_COMPRESSED_RGBA_ASTC_4x4_KHR; break;  // NOTE: Requires OpenGL ES 3.1 or OpenGL 4.3
        case TextureFormat::ASTC_8x8_RGBA: if (GetExtensions().texCompASTC) *glInternalFormat = GL_COMPRESSED_RGBA_ASTC_8x8_KHR; break;  // NOTE: Requires OpenGL ES 3.1 or OpenGL 4.3
#   endif
        default: NEXUS_LOG(Warning) << "[gl::GetGlTextureFormats] Current format not supported (" << static_cast<int>(format) << ")\n"; break;
    }
}

#if defined(SHOW_GL_DETAILS_INFO)
// Get compressed format official GL identifier name
const char *gl::GetCompressedFormatName(int format)
{
    switch (format)
    {
        // GL_EXT_texture_compression_s3tc
        case 0x83F0: return "GL_COMPRESSED_RGB_S3TC_DXT1_EXT"; break;
        case 0x83F1: return "GL_COMPRESSED_RGBA_S3TC_DXT1_EXT"; break;
        case 0x83F2: return "GL_COMPRESSED_RGBA_S3TC_DXT3_EXT"; break;
        case 0x83F3: return "GL_COMPRESSED_RGBA_S3TC_DXT5_EXT"; break;
        // GL_3DFX_texture_compression_FXT1
        case 0x86B0: return "GL_COMPRESSED_RGB_FXT1_3DFX"; break;
        case 0x86B1: return "GL_COMPRESSED_RGBA_FXT1_3DFX"; break;
        // GL_IMG_texture_compression_pvrtc
        case 0x8C00: return "GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG"; break;
        case 0x8C01: return "GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG"; break;
        case 0x8C02: return "GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG"; break;
        case 0x8C03: return "GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG"; break;
        // GL_OES_compressed_ETC1_RGB8_texture
        case 0x8D64: return "GL_ETC1_RGB8_OES"; break;
        // GL_ARB_texture_compression_rgtc
        case 0x8DBB: return "GL_COMPRESSED_RED_RGTC1"; break;
        case 0x8DBC: return "GL_COMPRESSED_SIGNED_RED_RGTC1"; break;
        case 0x8DBD: return "GL_COMPRESSED_RG_RGTC2"; break;
        case 0x8DBE: return "GL_COMPRESSED_SIGNED_RG_RGTC2"; break;
        // GL_ARB_texture_compression_bptc
        case 0x8E8C: return "GL_COMPRESSED_RGBA_BPTC_UNORM_ARB"; break;
        case 0x8E8D: return "GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB"; break;
        case 0x8E8E: return "GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB"; break;
        case 0x8E8F: return "GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB"; break;
        // GL_ARB_ES3_compatibility
        case 0x9274: return "GL_COMPRESSED_RGB8_ETC2"; break;
        case 0x9275: return "GL_COMPRESSED_SRGB8_ETC2"; break;
        case 0x9276: return "GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2"; break;
        case 0x9277: return "GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2"; break;
        case 0x9278: return "GL_COMPRESSED_RGBA8_ETC2_EAC"; break;
        case 0x9279: return "GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC"; break;
        case 0x9270: return "GL_COMPRESSED_R11_EAC"; break;
        case 0x9271: return "GL_COMPRESSED_SIGNED_R11_EAC"; break;
        case 0x9272: return "GL_COMPRESSED_RG11_EAC"; break;
        case 0x9273: return "GL_COMPRESSED_SIGNED_RG11_EAC"; break;
        // GL_KHR_texture_compression_astc_hdr
        case 0x93B0: return "GL_COMPRESSED_RGBA_ASTC_4x4_KHR"; break;
        case 0x93B1: return "GL_COMPRESSED_RGBA_ASTC_5x4_KHR"; break;
        case 0x93B2: return "GL_COMPRESSED_RGBA_ASTC_5x5_KHR"; break;
        case 0x93B3: return "GL_COMPRESSED_RGBA_ASTC_6x5_KHR"; break;
        case 0x93B4: return "GL_COMPRESSED_RGBA_ASTC_6x6_KHR"; break;
        case 0x93B5: return "GL_COMPRESSED_RGBA_ASTC_8x5_KHR"; break;
        case 0x93B6: return "GL_COMPRESSED_RGBA_ASTC_8x6_KHR"; break;
        case 0x93B7: return "GL_COMPRESSED_RGBA_ASTC_8x8_KHR"; break;
        case 0x93B8: return "GL_COMPRESSED_RGBA_ASTC_10x5_KHR"; break;
        case 0x93B9: return "GL_COMPRESSED_RGBA_ASTC_10x6_KHR"; break;
        case 0x93BA: return "GL_COMPRESSED_RGBA_ASTC_10x8_KHR"; break;
        case 0x93BB: return "GL_COMPRESSED_RGBA_ASTC_10x10_KHR"; break;
        case 0x93BC: return "GL_COMPRESSED_RGBA_ASTC_12x10_KHR"; break;
        case 0x93BD: return "GL_COMPRESSED_RGBA_ASTC_12x12_KHR"; break;
        case 0x93D0: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR"; break;
        case 0x93D1: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR"; break;
        case 0x93D2: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR"; break;
        case 0x93D3: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR"; break;
        case 0x93D4: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR"; break;
        case 0x93D5: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR"; break;
        case 0x93D6: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR"; break;
        case 0x93D7: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR"; break;
        case 0x93D8: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR"; break;
        case 0x93D9: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR"; break;
        case 0x93DA: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR"; break;
        case 0x93DB: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR"; break;
        case 0x93DC: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR"; break;
        case 0x93DD: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR"; break;
        default: return "GL_COMPRESSED_UNKNOWN"; break;
    }
}
#endif  // SHOW_GL_DETAILS_INFO
