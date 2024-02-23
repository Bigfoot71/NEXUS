#ifndef NEXUS_GL_MESH_HPP
#define NEXUS_GL_MESH_HPP

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
#include "./nxMaterial.hpp"

namespace nexus { namespace gl {
    class Mesh;
}}

namespace _gl_impl {

    /**
     * @brief Represents a mesh in OpenGL rendering.
     *
     * The Mesh class encapsulates functionality related to OpenGL meshes,
     * including vertex buffer objects, vertex array objects, and drawing methods.
     */
    class Mesh : public _gapi_impl::Mesh<nexus::gl::Context, nexus::gl::Material>
    {
        friend class _gapi_impl::Model<nexus::gl::Context, nexus::gl::Mesh, nexus::gl::Material>;

      protected:
        std::array<Uint32, 7> vboId{};      ///< OpenGL Vertex Buffer Objects id (default vertex data)
        Uint32 vaoId = 0;                   ///< OpenGL Vertex Array Object id (if available)

      protected:
        // Function reserved for _gapi_impl::Model
        // Allows updating vertex transformations
        void UpdateAnimation(std::vector<_gapi_impl::BoneInfo>& boneInfos);

      public:
        using _gapi_impl::Mesh<nexus::gl::Context, nexus::gl::Material>::Mesh;

        ~Mesh();

      public:
        /**
         * @brief Checks if the mesh data is uploaded to the GPU.
         * @return True if the mesh data is uploaded, false otherwise.
         */
        bool IsUploaded() const { return vaoId != 0; }

        /**
         * @brief Uploads the mesh data to the GPU.
         * @param dynamic True if the data is dynamic and may change frequently, false otherwise.
         */
        void Upload(bool dynamic);

        /**
         * @brief Updates the mesh data on the GPU.
         * @param buffer The buffer index to update.
         */
        void Update(Uint8 buffer);

        /**
         * @brief Generates tangents for the mesh.
         * @param upload True if the tangents should be uploaded to the GPU, false otherwise.
         */
        void GenTangents(bool upload = true);

        /**
         * @brief Draws the mesh with a specified material and transformation.
         * @param material The material to use for rendering.
         * @param transform The transformation matrix to apply to the mesh.
         */
        void Draw(const nexus::gl::Material& material, const nexus::math::Mat4& transform);

        /**
         * @brief Draws multiple instances of the mesh with a specified material and transformations.
         * @param material The material to use for rendering.
         * @param transforms The list of transformation matrices to apply to each instance of the mesh.
         */
        void Draw(const nexus::gl::Material& material, const std::vector<nexus::math::Mat4>& transforms); // draw multiple instanced
    };

}

namespace nexus { namespace gl {

    /**
     * @brief Represents a mesh in OpenGL rendering.
     *
     * The Mesh class encapsulates functionality related to OpenGL meshes,
     * including vertex buffer objects, vertex array objects, and drawing methods.
     */
    class NEXUS_API Mesh : public Container<_gl_impl::Mesh>
    {
        friend class _gapi_impl::Model<Context, Mesh, Material>;

      public:
        using VertexBoneData = _gapi_impl::VertexBoneData;

      protected:
        /**
         * @brief Constructor for Mesh used internally to move _gapi_impl::Mesh instances.
         * @param other The _gapi_impl::Mesh instance to move.
         */
        template <typename T_Material>
        Mesh(_gapi_impl::Mesh<Context, T_Material>&& other) noexcept
        : Container(other.ctx, std::move(other))
        { }

      public:
        /**
         * @brief Creates a polygonal mesh.
         * @param ctx The OpenGL context.
         * @param sides The number of sides of the polygon.
         * @param radius The radius of the polygon.
         * @return The created Mesh object.
         */
        static Mesh Polygon(Context& ctx, int sides, float radius)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Polygon(ctx, sides, radius));
        }

