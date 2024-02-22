#ifndef NEXUS_GAPI_IMPL_SP_MODEL_HPP
#define NEXUS_GAPI_IMPL_SP_MODEL_HPP

#include "./nxMaterial.hpp"
#include "./nxMesh.hpp"

#include "../../../utils/nxContextual.hpp"
#include "../../../core/nxException.hpp"
#include "../../../shape/3D/nxAABB.hpp"
#include "../../../math/nxMath.hpp"
#include "../../../math/nxMat4.hpp"
#include "../../../gfx/nxColor.hpp"
#include "../../../core/nxLog.hpp"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <unordered_map>
#include <type_traits>
#include <utility>
#include <cassert>
#include <vector>

namespace _gapi_impl {

    /**
     * @brief Class representing a bone in an animated skeleton.
     */
    class Bone
    {
      private:
        template <typename T> struct KeyFrame
        {
            T value; float timeStamp;
            KeyFrame(const T& v, float t)
                : value(v), timeStamp(t) { }
        };

      private:
        std::vector<KeyFrame<nexus::math::Vec3>> positions;     ///< Keyframes for position
        std::vector<KeyFrame<nexus::math::Vec4>> rotations;     ///< Keyframes for rotation
        std::vector<KeyFrame<nexus::math::Vec3>> scales;        ///< Keyframes for scaling
        nexus::math::Mat4 localTransform;                       ///< Local transformation matrix
        int id;                                                 ///< Bone ID

      public:
        /**
         * @brief Constructor for Bone.
         * @param boneID The ID of the bone.
         * @param channel The Assimp node animation channel.
         */
        Bone(int boneID, const aiNodeAnim* channel);

        /**
         * @brief Move constructor for Bone.
         */
        Bone(Bone&& other) noexcept;

        /**
         * @brief Move assignment operator for Bone.
         */
        Bone& operator=(Bone&& other) noexcept;
    
      private:
        /**
         * @brief Interpolates between position keys and returns the translation matrix.
         * @param animationTime The current time of the animation.
         * @return The interpolated translation matrix.
         */
        nexus::math::Mat4 InterpolatePosition(float animationTime);

        /**
         * @brief Interpolates between rotation keys and returns the rotation matrix.
         * @param animationTime The current time of the animation.
         * @return The interpolated rotation matrix.
         */
        nexus::math::Mat4 InterpolateRotation(float animationTime);

        /**
         * @brief Interpolates between scaling keys and returns the scale matrix.
         * @param animationTime The current time of the animation.
         * @return The interpolated scale matrix.
         */
        nexus::math::Mat4 InterpolateScaling(float animationTime);

        /**
         * @brief Gets the current index on positions to interpolate based on the current animation time.
         * @param animationTime The current time of the animation.
         * @return The index on positions to interpolate.
         */
        int GetPositionIndex(float animationTime);

        /**
         * @brief Gets the current index on rotations to interpolate based on the current animation time.
         * @param animationTime The current time of the animation.
         * @return The index on rotations to interpolate.
         */
        int GetRotationIndex(float animationTime);

        /**
         * @brief Gets the current index on scalings to interpolate based on the current animation time.
         * @param animationTime The current time of the animation.
         * @return The index on scalings to interpolate.
         */
        int GetScaleIndex(float animationTime);

      public:
        /**
         * @brief Interpolates between position, rotation, and scaling keys based on the current time of the animation
         *        and prepares the local transformation matrix by combining all keys transformations.
         * @param animationTime The current time of the animation.
         */
        void Update(float animationTime);

        /**
         * @brief Gets the local transformation matrix of the bone.
         * @return The local transformation matrix.
         */
        const nexus::math::Mat4& GetLocalTransform() const
        {
            return localTransform;
        }

        /**
         * @brief Gets the ID of the bone.
         * @return The bone ID.
         */
        int GetBoneID() const
        {
            return id;
        }
    };

