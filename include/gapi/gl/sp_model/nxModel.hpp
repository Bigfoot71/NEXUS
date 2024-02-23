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

#ifndef NEXUS_GL_MODEL_HPP
#define NEXUS_GL_MODEL_HPP

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
#include "../nxShader.hpp"

namespace _gl_impl
{
    /**
     * @brief Model class implementation for OpenGL rendering.
     *
     * The Model class represents a 3D model in OpenGL rendering, composed of meshes and materials.
     */
    class Model : public _gapi_impl::Model<nexus::gl::Context, nexus::gl::Mesh, nexus::gl::Material>
    {
      public:
        /**
         * @brief Constructor for loading a model from a file.
         * @param ctx The OpenGL context.
         * @param filePath The path to the model file.
         * @param loadAllAnimations Flag to indicate whether to load all animations.
         * @param assetPath The directory for loading material assets (optional).
         * @param upload Flag to indicate whether to upload meshes immediately.
         * @param dynamic Flag to indicate whether to create dynamic meshes.
         */
        Model(Context& ctx, const std::string& filePath, bool loadAllAnimations, std::string assetPath, bool upload, bool dynamic)
        : _gapi_impl::Model<nexus::gl::Context, nexus::gl::Mesh, nexus::gl::Material>(ctx, filePath, loadAllAnimations, assetPath)
        {
            if (upload) UploadMeshes(dynamic);
        }

        /**
         * @brief Constructor for loading a model from memory.
         * @param ctx The OpenGL context.
         * @param data Vector containing the model data.
         * @param loadAllAnimations Flag to indicate whether to load all animations.
         * @param assetPath The directory for loading material assets.
         * @param upload Flag to indicate whether to upload meshes immediately.
         * @param dynamic Flag to indicate whether to create dynamic meshes.
         */
        Model(Context& ctx, const std::vector<Uint8>& data, bool loadAllAnimations, const std::string& assetPath, bool upload, bool dynamic)
        : _gapi_impl::Model<nexus::gl::Context, nexus::gl::Mesh, nexus::gl::Material>(ctx, data, loadAllAnimations, assetPath)
        {
            if (upload) UploadMeshes(dynamic);
        }

        /**
         * @brief Constructor for loading a model from a mesh.
         * @param ctx The OpenGL context.
         * @param mesh The mesh representing the model.
         * @param upload Flag to indicate whether to upload meshes immediately.
         * @param dynamic Flag to indicate whether to create dynamic meshes.
         */
        Model(Context& ctx, const nexus::gl::Mesh& mesh, bool upload, bool dynamic)
        : _gapi_impl::Model<nexus::gl::Context, nexus::gl::Mesh, nexus::gl::Material>(ctx, mesh)
        {
            if (upload) UploadMeshes(dynamic);
        }

        /**
         * @brief Uploads all meshes of the model to the GPU.
         * @param dynamic Flag to indicate whether to create dynamic meshes.
         */
        void UploadMeshes(bool dynamic = false)
        {
            for (auto& mesh : meshes) mesh->Upload(dynamic);
        }

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

namespace nexus { namespace gl {

    /**
     * @brief Model class representing a 3D model in OpenGL rendering.
     *
     * The Model class encapsulates functionality related to loading and managing 3D models
     * in an OpenGL context.
     */
    class NEXUS_API Model : public Container<_gl_impl::Model>
    {
      public:
        /**
         * @brief Constructor for loading a model from a file.
         * @param ctx The OpenGL context.
         * @param filePath The path to the model file.
         * @param loadAllAnimations Flag to indicate whether to load all animations (default: false).
         * @param assetPath The directory for loading material assets (default: "").
         * @param upload Flag to indicate whether to upload meshes immediately (default: true).
         * @param dynamic Flag to indicate whether to create dynamic meshes (default: false).
         */
        Model(Context& ctx, const std::string& filePath, bool loadAllAnimations = false, std::string assetPath = "", bool upload = true, bool dynamic = false)
        : Container(ctx, filePath, loadAllAnimations, assetPath, upload, dynamic)
        { }

        /**
         * @brief Constructor for loading a model from memory.
         * @param ctx The OpenGL context.
         * @param data Vector containing the model data.
         * @param loadAllAnimations Flag to indicate whether to load all animations (default: false).
         * @param assetPath The directory for loading material assets (default: "").
         * @param upload Flag to indicate whether to upload meshes immediately (default: true).
         * @param dynamic Flag to indicate whether to create dynamic meshes (default: false).
         */
        Model(Context& ctx, const std::vector<Uint8>& data, bool loadAllAnimations = false, const std::string& assetPath = "", bool upload = true, bool dynamic = false)
        : Container(ctx, data, loadAllAnimations, assetPath, upload, dynamic)
        { }

        /**
         * @brief Constructor for loading a model from a mesh.
         * @param ctx The OpenGL context.
         * @param mesh The mesh representing the model.
         * @param upload Flag to indicate whether to upload meshes immediately (default: true).
         * @param dynamic Flag to indicate whether to create dynamic meshes (default: false).
         */
        Model(Context& ctx, const Mesh& mesh, bool upload = true, bool dynamic = false)
        : Container(ctx, mesh, upload, dynamic)
        { }
    };

}}

#endif //NEXUS_GL_MODEL_HPP
