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

    using Material = _gapi_impl::Material<nexus::sr::Context, nexus::sr::Shader, nexus::sr::Texture>;

}

namespace nexus { namespace sr {

    class NEXUS_API Material : public Container<_sr_impl::Material>
    {
      public:
        static constexpr auto MapCount = _sr_impl::Material::MapCount;
        using MapType = _sr_impl::Material::MapType;

      public:
        // Constructeur pour le Material par defaut (texture et shader par defaut)
        Material(nexus::sr::Context& ctx)
        : Container<_sr_impl::Material>(ctx)
        { }

        // Constructeur de chargement du material depuis les données chargées par Assimp
        Material(nexus::sr::Context& ctx, const aiMaterial* material, const aiScene* scene)
        : Container<_sr_impl::Material>(ctx, material, scene)
        { }
    };

}}

#endif //NEXUS_SR_MATERIAL_3D_HPP