    // Forward declaration of Model class for Animation class
    template <typename T_Context, typename T_Mesh, typename T_Material>
    class Model;

    /**
     * @brief Internal implementation class for animation management within the Model class.
     * @tparam T_Context The context type.
     * @tparam T_Mesh The mesh type.
     * @tparam T_Material The material type.
     */
    template <typename T_Context, typename T_Mesh, typename T_Material>
    class Animation
    {
      public:
        /**
         * @brief Structure representing node data in the animation hierarchy.
         */
        struct NodeData
        {
            std::string name;                      ///< Name of the node
            nexus::math::Mat4 transformation;      ///< Transformation matrix of the node
            std::vector<NodeData> children;        ///< Children nodes
        };

      private:
        float duration;                             ///< Duration of the animation
        int ticksPerSecond;                         ///< Ticks per second for the animation
        std::unordered_map<std::string, Bone> bones;///< Map of bone names to Bone objects

        NodeData rootNode;                          ///< Root node of the animation hierarchy

      private:
        /**
         * @brief Reads missing bones from the animation and updates the model.
         * @param animation The Assimp animation.
         * @param model Pointer to the model.
         */
        void ReadMissingBones(const aiAnimation* animation, Model<T_Context, T_Mesh, T_Material>* model);

        /**
         * @brief Reads node hierarchy recursively.
         * @param dest Destination node data.
         * @param src Source Assimp node.
         */
        void ReadNodeHierarchy(NodeData& dest, const aiNode* src);

      public:
        /**
         * @brief Default constructor for Animation.
         */
        Animation() = default;

        /**
         * @brief Constructor for Animation.
         * @param animation The Assimp animation.
         * @param scene The Assimp scene.
         * @param model Pointer to the model.
         */
        Animation(const aiAnimation* animation, const aiScene* scene, Model<T_Context, T_Mesh, T_Material>* model);

        /**
         * @brief Move constructor for Animation.
         */
        Animation(Animation&& other) noexcept;

        /**
         * @brief Move assignment operator for Animation.
         */
        Animation& operator=(Animation&& other) noexcept;

        /**
         * @brief Gets the ticks per second of the animation.
         * @return The ticks per second.
         */
        float GetTicksPerSecond() { return ticksPerSecond; }

        /**
         * @brief Gets the duration of the animation.
         * @return The duration.
         */
        float GetDuration() { return duration; }

        /**
         * @brief Gets the root node of the animation hierarchy.
         * @return The root node.
         */
        const NodeData& GetRootNode() { return rootNode; }

        /**
         * @brief Gets a pointer to the Bone object with the specified name.
         * @param boneName The name of the bone.
         * @return A pointer to the Bone object, or nullptr if not found.
         */
        Bone* GetBone(const std::string& boneName)
        {
            auto it = bones.find(boneName);
            if (it == bones.end()) return nullptr;
            return &(it->second);
        }
    };

    /**
     * @brief Implementation class for Model, which is a contextual object encapsulated within a container. 
     *        However, the Mesh and Material objects it takes will be containers and not direct contextual objects.
     * @tparam T_Context The context type.
     * @tparam T_Mesh The mesh container type.
     * @tparam T_Material The material container type.
     */
    template <typename T_Context, typename T_Mesh, typename T_Material>
    class Model : public nexus::utils::Contextual<T_Context>
    {
      protected:
        static_assert(nexus::utils::IsContainer<T_Mesh>::value,
            "[Model] <T_Mesh> must be derived from a context container.");

        static_assert(nexus::utils::IsContainer<T_Material>::value,
            "[Model] <T_Material> must be derived from a context container.");

      protected:
        friend class _gapi_impl::Animation<T_Context, T_Mesh, T_Material>;

      public:
        using Mesh = T_Mesh;
        using Material = T_Material;

