#ifndef NEXUS_GAPI_IMPL_SP_MODEL_MESH_HPP
#define NEXUS_GAPI_IMPL_SP_MODEL_MESH_HPP

#include "./nxMaterial.hpp"

#include "../../../utils/nxContextual.hpp"
#include "../../../core/nxException.hpp"
#include "../../../shape/3D/nxMesh.hpp"
#include "../../../gfx/nxSurface.hpp"
#include "../../../gfx/nxColor.hpp"
#include "../../../math/nxMath.hpp"
#include "../../../math/nxVec4.hpp"
#include "../../../math/nxVec3.hpp"
#include "../../../math/nxVec2.hpp"
#include "../../../core/nxLog.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <unordered_map>
#include <utility>
#include <vector>
#include <limits>
#include <array>

namespace _gapi_impl {

    /**
     * @brief Bone information stored in Model but needed by Mesh during loading
     * and when updating vertices during animations.
     */
    struct BoneInfo
    {
        nexus::math::Mat4 offset;                  ///< Offset matrix transforms vertex from model space to bone space
        nexus::math::Mat4 finalTransformation;     ///< Final transformation of the bone during animation

        /**
         * @brief Constructs a BoneInfo object.
         * @param o The offset matrix.
         * @param ft The final transformation matrix.
         */
        BoneInfo(const nexus::math::Mat4& o = nexus::math::Mat4(), const nexus::math::Mat4& ft = nexus::math::Mat4())
        : offset(o), finalTransformation(ft)
        { }
    };

    /**
     * @brief Structure representing bone data associated with a vertex.
     */
    struct VertexBoneData
    {
        static constexpr Uint8 MaxBoneInfluence = 4;

        std::array<int, MaxBoneInfluence> boneIDs;          ///< Bone indices influencing the corresponding vertex (VertexBoneData[i] -> influences -> vertices[i])
        std::array<float, MaxBoneInfluence> weights;        ///< Weights from each bone influencing the vertex

        /**
         * @brief Default constructor for VertexBoneData.
         */
        VertexBoneData();

        /**
         * @brief Adds bone data to the vertex.
         * @param boneID The index of the bone.
         * @param weight The weight of influence from the bone.
         */
        void AddBoneData(int boneID, float weight);

        /**
         * @brief Normalizes the weights of bone influence.
         */
        void Normalize();
    };

    /**
     * @brief Implementation class for Mesh
     *
     * @note: This class does not directly use T_Context but is required by its implementation-derived classes for rendering
     *       and for encapsulation within nexus::utils::Container<T_Contextual>. This also reduces the amount of code as we do not need to override constructors in
     *       implementation-derived classes.
     *
     * @note: The expected T_Material is a contextual object encapsulated within a container and not a direct contextual object.
     *
     * @tparam T_Context The context type.
     * @tparam T_Material The material type.
     */
    template <typename T_Context, typename T_Material>
    struct Mesh : nexus::shape3D::Mesh, nexus::utils::Contextual<T_Context>
    {
      public:
        using Material = T_Material;

      private:
        // Alias for base class members
        using nexus::shape3D::Mesh::Polygon;
        using nexus::shape3D::Mesh::Plane;
        using nexus::shape3D::Mesh::Cube;
        using nexus::shape3D::Mesh::Sphere;
        using nexus::shape3D::Mesh::HemiSphere;
        using nexus::shape3D::Mesh::Cylinder;
        using nexus::shape3D::Mesh::Cone;
        using nexus::shape3D::Mesh::Torus;
        using nexus::shape3D::Mesh::TrefoilKnot;
        using nexus::shape3D::Mesh::Heightmap;
        using nexus::shape3D::Mesh::Cubicmap;

      public:
        // Animation vertex data (copy of original for animation transforms)
        std::vector<nexus::math::Vec3> animVertices;   ///< Animated vertex positions (after bone transformations)
        std::vector<nexus::math::Vec3> animNormals;    ///< Animated normals (after bone transformations)
        std::vector<VertexBoneData> bones;             ///< Bone influences for each vertex

