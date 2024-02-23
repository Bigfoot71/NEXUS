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

#ifndef NEXUS_GL_EXTENSIONS_HPP
#define NEXUS_GL_EXTENSIONS_HPP

#include "../../platform/nxPlatform.hpp"

#if defined(GRAPHICS_API_OPENGL_ES2) && !defined(GRAPHICS_API_OPENGL_ES3)

// NOTE: VAO functionality is exposed through extensions (OES)
extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;

// NOTE: Instancing functionality could also be available through extension
extern PFNGLDRAWARRAYSINSTANCEDEXTPROC glDrawArraysInstanced;
extern PFNGLDRAWELEMENTSINSTANCEDEXTPROC glDrawElementsInstanced;
extern PFNGLVERTEXATTRIBDIVISOREXTPROC glVertexAttribDivisor;

#endif

namespace nexus { namespace gl {

    struct NEXUS_API Extensions
    {
        bool vao            = false;                ///< VAO support (OpenGL ES2 could not support VAO extension) (GL_ARB_vertex_array_object)
        bool instancing     = false;                ///< Instancing supported (GL_ANGLE_instanced_arrays, GL_EXT_draw_instanced + GL_EXT_instanced_arrays)
        bool texNPOT        = false;                ///< NPOT textures full support (GL_ARB_texture_non_power_of_two, GL_OES_texture_npot)
        bool texDepth       = false;                ///< Depth textures supported (GL_ARB_depth_texture, GL_OES_depth_texture)
        bool texDepthWebGL  = false;                ///< Depth textures supported WebGL specific (GL_WEBGL_depth_texture)
        bool texFloat32     = false;                ///< float textures support (32 bit per channel) (GL_OES_texture_float)
        bool texFloat16     = false;                ///< half float textures support (16 bit per channel) (GL_OES_texture_half_float)
        bool texCompDXT     = false;                ///< DDS texture compression support (GL_EXT_texture_compression_s3tc, GL_WEBGL_compressed_texture_s3tc, GL_WEBKIT_WEBGL_compressed_texture_s3tc)
        bool texCompETC1    = false;                ///< ETC1 texture compression support (GL_OES_compressed_ETC1_RGB8_texture, GL_WEBGL_compressed_texture_etc1)
        bool texCompETC2    = false;                ///< ETC2/EAC texture compression support (GL_ARB_ES3_compatibility)
        bool texCompPVRT    = false;                ///< PVR texture compression support (GL_IMG_texture_compression_pvrtc)
        bool texCompASTC    = false;                ///< ASTC texture compression support (GL_KHR_texture_compression_astc_hdr, GL_KHR_texture_compression_astc_ldr)
        bool texMirrorClamp = false;                ///< Clamp mirror wrap mode supported (GL_EXT_texture_mirror_clamp)
        bool texAnisoFilter = false;                ///< Anisotropic texture filtering support (GL_EXT_texture_filter_anisotropic)
        bool computeShader  = false;                ///< Compute shaders support (GL_ARB_compute_shader)
        bool ssbo           = false;                ///< Shader storage buffer object support (GL_ARB_shader_storage_buffer_object)

        float maxAnisotropyLevel = 0;               ///< Maximum anisotropy level supported (minimum is 2.0f)
        int maxDepthBits         = 0;               ///< Maximum bits for depth component
    };

    /**
     * @brief Check if OpenGL extensions have been loaded.
     *
     * This function indicates whether OpenGL extensions have been successfully loaded.
     *
     * @return True if OpenGL extensions are loaded, false otherwise.
     */
    NEXUS_API bool IsExtensionsLoaded();

    /**
     * @brief Get the available OpenGL extensions.
     *
     * This function returns the available OpenGL extensions as a list.
     *
     * @return A structure containing information about the available OpenGL extensions.
     */
    NEXUS_API const Extensions& GetExtensions();

    /**
     * @brief Load OpenGL extensions using a custom loader function.
     *
     * This function loads OpenGL extensions using a custom loader function provided as a parameter.
     *
     * @param loader A function pointer to the loader that loads the OpenGL extensions.
     */
    NEXUS_API void LoadExtensions(void *loader(const char *));

}}

#endif //NEXUS_GL_EXTENSIONS_HPP