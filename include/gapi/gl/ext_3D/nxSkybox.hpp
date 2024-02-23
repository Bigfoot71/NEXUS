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

#ifndef NEXUS_EXT_3D_GL_SKYBOX_HPP
#define NEXUS_EXT_3D_GL_SKYBOX_HPP

#include "../sp_model/nxModel.hpp"
#include "../nxTexture.hpp"
#include "../nxShader.hpp"

// TODO: Allow HDR texture loading, everything is already implemented except loading the file format

namespace nexus { namespace gl {

    /**
     * @brief Class for managing skybox rendering in OpenGL.
     */
    class NEXUS_API Skybox
    {
      private:
        static inline std::unique_ptr<Shader> shaderCubemapHDR;   ///< Shader for cubemap generation from an HDR panorama texture
        static inline std::unique_ptr<Model> model;               ///< Model of the skybox (contains cube mesh and shader)
        static inline int locEnvironementMap = -1;                ///< Location of the skybox shader in the model
        static inline int locVFlipped = -1;                       ///< Location of the V-flipped shader in the model
        static inline int locDoGamma = -1;                        ///< Location of the gamma shader in the model

      private:
        static inline Uint32 instanceCounter = 0;                 ///< Number of skybox instances created, to determine when to load global assets and when to unload them
        static inline Skybox *prevSkybox = nullptr;               ///< Indicates if the currently rendered skybox is not the same as the previous one, and data needs to be sent to the shader again.

      private:
        std::unique_ptr<Texture> texture;
        Uint32 skyboxID;
        bool useHDR;

      private:
        /**
         * @brief Generates cubemap texture from HDR panorama.
         *
         * @note: HDR is not supported by SDL_image, but at least
         *       it's already implemented here for the future.
         *
         * @param ctx The graphics context.
         * @param shader The shader used for cubemap generation.
         * @param panorama The HDR panorama surface.
         */
        void GenTextureCubemapHDR(Context& ctx, Shader& shader, const gfx::Surface& panorama);

      public:
        /**
         * @brief Constructs a Skybox object.
         *
         * @warning: SDL_image does not support loading HDR images.
         *
         * @param ctx The graphics context.
         * @param texPath The path to the skybox texture.
         * @param layout The cubemap layout (default is AutoDetect).
         * @param useHDR Whether to use HDR (default is false).
         */
        Skybox(Context& ctx, const std::string& texPath, Texture::CubemapLayout layout = Texture::CubemapLayout::AutoDetect, bool useHDR = false);
        
        /**
         * @brief Destructor for Skybox object.
         */
        ~Skybox();

        /**
         * @brief Draws the skybox.
         */
        void Draw();
    };

}}

#endif //NEXUS_EXT_3D_GL_SKYBOX_HPP