      public:
        /**
         * @brief Static generation functions.
         */
        static Mesh Polygon(T_Context& ctx, int sides, float radius);
        static Mesh Plane(T_Context& ctx, float width, float length, int resX, int resZ);
        static Mesh Cube(T_Context& ctx, float width, float height, float length);
        static Mesh Sphere(T_Context& ctx, float radius, int rings, int slices);
        static Mesh HemiSphere(T_Context& ctx, float radius, int rings, int slices);
        static Mesh Cylinder(T_Context& ctx, float radius, float height, int slices);
        static Mesh Cone(T_Context& ctx, float radius, float height, int slices);
        static Mesh Torus(T_Context& ctx, float radius, float size, int radSeg, int sides);
        static Mesh TrefoilKnot(T_Context& ctx, float radius, float size, int radSeg, int sides);
        static Mesh Heightmap(T_Context& ctx, const nexus::gfx::Surface& heightmap, const nexus::math::Vec3& size);
        static Mesh Cubicmap(T_Context& ctx, const nexus::gfx::Surface& cubicmap, const nexus::math::Vec3& cubeSize);

      private:
        // Private constructor used internally to move generated vertices by shape3D::Mesh static generation functions
        Mesh(T_Context& ctx, nexus::shape3D::Mesh&& mesh);

      public:
        /**
         * @brief Constructor that initializes an empty mesh that the user can generate.
         * @param ctx The context.
         */
        Mesh(T_Context& ctx);

        /**
         * @brief Constructor reserved for the Model class for loading a mesh from an Assimp structure.
         * @param ctx The context.
         * @param mesh The Assimp mesh.
         * @param boneIDMap A map of bone names and their transformation information.
         * @param boneInfos Information about the bones.
         */
        Mesh(T_Context& ctx, const aiMesh* mesh, std::unordered_map<std::string, int>& boneIDMap, std::vector<BoneInfo>& boneInfos);

        // NOTE: Mesh copy is currently impossible
        //Mesh(const Mesh&) = delete;
        //Mesh operator=(const Mesh&) = delete;

        /**
         * @brief Mesh move constructor.
         */
        Mesh(Mesh&&) noexcept;

        /**
         * @brief Mesh move assignment operator.
         */
        Mesh& operator=(Mesh&&) noexcept;

        /**
         * @brief Move constructor used during mesh generation by contextual containers.
         * @note: This constructor could also potentially be used for moving meshes (?).
         * @param ctx The context.
         * @param other The other mesh.
         */
        Mesh(T_Context& ctx, Mesh<T_Context, T_Material>&& other) noexcept;

        /**
         * @brief Virtual default destructor.
         */
        virtual ~Mesh() = default;

        /**
         * @brief Function reserved for gl::Model.
         *        Updates vertex transformations for animation.
         * @param boneInfos Information about the bones.
         * @return True if one or more vertices have been updated.
         */
        bool UpdateAnimation(std::vector<BoneInfo>& boneInfos);
    };


    /* Private Implementation Mesh */

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material>::Mesh(T_Context& ctx, nexus::shape3D::Mesh&& mesh)
    : nexus::utils::Contextual<T_Context>(ctx)
    , nexus::shape3D::Mesh(std::move(mesh))
    { }


