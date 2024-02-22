#ifndef NEXUS_SR_MESH_HPP
#define NEXUS_SR_MESH_HPP

#include "./nxMaterial.hpp"

#include "../../cmn_impl/sp_model/nxModel.hpp"
#include "../../cmn_impl/sp_model/nxMesh.hpp"
#include "../../../gfx/nxSurface.hpp"
#include "../../../math/nxMath.hpp"
#include "../../../math/nxVec3.hpp"
#include "../../../math/nxVec2.hpp"
#include "../../../gfx/nxColor.hpp"
#include "../../../core/nxLog.hpp"
#include "../nxContextual.hpp"
#include "../nxContext.hpp"
#include "../nxContext.hpp"

namespace nexus { namespace sr {
    class Mesh;
}}

namespace _sr_impl {

    class Mesh : public _gapi_impl::Mesh<nexus::sr::Context, nexus::sr::Material>
    {
      private:
        friend class _gapi_impl::Model<nexus::sr::Context, nexus::sr::Mesh, nexus::sr::Material>;

      public:
        using _gapi_impl::Mesh<nexus::sr::Context, nexus::sr::Material>::Mesh;

      public:
        void Draw(nexus::sr::Material& material, const nexus::math::Mat4& transform);
    };
}

namespace nexus { namespace sr {

    class NEXUS_API Mesh : public Container<_sr_impl::Mesh>
    {
        friend class _gapi_impl::Model<Context, Mesh, Material>;

      public:
        using VertexBoneData = _gapi_impl::VertexBoneData;

      protected:
        // Utiliser en interne par cette même class pour déplacer des _gapi_impl::Mesh,
        // génénré par les fonctions static de génération, vers le containeur contextuel
        template <typename T_Material>
        Mesh(_gapi_impl::Mesh<Context, T_Material>&& other) noexcept
        : Container(other.ctx, std::move(other))
        { }

      public:
        static Mesh Polygon(Context& ctx, int sides, float radius)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Polygon(ctx, sides, radius));
        }

        static Mesh Plane(Context& ctx, float width, float length, int resX, int resZ)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Plane(ctx, width, length, resX, resZ));
        }

        static Mesh Cube(Context& ctx, float width, float height, float length)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Cube(ctx, width, height, length));
        }

        static Mesh Sphere(Context& ctx, float radius, int rings, int slices)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Sphere(ctx, radius, rings, slices));
        }

        static Mesh HemiSphere(Context& ctx, float radius, int rings, int slices)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::HemiSphere(ctx, radius, rings, slices));
        }

        static Mesh Cylinder(Context& ctx, float radius, float height, int slices)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Cylinder(ctx, radius, height, slices));
        }

        static Mesh Cone(Context& ctx, float radius, float height, int slices)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Cone(ctx, radius, height, slices));
        }

        static Mesh Torus(Context& ctx, float radius, float size, int radSeg, int sides)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Torus(ctx, radius, size, radSeg, sides));
        }

        static Mesh TrefoilKnot(Context& ctx, float radius, float size, int radSeg, int sides)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::TrefoilKnot(ctx, radius, size, radSeg, sides));
        }

        static Mesh Heightmap(Context& ctx, const nexus::gfx::Surface& heightmap, const nexus::math::Vec3& size)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Heightmap(ctx, heightmap, size));
        }

        static Mesh Cubicmap(Context& ctx, const nexus::gfx::Surface& cubicmap, const nexus::math::Vec3& cubeSize)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Cubicmap(ctx, cubicmap, cubeSize));
        }

      public:
        // Constructeur reservé à gl::Model
        // NOTE: Ne peut pas être privé actuellement car utilisé via std::vector::emplace_back(...) dans gl::Model
        Mesh(Context& ctx, const aiMesh* mesh, std::unordered_map<std::string, int>& boneIDMap, std::vector<_gapi_impl::BoneInfo>& boneInfos)
        : Container(ctx, mesh, boneIDMap, boneInfos)
        { }
    };

}}

#endif //NEXUS_SR_MESH_HPP
