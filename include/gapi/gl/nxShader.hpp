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

#ifndef NEXUS_GFX_GL_SHADER_HPP
#define NEXUS_GFX_GL_SHADER_HPP

#include "../../math/nxMat4.hpp"
#include "./nxContextual.hpp"
#include "./nxContext.hpp"
#include "./nxTexture.hpp"
#include "./nxEnums.hpp"
#include <SDL_stdinc.h>
#include <string>

namespace _gl_impl
{
    class Shader : public nexus::gl::Contextual
    {
      public:
        using LocArray = std::array<int, GL_MAX_SHADER_LOCATIONS>;

      public:
        LocArray locs;    ///< Array of shader locations.

      protected:
        Uint32 id = 0;    ///< Shader program ID.

      private:
        void LoadFromMemory(const char*, const char*);
        void LoadFromMemory(const std::string&, const std::string&);

        void Load(const char*, const char*);
        void Load(const std::string&, const std::string&);

      public:
        Shader(nexus::gl::Context& ctx);
        Shader(nexus::gl::Context& ctx, const std::string& vs, const std::string& fs, bool isData = true);
        Shader(nexus::gl::Context& ctx, const char* vs, const char* fs, bool isData = true);

        ~Shader();

        /**
         * @brief Gets the location of a default uniform in the shader program.
         * @param loc Enum uniform index.
         * @return The location of the uniform.
         */
        int GetLocation(nexus::gl::ShaderLocationIndex loc) const;

        /**
         * @brief Gets the location of a uniform in the shader program.
         * @param uniformName The name of the uniform.
         * @return The location of the uniform.
         */
        int GetLocation(const std::string& uniformName) const;

        /**
         * @brief Gets the location of an attribute in the shader program.
         * @param attribName The name of the attribute.
         * @return The location of the attribute.
         */
        int GetLocationAttrib(const std::string& attribName) const;

        /**
         * @brief Sends a value to a uniform in the shader program.
         * @param locIndex The location index of the uniform.
         * @param value Pointer to the value to send.
         * @param uniformType Type of the uniform.
         * @param count Number of elements to send.
         */
        void SendValue(int locIndex, const void* value, nexus::gl::ShaderUniformType uniformType, int count = 1);

        /**
         * @brief Sends a matrix value to a uniform in the shader program.
         * @param locIndex The location index of the uniform.
         * @param mat The matrix value to send.
         */
        void SendMatrix(int locIndex, const nexus::math::Mat4& mat);

        /**
         * @brief Sends a texture value to a uniform in the shader program.
         * @param locIndex The location index of the uniform.
         * @param texture The texture to send.
         */
        void SendTexture(int locIndex, const nexus::gl::Texture& texture);

        /**
         * @brief Sends a texture ID value to a uniform in the shader program.
         * @param locIndex The location index of the uniform.
         * @param textureID The texture ID to send.
         */
        void SendTexture(int locIndex, Uint32 textureID);

        /**
         * @brief Begins using the shader program.
         */
        void Begin();

        /**
         * @brief Ends using the shader program.
         */
        void End();

        /**
         * @brief Gets the ID of the shader program.
         * @return The shader program ID.
         */
        Uint32 GetID() const { return id; }
    };
}

namespace nexus { namespace gl {

    /**
     * @brief Represents an OpenGL shader.
     *
     * The Shader class encapsulates functionality related to OpenGL shaders.
     */
    class NEXUS_API Shader : public Container<_gl_impl::Shader>
    {
      public:
        using Location = ShaderLocationIndex;  ///< Type alias for shader location index.
        using Uniform = ShaderUniformType;     ///< Type alias for shader uniform type.

      public:
        /**
         * @brief Default constructor.
         * @param ctx The OpenGL context.
         */
        Shader(Context& ctx)
            : Container(ctx)
        { }

        /**
         * @brief Constructor specifying vertex and fragment shader sources.
         * @param ctx The OpenGL context.
         * @param vs The vertex shader source.
         * @param fs The fragment shader source.
         * @param isData Boolean indicating whether shader sources are provided as raw data.
         */
        Shader(Context& ctx, const std::string& vs, const std::string& fs, bool isData = true)
            : Container(ctx, vs, fs, isData)
        { }

        /**
         * @brief Constructor specifying vertex and fragment shader sources as C-style strings.
         * @param ctx The OpenGL context.
         * @param vs The vertex shader source as a C-style string.
         * @param fs The fragment shader source as a C-style string.
         * @param isData Boolean indicating whether shader sources are provided as raw data.
         */
        Shader(Context& ctx, const char* vs, const char* fs, bool isData = true)
            : Container(ctx, vs, fs, isData)
        { }
    };

}}

#endif //NEXUS_GFX_GL_SHADER_HPP