      protected:
        nexus::math::Mat4 globalInverseTransform;
        std::unordered_map<std::string, _gapi_impl::Animation<T_Context, T_Mesh, T_Material>> animations;
        std::unordered_map<std::string, int> boneIDMap;
        std::vector<BoneInfo> boneInfos;

      public:
        nexus::math::Mat4 transform;                   ///< Local transform matrix
        std::vector<T_Mesh> meshes;                    ///< Array of meshes
        std::vector<T_Material> materials;             ///< Array of materials
        std::vector<int> meshMaterial;                 ///< Mesh material number

      protected:
        // Transforms the bone/node hierarchy recursively according to the given animation time
        void BoneTransform(Animation<T_Context, T_Mesh, T_Material>& animation, float currentTime, const typename Animation<T_Context, T_Mesh, T_Material>::NodeData& node, const nexus::math::Mat4& parentTransform);

      public:
        /**
         * @brief Constructor for Model.
         * @param ctx The context.
         * @param filePath The file path of the model.
         * @param loadAllAnimations Indicates whether to load all animations.
         * @param assetPath The asset path for loading materials.
         */
        Model(T_Context& ctx, const std::string& filePath, bool loadAllAnimations = false, std::string assetPath = "");

        /**
         * @brief Constructor for Model.
         * @param ctx The context.
         * @param data The raw model data.
         * @param loadAllAnimations Indicates whether to load all animations.
         * @param assetPath The asset path for loading materials.
         */
        Model(T_Context& ctx, const std::vector<Uint8>& data, bool loadAllAnimations = false, const std::string& assetPath = "");

        /**
         * @brief Constructor for Model.
         * @param ctx The context.
         * @param mesh The mesh container.
         */
        Model(T_Context& ctx, const T_Mesh& mesh);

        /**
         * @brief Loads animations from a file or files.
         * @param filePath The file path or paths for loading animations.
         * @param animationNames The names of animations to load.
         */
        void LoadAnimations(const std::string& filePath, const std::string& animationNames = "");

        /**
         * @brief Unloads animations.
         * @param animationNames The names of animations to unload.
         */
        void UnloadAnimations(const std::string& animationNames);

        /**
         * @brief Updates the animation frame according to the specified animation and time.
         * @param animationName The name of the animation.
         * @param currentTime The current time of the animation.
         */
        void UpdateAnimation(const std::string& animationName, float currentTime);

        /**
         * @brief Gets the axis-aligned bounding box (AABB) of the model.
         * @return The AABB of the model.
         * @warning The current BoundingBox structure design does not support rotation transformations. 
         *          In those cases, it is up to the user to calculate the proper box bounds (8 vertices transformed).
         */
        nexus::shape3D::AABB GetAABB();
    };


    /* Private Implementation Animation */

    template <typename T_Context, typename T_Mesh, typename T_Material>
    void Animation<T_Context, T_Mesh, T_Material>::ReadMissingBones(const aiAnimation* animation, Model<T_Context, T_Mesh, T_Material>* model)
    {
        for (int i = 0; i < animation->mNumChannels; i++)
        {
            const aiNodeAnim *channel = animation->mChannels[i];
            const std::string &boneName = channel->mNodeName.C_Str();

            if (model->boneIDMap.find(boneName) == model->boneIDMap.end()) // maybe add a new bone with identity transforms (?) (condition never seems true)
            {
                int boneID = model->boneInfos.size();
                model->boneIDMap.emplace(boneName, boneID);
                model->boneInfos.push_back({ nexus::math::Mat4::Identity(), nexus::math::Mat4::Identity() });
            }

            bones.emplace(boneName, Bone(model->boneIDMap[boneName], channel));
        }
    }

    template <typename T_Context, typename T_Mesh, typename T_Material>
    void Animation<T_Context, T_Mesh, T_Material>::ReadNodeHierarchy(NodeData& dest, const aiNode* src)
    {
        dest.name = src->mName.data;
        dest.transformation = src->mTransformation;

        dest.children.reserve(src->mNumChildren);
        for (int i = 0; i < src->mNumChildren; i++)
        {
            NodeData newData;
            ReadNodeHierarchy(newData, src->mChildren[i]);
            dest.children.push_back(newData);
        }
    }

