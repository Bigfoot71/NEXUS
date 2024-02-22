#ifndef NEXUS_SR_MODEL_HPP
#define NEXUS_SR_MODEL_HPP

#include "./nxMaterial.hpp"
#include "./nxMesh.hpp"

#include "../../cmn_impl/sp_model/nxModel.hpp"
#include "../../../core/nxException.hpp"
#include "../../../math/nxMath.hpp"
#include "../../../math/nxMat4.hpp"
#include "../../../gfx/nxColor.hpp"
#include "../../../core/nxLog.hpp"
#include "../nxContext.hpp"
#include "../nxTexture.hpp"

namespace _sr_impl
{
    class Model : public _gapi_impl::Model<nexus::sr::Context, nexus::sr::Mesh, nexus::sr::Material>
    {
      public:
        using _gapi_impl::Model<nexus::sr::Context, nexus::sr::Mesh, nexus::sr::Material>::Model;

        void Draw(const nexus::math::Vec3& position, const nexus::math::Vec3& rotationAxis, float rotationAngle, const nexus::math::Vec3& scale, const nexus::gfx::Color& tint);
        void Draw(const nexus::math::Vec3& position, float scale, const nexus::gfx::Color& tint);

        void DrawWires(const nexus::math::Vec3& position, float scale, const nexus::gfx::Color& tint);
        void DrawWires(const nexus::math::Vec3& position, const nexus::math::Vec3& rotationAxis, float rotationAngle, const nexus::math::Vec3& scale, const nexus::gfx::Color& tint);
    };
}

namespace nexus { namespace sr {

    class NEXUS_API Model : public Container<_sr_impl::Model>
    {
      public:
        // NOTE: 'assetPath' indicates the directory where we must load the material assets
        // If no 'assetPath' is indicated, the 'filePath' directory will be retained
        Model(Context& ctx, const std::string& filePath, bool loadAllAnimations = false, std::string assetPath = "")
        : Container(ctx, filePath, loadAllAnimations, assetPath)
        { }

        // Charge un model complet dont les données bruts sont déjà presnetes en mémoire
        Model(Context& ctx, const std::vector<Uint8>& data, bool loadAllAnimations = false, const std::string& assetPath = "")
        : Container(ctx, data, loadAllAnimations, assetPath)
        { }

        // Charge le maillage donné avec un material par defaut, le model et le material créé auront le meme context que le maillage fourni
        Model(Context& ctx, const Mesh& mesh)
        : Container(ctx, mesh)
        { }
    };

}}

#endif //NEXUS_SR_MODEL_HPP
