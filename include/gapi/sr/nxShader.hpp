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

#ifndef NEXUS_SR_SHADER_HPP
#define NEXUS_SR_SHADER_HPP

#include "../../gfx/nxSurface.hpp"

namespace nexus { namespace sr {

    /**
     * @brief Base class for shaders used in the rasterizer.
     *
     * This class serves as the base class for shaders used in the rasterizer.
     * It provides virtual functions for vertex and fragment processing, which can be overridden
     * by derived classes to implement custom shader behavior.
     */
    struct NEXUS_API Shader
    {
        class Context* ctx = nullptr;

      public:
        Shader() = default;

        /**
         * @brief Constructs a shader with a specific context.
         * @param ctx The context associated with the shader.
         */
        Shader(class Context& ctx)
        : ctx(&ctx)
        { }

        virtual ~Shader() = default;

        /**
         * @brief Performs vertex processing in the shader.
         * @param mvp The model-view-projection matrix.
         * @param vertex The input vertex to be processed.
         * @return The processed vertex.
         */
        virtual math::Vec4 Vertex(const math::Mat4& mvp, const math::Vec4& vertex)
        {
            return mvp * vertex;
        }

        /**
         * @brief Performs fragment processing in the shader.
         * @param fragCoord The fragment coordinates.
         * @param fragNormal The fragment normal vector.
         * @param fragColor The fragment color.
         * @return The processed fragment color.
         */
        virtual gfx::Color Fragment(const math::IVec2& fragCoord, const math::Vec3& fragNormal, const gfx::Color& fragColor)
        {
            (void)fragCoord; (void)fragNormal;
            return fragColor;
        }

        /**
         * @brief Performs fragment processing using a texture in the shader.
         * @param image The texture image.
         * @param fragCoord The fragment coordinates.
         * @param texCoord The texture coordinates.
         * @param fragNormal The fragment normal vector.
         * @param fragColor The fragment color.
         * @return The processed fragment color.
         */
        virtual gfx::Color Fragment(const gfx::Surface* image, const math::IVec2& fragCoord, const math::Vec2& texCoord, const math::Vec3 fragNormal, const gfx::Color& fragColor)
        {
            (void)fragCoord; (void)fragNormal;
            return image->GetFragUnsafe(texCoord) * fragColor;
        }

        /**
         * @brief Begins shader processing.
         */
        void Begin();

        /**
         * @brief Ends shader processing.
         */
        void End();
    };

}}

#endif //NEXUS_SR_SHADER_HPP
