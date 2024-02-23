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

#ifndef NEXUS_GL_MATERIAL_3D_HPP
#define NEXUS_GL_MATERIAL_3D_HPP

#include "../../cmn_impl/sp_model/nxMaterial.hpp"
#include "../../../math/nxMath.hpp"
#include "../../../math/nxVec3.hpp"
#include "../../../math/nxVec2.hpp"
#include "../../../gfx/nxColor.hpp"
#include "../../../core/nxText.hpp"
#include "../../../core/nxLog.hpp"
#include "../nxContext.hpp"
#include "../nxTexture.hpp"
#include "../nxShader.hpp"

namespace _gl_impl {

    /**
     * @brief Alias for Material type using OpenGL specific types.
     */
    using Material = _gapi_impl::Material<nexus::gl::Context, nexus::gl::Shader, nexus::gl::Texture>;

}

namespace nexus { namespace gl {

    /**
     * @brief Represents a material in OpenGL rendering.
     *
     * The Material class encapsulates functionality related to OpenGL materials,
     * including textures and shaders.
     */
    class NEXUS_API Material : public Container<_gl_impl::Material>
    {
      public:
        static constexpr auto MapCount = _gl_impl::Material::MapCount;    ///< Number of texture maps.
        using MapType = _gl_impl::Material::MapType;                      ///< Type of texture maps.

      public:
        /**
         * @brief Default constructor for Material.
         * @param ctx The OpenGL context.
         */
        Material(nexus::gl::Context& ctx)
        : Container<_gl_impl::Material>(ctx)
        { }

        /**
         * @brief Constructor for Material loading from Assimp data.
         * @param ctx The OpenGL context.
         * @param material Pointer to the Assimp material data.
         * @param scene Pointer to the Assimp scene data.
         */
        Material(nexus::gl::Context& ctx, const aiMaterial* material, const aiScene* scene)
        : Container<_gl_impl::Material>(ctx, material, scene)
        { }
    };

}}

#endif // NEXUS_GL_MATERIAL_3D_HPP