        /**
         * @brief Creates a planar mesh.
         * @param ctx The OpenGL context.
         * @param width The width of the plane.
         * @param length The length of the plane.
         * @param resX The resolution along the X-axis.
         * @param resZ The resolution along the Z-axis.
         * @return The created Mesh object.
         */
        static Mesh Plane(Context& ctx, float width, float length, int resX, int resZ)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Plane(ctx, width, length, resX, resZ));
        }

        /**
         * @brief Creates a cubic mesh.
         * @param ctx The OpenGL context.
         * @param width The width of the cube.
         * @param height The height of the cube.
         * @param length The length of the cube.
         * @return The created Mesh object.
         */
        static Mesh Cube(Context& ctx, float width, float height, float length)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Cube(ctx, width, height, length));
        }

        /**
         * @brief Creates a spherical mesh.
         * @param ctx The OpenGL context.
         * @param radius The radius of the sphere.
         * @param rings The number of rings.
         * @param slices The number of slices.
         * @return The created Mesh object.
         */
        static Mesh Sphere(Context& ctx, float radius, int rings, int slices)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Sphere(ctx, radius, rings, slices));
        }

        /**
         * @brief Creates a hemispherical mesh.
         * @param ctx The OpenGL context.
         * @param radius The radius of the hemisphere.
         * @param rings The number of rings.
         * @param slices The number of slices.
         * @return The created Mesh object.
         */
        static Mesh HemiSphere(Context& ctx, float radius, int rings, int slices)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::HemiSphere(ctx, radius, rings, slices));
        }

        /**
         * @brief Creates a cylindrical mesh.
         * @param ctx The OpenGL context.
         * @param radius The radius of the cylinder.
         * @param height The height of the cylinder.
         * @param slices The number of slices.
         * @return The created Mesh object.
         */
        static Mesh Cylinder(Context& ctx, float radius, float height, int slices)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Cylinder(ctx, radius, height, slices));
        }

        /**
         * @brief Creates a conical mesh.
         * @param ctx The OpenGL context.
         * @param radius The radius of the cone.
         * @param height The height of the cone.
         * @param slices The number of slices.
         * @return The created Mesh object.
         */
        static Mesh Cone(Context& ctx, float radius, float height, int slices)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Cone(ctx, radius, height, slices));
        }

        /**
         * @brief Creates a toroidal mesh (torus).
         * @param ctx The OpenGL context.
         * @param radius The radius of the torus.
         * @param size The size of the torus.
         * @param radSeg The number of radial segments.
         * @param sides The number of sides.
         * @return The created Mesh object.
         */
        static Mesh Torus(Context& ctx, float radius, float size, int radSeg, int sides)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Torus(ctx, radius, size, radSeg, sides));
        }

        /**
         * @brief Creates a mesh representing a trefoil knot.
         * @param ctx The OpenGL context.
         * @param radius The radius of the trefoil knot.
         * @param size The size of the trefoil knot.
         * @param radSeg The number of radial segments.
         * @param sides The number of sides.
         * @return The created Mesh object.
         */
        static Mesh TrefoilKnot(Context& ctx, float radius, float size, int radSeg, int sides)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::TrefoilKnot(ctx, radius, size, radSeg, sides));
        }

        /**
         * @brief Creates a mesh from a heightmap.
         * @param ctx The OpenGL context.
         * @param heightmap The heightmap surface.
         * @param size The size of the mesh.
         * @return The created Mesh object.
         */
        static Mesh Heightmap(Context& ctx, const nexus::gfx::Surface& heightmap, const nexus::math::Vec3& size)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Heightmap(ctx, heightmap, size));
        }

        /**
         * @brief Creates a mesh from a cubic map.
         * @param ctx The OpenGL context.
         * @param cubicmap The cubic map surface.
         * @param cubeSize The size of the cube.
         * @return The created Mesh object.
         */
        static Mesh Cubicmap(Context& ctx, const nexus::gfx::Surface& cubicmap, const nexus::math::Vec3& cubeSize)
        {
            return Mesh(_gapi_impl::Mesh<Context, Material>::Cubicmap(ctx, cubicmap, cubeSize));
        }

      public:
        /**
         * @brief Constructor reserved for gl::Model.
         *
         * @note: Cannot be private currently because used via std::vector::emplace_back(...) in gl::Model
         *
         * @param ctx The OpenGL context.
         * @param mesh Pointer to the Assimp mesh data.
         * @param boneIDMap The map of bone IDs.
         * @param boneInfos Vector containing bone information.
         */
        Mesh(Context& ctx, const aiMesh* mesh, std::unordered_map<std::string, int>& boneIDMap, std::vector<_gapi_impl::BoneInfo>& boneInfos)
        : Container(ctx, mesh, boneIDMap, boneInfos)
        { }
    };

}}

#endif //NEXUS_GL_MESH_HPP