    /* Public Implementation Mesh */

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material>::Mesh(T_Context& ctx)
    : nexus::utils::Contextual<T_Context>(ctx)
    { }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material>::Mesh(T_Context& ctx, const aiMesh* mesh, std::unordered_map<std::string, int>& boneIDMap, std::vector<BoneInfo>& boneInfos)
    : nexus::utils::Contextual<T_Context>(ctx)
    {
        numVertices = mesh->mNumVertices;
        numFaces = mesh->mNumFaces;

        vertices.reserve(numVertices);

        if (mesh->HasTextureCoords(0)) texcoords.reserve(numVertices);
        if (mesh->HasNormals()) normals.reserve(numVertices);

        if (mesh->HasTextureCoords(1)) texcoords2.reserve(numVertices);
        if (mesh->HasTangentsAndBitangents()) tangents.reserve(numVertices);
        if (mesh->HasVertexColors(0)) colors.reserve(numVertices);

        for (int i = 0; i < numVertices; i++)
        {
            vertices.push_back(mesh->mVertices[i]);

            if (texcoords.capacity()) texcoords.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            if (normals.capacity()) normals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

            if (texcoords2.capacity()) texcoords2.emplace_back(mesh->mTextureCoords[1][i].x, mesh->mTextureCoords[1][i].y);
            if (tangents.capacity()) tangents.emplace_back(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 0.0f);
            if (colors.capacity()) colors.emplace_back(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a);
        }

        indices.reserve(numVertices);
        for (Uint32 i = 0; i < mesh->mNumFaces; i++)
        {
            for (Uint32 j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            {
                indices.emplace_back(mesh->mFaces[i].mIndices[j]);
            }
        }

        if (mesh->HasBones())
        {
            bones.resize(numVertices);
            animVertices = vertices;
            animNormals = normals;

            for (Uint32 i = 0; i < mesh->mNumBones; i++)
            {
                const aiBone *bone = mesh->mBones[i];
                if (bone->mNumWeights == 0) continue;

                int boneID = -1;

                if (boneIDMap.find(bone->mName.C_Str()) == boneIDMap.end())
                {
                    boneID = boneInfos.size();
                    boneIDMap.emplace(bone->mName.C_Str(), boneID);
                    boneInfos.emplace_back(bone->mOffsetMatrix);
                }
                else
                {
                    boneID = boneIDMap[bone->mName.C_Str()];
                }

                for (Uint32 j = 0; j < mesh->mBones[i]->mNumWeights; j++)
                {
                    const Uint32 vertexID = bone->mWeights[j].mVertexId;
                    bones[vertexID].AddBoneData(boneID, bone->mWeights[j].mWeight);
                }
            }

            for (auto& bone : bones)
            {
                bone.Normalize();
            }
        }
    }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material>::Mesh(Mesh&& other) noexcept
    : nexus::utils::Contextual<T_Context>(other.ctx)
    , nexus::shape3D::Mesh(other)
    , animVertices(std::move(other.animVertices))
    , animNormals(std::move(other.animNormals))
    , bones(std::move(other.bones))
    { }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material>& Mesh<T_Context, T_Material>::operator=(Mesh&& other) noexcept
    {
        if (this != &other)
        {
            numVertices   = std::exchange(other.numVertices, 0);
            numFaces      = std::exchange(other.numFaces, 0);
            vertices      = std::move(other.vertices);
            texcoords     = std::move(other.texcoords);
            normals       = std::move(other.normals);
            colors        = std::move(other.colors);
            tangents      = std::move(other.tangents);
            texcoords2    = std::move(other.texcoords2);
            indices       = std::move(other.indices);
            animVertices  = std::move(other.animVertices);
            animNormals   = std::move(other.animNormals);
            bones         = std::move(other.bones);
        }
        return *this;
    }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material>::Mesh(T_Context& ctx, Mesh<T_Context, T_Material>&& other) noexcept
    : nexus::utils::Contextual<T_Context>(ctx)
    {
        numVertices   = std::exchange(other.numVertices, 0);
        numFaces      = std::exchange(other.numFaces, 0);
        vertices      = std::move(other.vertices);
        texcoords     = std::move(other.texcoords);
        normals       = std::move(other.normals);
        colors        = std::move(other.colors);
        tangents      = std::move(other.tangents);
        texcoords2    = std::move(other.texcoords2);
        indices       = std::move(other.indices);
        animVertices  = std::move(other.animVertices);
        animNormals   = std::move(other.animNormals);
        bones         = std::move(other.bones);
    }

    template <typename T_Context, typename T_Material>
    bool Mesh<T_Context, T_Material>::UpdateAnimation(std::vector<BoneInfo>& boneInfos)
    {
        bool vertsUpdated = false;

        for (Uint32 i = 0; i < numVertices; i++)
        {
            const VertexBoneData &bone = bones[i];
            nexus::math::Mat4 transform;

            for (Uint8 j = 0; j < VertexBoneData::MaxBoneInfluence; j++)
            {
                if (bone.boneIDs[j] == -1) break;
                if (bone.weights[j] == 0.0f) continue;
                transform += boneInfos[bone.boneIDs[j]].finalTransformation * bone.weights[j];
            }

            if (transform != nexus::math::Mat4()) // TODO: Optimize this
            {
                animVertices[i] = vertices[i].Transformed(transform);

                const nexus::math::Vec3& n = normals[i];
                const nexus::math::Vec4 norm = transform * nexus::math::Vec4(n.x, n.y, n.z, 0.0f);
                animNormals[i] = { norm.x, norm.y, norm.z };

                vertsUpdated = true;
            }
        }

        return vertsUpdated;
    }


    /* Public Implementation Mesh Generation */

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material> Mesh<T_Context, T_Material>::Polygon(T_Context& ctx, int sides, float radius)
    {
        return Mesh(ctx, nexus::shape3D::Mesh::Polygon(sides, radius));
    }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material> Mesh<T_Context, T_Material>::Plane(T_Context& ctx, float width, float length, int resX, int resZ)
    {
        return Mesh(ctx, nexus::shape3D::Mesh::Plane(width, length, resX, resZ));
    }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material> Mesh<T_Context, T_Material>::Cube(T_Context& ctx, float width, float height, float length)
    {
        return Mesh(ctx, nexus::shape3D::Mesh::Cube(width, height, length));
    }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material> Mesh<T_Context, T_Material>::Sphere(T_Context& ctx, float radius, int rings, int slices)
    {
        return Mesh(ctx, nexus::shape3D::Mesh::Sphere(radius, rings, slices));
    }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material> Mesh<T_Context, T_Material>::HemiSphere(T_Context& ctx, float radius, int rings, int slices)
    {
        return Mesh(ctx, nexus::shape3D::Mesh::HemiSphere(radius, rings, slices));
    }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material> Mesh<T_Context, T_Material>::Cylinder(T_Context& ctx, float radius, float height, int slices)
    {
        return Mesh(ctx, nexus::shape3D::Mesh::Cylinder(radius, height, slices));
    }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material> Mesh<T_Context, T_Material>::Cone(T_Context& ctx, float radius, float height, int slices)
    {
        return Mesh(ctx, nexus::shape3D::Mesh::Cone(radius, height, slices));
    }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material> Mesh<T_Context, T_Material>::Torus(T_Context& ctx, float radius, float size, int radSeg, int sides)
    {
        return Mesh(ctx, nexus::shape3D::Mesh::Torus(radius, size, radSeg, sides));
    }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material> Mesh<T_Context, T_Material>::TrefoilKnot(T_Context& ctx, float radius, float size, int radSeg, int sides)
    {
        return Mesh(ctx, nexus::shape3D::Mesh::TrefoilKnot(radius, size, radSeg, sides));
    }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material> Mesh<T_Context, T_Material>::Heightmap(T_Context& ctx, const nexus::gfx::Surface& heightmap, const nexus::math::Vec3& size)
    {
        return Mesh(ctx, nexus::shape3D::Mesh::Heightmap(heightmap, size));
    }

    template <typename T_Context, typename T_Material>
    Mesh<T_Context, T_Material> Mesh<T_Context, T_Material>::Cubicmap(T_Context& ctx, const nexus::gfx::Surface& cubicmap, const nexus::math::Vec3& cubeSize)
    {
        return Mesh(ctx, nexus::shape3D::Mesh::Cubicmap(cubicmap, cubeSize));
    }

}

#endif //NEXUS_GAPI_IMPL_SP_MODEL_MESH_HPP
