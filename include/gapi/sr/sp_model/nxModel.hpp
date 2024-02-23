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
    /**
     * @brief Model class implementation for Software Rasterizer rendering.
     *
     * The Model class represents a 3D model in SR rendering, composed of meshes and materials.
     */
    class Model : public _gapi_impl::Model<nexus::sr::Context, nexus::sr::Mesh, nexus::sr::Material>
    {
      public:
        // We use already existing constructors.
        using _gapi_impl::Model<nexus::sr::Context, nexus::sr::Mesh, nexus::sr::Material>::Model;

        /**
         * @brief Draws the model with specified transformation parameters and tint color.
         * @param position The position of the model.
         * @param rotationAxis The axis of rotation for the model.
         * @param rotationAngle The angle of rotation for the model.
         * @param scale The scale of the model.
         * @param tint The tint color of the model.
         */
        void Draw(const nexus::math::Vec3& position, const nexus::math::Vec3& rotationAxis, float rotationAngle, const nexus::math::Vec3& scale, const nexus::gfx::Color& tint);

        /**
         * @brief Draws the model with specified position, uniform scale, and tint color.
         * @param position The position of the model.
         * @param scale The uniform scale of the model.
         * @param tint The tint color of the model.
         */
        void Draw(const nexus::math::Vec3& position, float scale, const nexus::gfx::Color& tint);

        /**
         * @brief Draws the wireframe of the model with specified position, uniform scale, and tint color.
         * @param position The position of the model.
         * @param scale The uniform scale of the model.
         * @param tint The tint color of the wireframe.
         */
        void DrawWires(const nexus::math::Vec3& position, float scale, const nexus::gfx::Color& tint);

        /**
         * @brief Draws the wireframe of the model with specified transformation parameters and tint color.
         * @param position The position of the model.
         * @param rotationAxis The axis of rotation for the model.
         * @param rotationAngle The angle of rotation for the model.
         * @param scale The scale of the model.
         * @param tint The tint color of the wireframe.
         */
        void DrawWires(const nexus::math::Vec3& position, const nexus::math::Vec3& rotationAxis, float rotationAngle, const nexus::math::Vec3& scale, const nexus::gfx::Color& tint);
    };
}

namespace nexus { namespace sr {

    /**
     * @brief Model class representing a 3D model in Rasterizer rendering.
     *
     * The Model class encapsulates functionality related to loading and managing 3D models
     * in an SR context.
     */
    class NEXUS_API Model : public Container<_sr_impl::Model>
    {
      public:
        /**
         * @brief Constructor for loading a model from a file.
         * @param ctx The Rasterizer context.
         * @param filePath The path to the model file.
         * @param loadAllAnimations Flag to indicate whether to load all animations (default: false).
         * @param assetPath The directory for loading material assets (default: "").
         */
        Model(Context& ctx, const std::string& filePath, bool loadAllAnimations = false, std::string assetPath = "")
        : Container(ctx, filePath, loadAllAnimations, assetPath)
        { }

        /**
         * @brief Constructor for loading a model from memory.
         * @param ctx The Rasterizer context.
         * @param data Vector containing the model data.
         * @param loadAllAnimations Flag to indicate whether to load all animations (default: false).
         * @param assetPath The directory for loading material assets (default: "").
         */
        Model(Context& ctx, const std::vector<Uint8>& data, bool loadAllAnimations = false, const std::string& assetPath = "")
        : Container(ctx, data, loadAllAnimations, assetPath)
        { }

        /**
         * @brief Constructor for loading a model from a mesh.
         * @param ctx The Rasterizer context.
         * @param mesh The mesh representing the model.
         */
        Model(Context& ctx, const Mesh& mesh)
        : Container(ctx, mesh)
        { }
    };

}}

#endif //NEXUS_SR_MODEL_HPP
