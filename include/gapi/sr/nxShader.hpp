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