    template <typename T_Context, typename T_Mesh, typename T_Material>
    Animation<T_Context, T_Mesh, T_Material>::Animation(const aiAnimation* animation, const aiScene* scene, Model<T_Context, T_Mesh, T_Material>* model)
    {
        duration = animation->mDuration;
        ticksPerSecond = animation->mTicksPerSecond;
        ReadNodeHierarchy(rootNode, scene->mRootNode);
        ReadMissingBones(animation, model);
    }

    template <typename T_Context, typename T_Mesh, typename T_Material>
    Animation<T_Context, T_Mesh, T_Material>::Animation(Animation&& other) noexcept
    : duration(std::exchange(other.duration, 0))
    , ticksPerSecond(std::exchange(other.ticksPerSecond, 0))
    , bones(std::move(other.bones))
    , rootNode(std::move(other.rootNode))
    { }

    template <typename T_Context, typename T_Mesh, typename T_Material>
    Animation<T_Context, T_Mesh, T_Material>& Animation<T_Context, T_Mesh, T_Material>::operator=(Animation&& other) noexcept
    {
        if (this != &other)
        {
            duration = std::exchange(other.duration, 0);
            ticksPerSecond = std::exchange(other.ticksPerSecond, 0);
            bones = std::move(other.bones);
            rootNode = std::move(other.rootNode);
        }
        return *this;
    }


    /* Private Implementation Model */

    template <typename T_Context, typename T_Mesh, typename T_Material>
    void Model<T_Context, T_Mesh, T_Material>::BoneTransform(Animation<T_Context, T_Mesh, T_Material>& animation, float currentTime, const typename Animation<T_Context, T_Mesh, T_Material>::NodeData& node, const nexus::math::Mat4& parentTransform)
    {
        nexus::math::Mat4 globalTransform;
        Bone *bone = animation.GetBone(node.name);

        if (bone)
        {
            bone->Update(currentTime);
            globalTransform = bone->GetLocalTransform() * parentTransform;

            BoneInfo &boneInfo = boneInfos[bone->GetBoneID()];
            boneInfo.finalTransformation = boneInfo.offset * globalTransform * globalInverseTransform;
        }
        else
        {
            globalTransform = node.transformation * parentTransform;
        }

        for (const auto& child : node.children)
        {
            BoneTransform(animation, currentTime, child, globalTransform);
        }
    }


    /* Public Implementation Model */

    template <typename T_Context, typename T_Mesh, typename T_Material>
    Model<T_Context, T_Mesh, T_Material>::Model(T_Context& ctx, const std::string& filePath, bool loadAllAnimations, std::string assetPath)
    : nexus::utils::Contextual<T_Context>(ctx), transform(nexus::math::Mat4::Identity())
    {
        // Open model file with Assimp
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            throw nexus::core::NexusException(
                "Model", "Unable to load model",
                "Assimp", importer.GetErrorString());
        }

        // Set working directory to load related assets
        const std::string previousWorkingDir = nexus::core::GetWorkingDirectory();
        nexus::core::SetWorkingDirectory(assetPath.empty() ? nexus::core::GetDirectoryPath(filePath) : assetPath);

        // Default global inverse transformation
        globalInverseTransform = scene->mRootNode->mTransformation.Inverse();

        // Load Materials
        materials.reserve(scene->mNumMaterials);
        for (Uint32 i = 0; i < scene->mNumMaterials; i++)
        {
            materials.emplace_back(ctx, scene->mMaterials[i], scene);
        }

        // Load Meshes and Bones for Model
        for (Uint32 i = 0; i < scene->mNumMeshes; i++)
        {
            const aiMesh* mesh = scene->mMeshes[i];
            meshes.emplace_back(ctx, mesh, boneIDMap, boneInfos);
            meshMaterial.push_back(mesh->mMaterialIndex);
        }

