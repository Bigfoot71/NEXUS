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
