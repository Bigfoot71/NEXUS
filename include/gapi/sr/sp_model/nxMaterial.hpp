#ifndef NEXUS_SR_MATERIAL_3D_HPP
#define NEXUS_SR_MATERIAL_3D_HPP

#include "../../cmn_impl/sp_model/nxMaterial.hpp"
#include "../../../math/nxMath.hpp"
#include "../../../math/nxVec3.hpp"
#include "../../../math/nxVec2.hpp"
#include "../../../gfx/nxColor.hpp"
#include "../../../core/nxText.hpp"
#include "../../../core/nxLog.hpp"
#include "../nxContextual.hpp"
#include "../nxTexture.hpp"
#include "../nxContext.hpp"

namespace _sr_impl {

    /**
     * @brief Alias for Material type using Software Rasterizer specific types.
     */
    using Material = _gapi_impl::Material<nexus::sr::Context, nexus::sr::Shader, nexus::sr::Texture>;

}

namespace nexus { namespace sr {

    /**
     * @brief Represents a material in Software Rasterizer rendering.
     *
     * The Material class encapsulates functionality related to SR materials,
     * including textures and shaders.
     */
    class NEXUS_API Material : public Container<_sr_impl::Material>
    {
      public:
        static constexpr auto MapCount = _sr_impl::Material::MapCount;    ///< Number of texture maps.
        using MapType = _sr_impl::Material::MapType;                      ///< Type of texture maps.

      public:
        /**
         * @brief Default constructor for Material.
         * @param ctx The Rasterizer context.
         */
        Material(nexus::sr::Context& ctx)
        : Container<_sr_impl::Material>(ctx)
        { }

        /**
         * @brief Constructor for Material loading from Assimp data.
         * @param ctx The Rasterizer context.
         * @param material Pointer to the Assimp material data.
         * @param scene Pointer to the Assimp scene data.
         */
        Material(nexus::sr::Context& ctx, const aiMaterial* material, const aiScene* scene)
        : Container<_sr_impl::Material>(ctx, material, scene)
        { }
    };

}}

#endif //NEXUS_SR_MATERIAL_3D_HPP