        // Load animations if requested
        if (loadAllAnimations && scene->HasAnimations())
        {
            animations.reserve(scene->mNumAnimations);
            for (Uint32 i = 0; i < scene->mNumAnimations; i++)
            {
                animations.emplace(scene->mAnimations[i]->mName.C_Str(), Animation(scene->mAnimations[i], scene, this));
            }
        }

        // Restore to the original working directory
        nexus::core::SetWorkingDirectory(previousWorkingDir);
    }

    template <typename T_Context, typename T_Mesh, typename T_Material>
    Model<T_Context, T_Mesh, T_Material>::Model(T_Context& ctx, const std::vector<Uint8>& data, bool loadAllAnimations, const std::string& assetPath)
    : nexus::utils::Contextual<T_Context>(ctx), transform(nexus::math::Mat4::Identity())
    {
        // Set working directory to load related assets
        const std::string previousWorkingDir = nexus::core::GetWorkingDirectory();
        if (assetPath.empty()) nexus::core::SetWorkingDirectory(assetPath);

        // Open model file with Assimp
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFileFromMemory(data.data(), data.size(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            throw nexus::core::NexusException(
                "Model", "Unable to load model",
                "Assimp", importer.GetErrorString());
        }

        // Default global inverse transformation
        globalInverseTransform = scene->mRootNode->mTransformation.Inverse();

        // Load Materials
        materials.reserve(scene->mNumMaterials);
        for (Uint32 i = 0; i < scene->mNumMaterials; i++)
        {
            materials.emplace_back(ctx, scene->mMaterials[i], scene);
        }

        // Load Meshes and Bones for Model
        for (Uint32 i = 0; i < scene->mNumMeshes; i++)
        {
            const aiMesh* mesh = scene->mMeshes[i];
            meshes.emplace_back(ctx, mesh, boneIDMap, boneInfos);
            meshMaterial.push_back(mesh->mMaterialIndex);
        }

        // Load animations if requested
        if (loadAllAnimations && scene->HasAnimations())
        {
            animations.reserve(scene->mNumAnimations);
            for (Uint32 i = 0; i < scene->mNumAnimations; i++)
            {
                animations.emplace(scene->mAnimations[i]->mName.C_Str(), Animation(scene->mAnimations[i], scene, this));
            }
        }

        // Restore to the original working directory
        nexus::core::SetWorkingDirectory(previousWorkingDir);
    }

    template <typename T_Context, typename T_Mesh, typename T_Material>
    Model<T_Context, T_Mesh, T_Material>::Model(T_Context& ctx, const T_Mesh& mesh)
    : nexus::utils::Contextual<T_Context>(ctx), transform(nexus::math::Mat4::Identity())
    {
        if (!mesh.HasSameContext(ctx))
        {
            throw nexus::core::NexusException("Model",
                "The context given to the model is not the same as that of the mesh.");
        }

        meshes.push_back(mesh);
        meshMaterial.emplace_back(0);
        materials.emplace_back(meshes.back().GetContext());    // Default material
    }

    template <typename T_Context, typename T_Mesh, typename T_Material>
    void Model<T_Context, T_Mesh, T_Material>::LoadAnimations(const std::string& filePath, const std::string& animationNames)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            throw nexus::core::NexusException(
                "Model", "Unable to load model animations",
                "Assimp", importer.GetErrorString());
        }

        if (!scene->HasAnimations())
        {
            NEXUS_LOG(Warning) << "The file [" << filePath << "] given to 'Model::LoadAnimations()' does not contain any animation.\n";
            return;
        }

        if (animationNames.empty())
        {
            for (Uint32 i = 0; i < scene->mNumAnimations; i++)
            {
                animations.emplace(scene->mAnimations[i]->mName.C_Str(), Animation(scene->mAnimations[i], scene, this));
            }
        }
        else // Loading only specified animations
        {
            std::stringstream stream(animationNames);
            std::string animationName;

            while(std::getline(stream, animationName, ';'))
            {
                bool animationFound = false;

                for (Uint32 i = 0; i < scene->mNumAnimations; i++)
                {
                    if (animationName == scene->mAnimations[i]->mName.C_Str())
                    {
                        animations.emplace(scene->mAnimations[i]->mName.C_Str(), Animation(scene->mAnimations[i], scene, this));
                        animationFound = true;
                        break;
                    }
                }

                if (!animationFound)
                {
                    NEXUS_LOG(Warning) << "Animation [" << animationName << "] not found in file [" << filePath << "]\n";
                }
            }
        }
    }

    template <typename T_Context, typename T_Mesh, typename T_Material>
    void Model<T_Context, T_Mesh, T_Material>::UnloadAnimations(const std::string& animationNames)
    {
        std::stringstream stream(animationNames);
        std::string animationName;

        while(std::getline(stream, animationName, ';'))
        {
            auto it = animations.find(animationName);

            if (it == animations.end())
            {
                NEXUS_LOG(Warning) << "Animation [" << animationName << "] to unload was not found.\n";
                continue;
            }

            animations.erase(it);
        }
    }

    template <typename T_Context, typename T_Mesh, typename T_Material>
    void Model<T_Context, T_Mesh, T_Material>::UpdateAnimation(const std::string& animationName, float currentTime)
    {
        auto it = animations.find(animationName);
        if (it == animations.end()) return;

        currentTime = std::fmod(currentTime * it->second.GetTicksPerSecond(), it->second.GetDuration());
        BoneTransform(it->second, currentTime, it->second.GetRootNode(), nexus::math::Mat4::Identity());

        for (auto& mesh : meshes)
        {
            mesh->UpdateAnimation(boneInfos);
        }
    }

    template <typename T_Context, typename T_Mesh, typename T_Material>
    nexus::shape3D::AABB Model<T_Context, T_Mesh, T_Material>::GetAABB()
    {
        nexus::shape3D::AABB bounds;
        nexus::math::Vec3 temp;

        for (const auto& mesh : meshes)
        {
            nexus::shape3D::AABB tempBounds = mesh->GetAABB();

            temp.x = (bounds.min.x < tempBounds.min.x) ? bounds.min.x : tempBounds.min.x;
            temp.y = (bounds.min.y < tempBounds.min.y) ? bounds.min.y : tempBounds.min.y;
            temp.z = (bounds.min.z < tempBounds.min.z) ? bounds.min.z : tempBounds.min.z;
            bounds.min = temp;

            temp.x = (bounds.max.x > tempBounds.max.x) ? bounds.max.x : tempBounds.max.x;
            temp.y = (bounds.max.y > tempBounds.max.y) ? bounds.max.y : tempBounds.max.y;
            temp.z = (bounds.max.z > tempBounds.max.z) ? bounds.max.z : tempBounds.max.z;
            bounds.max = temp;
        }

        // Apply model.transform to bounding box
        // WARNING: Current BoundingBox structure design does not support rotation transformations,
        // in those cases is up to the user to calculate the proper box bounds (8 vertices transformed)
        bounds.min = bounds.min.Transformed(transform);
        bounds.max = bounds.max.Transformed(transform);

        return bounds;
    }


    /* Struct to determine if a class is derived from _gapi_impl::Model at compile time */

    template <typename Derived>
    struct IsModel
    {
        template <typename T, typename U, typename V>
        static constexpr bool check(const Model<T, U, V>*)
        {
            return std::is_base_of<Model<T, U, V>, Derived>::value;
        }

        static constexpr bool value = check(static_cast<Derived*>(nullptr));
    };

}

#endif //NEXUS_GAPI_IMPL_SP_MODEL_HPP
