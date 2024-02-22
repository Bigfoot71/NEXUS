#ifndef NEXUS_GL_UTILS_HPP
#define NEXUS_GL_UTILS_HPP

#include "./nxEnums.hpp"

namespace nexus { namespace gl {

    NEXUS_API Version GetVersion();  

    NEXUS_API const char* GetTextureFormatName(TextureFormat format);                                                             // Get current OpenGL version
    NEXUS_API int GetPixelDataSize(int width, int height, TextureFormat format);                                                  // Get pixel data size in bytes (image or texture)
    NEXUS_API void GetGlTextureFormats(TextureFormat format, Uint32 *glInternalFormat, Uint32 *glFormat, Uint32 *glType);   // Get OpenGL internal formats

#   if (defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)) && defined(SHOW_GL_DETAILS_INFO)
    NEXUS_API const char *GetCompressedFormatName(int format); // Get compressed format official GL identifier name
#   endif  // RLGL_SHOW_GL_DETAILS_INFO

}}

#endif //NEXUS_GL_UTILS_HPP
