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

#include "gapi/gl/nxShader.hpp"
#include "core/nxFileSystem.hpp"
#include "gapi/gl/nxConfig.hpp"
#include "gapi/gl/nxEnums.hpp"

using namespace nexus;

/* Private Shader Implementation */

void _gl_impl::Shader::LoadFromMemory(const char* vsCode, const char* fsCode)
{
    id = ctx.LoadShaderCode(vsCode, fsCode);

    if (id == 0)
    {
        core::NexusException("gl::Shader", "The shader could not be loaded");
    }

    // Default shader attribute locations have been binded before linking:
    //          vertex position location    = 0
    //          vertex texcoord location    = 1
    //          vertex normal location      = 2
    //          vertex color location       = 3
    //          vertex tangent location     = 4
    //          vertex texcoord2 location   = 5

    // NOTE: If any location is not found, loc point becomes -1

    // Get handles to GLSL input attribute locations
    locs[gl::LocVertexPosition]     = ctx.GetLocationAttrib(id, GL_DEFAULT_SHADER_ATTRIB_NAME_POSITION);
    locs[gl::LocVertexTexCoord01]   = ctx.GetLocationAttrib(id, GL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD);
    locs[gl::LocVertexTexCoord02]   = ctx.GetLocationAttrib(id, GL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2);
    locs[gl::LocVertexNormal]       = ctx.GetLocationAttrib(id, GL_DEFAULT_SHADER_ATTRIB_NAME_NORMAL);
    locs[gl::LocVertexTangent]      = ctx.GetLocationAttrib(id, GL_DEFAULT_SHADER_ATTRIB_NAME_TANGENT);
    locs[gl::LocVertexColor]        = ctx.GetLocationAttrib(id, GL_DEFAULT_SHADER_ATTRIB_NAME_COLOR);

    // Get handles to GLSL uniform locations (vertex shader)
    locs[gl::LocMatrixMVP]          = ctx.GetLocationUniform(id, GL_DEFAULT_SHADER_UNIFORM_NAME_MVP);
    locs[gl::LocMatrixView]         = ctx.GetLocationUniform(id, GL_DEFAULT_SHADER_UNIFORM_NAME_VIEW);
    locs[gl::LocMatrixProjection]   = ctx.GetLocationUniform(id, GL_DEFAULT_SHADER_UNIFORM_NAME_PROJECTION);
    locs[gl::LocMatrixModel]        = ctx.GetLocationUniform(id, GL_DEFAULT_SHADER_UNIFORM_NAME_MODEL);
    locs[gl::LocMatrixNormal]       = ctx.GetLocationUniform(id, GL_DEFAULT_SHADER_UNIFORM_NAME_NORMAL);

    // Get handles to GLSL uniform locations (fragment shader)
    locs[gl::LocColorDiffuse]       = ctx.GetLocationUniform(id, GL_DEFAULT_SHADER_UNIFORM_NAME_COLOR);
    locs[gl::LocMapDiffuse]         = ctx.GetLocationUniform(id, GL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0);
    locs[gl::LocMapSpecular]        = ctx.GetLocationUniform(id, GL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE1);
    locs[gl::LocMapNormal]          = ctx.GetLocationUniform(id, GL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE2);
}

void _gl_impl::Shader::LoadFromMemory(const std::string& vsCode, const std::string& fsCode)
{
    return LoadFromMemory(vsCode.c_str(), fsCode.c_str());
}

void _gl_impl::Shader::Load(const char* vsPath, const char* fsPath)
{
    std::string vsCode = vsPath ? core::LoadTextFile(vsPath) : "";
    std::string fsCode = fsPath ? core::LoadTextFile(fsPath) : "";

    LoadFromMemory(
        vsCode.empty() ? nullptr : vsCode.c_str(),
        fsCode.empty() ? nullptr : fsCode.c_str());
}

void _gl_impl::Shader::Load(const std::string& vsPath, const std::string& fsPath)
{
    LoadFromMemory(
        core::LoadTextFile(vsPath).c_str(),
        core::LoadTextFile(fsPath).c_str());
}

_gl_impl::Shader::Shader(gl::Context& ctx)
: Contextual(ctx)
, id(ctx.GetShaderIdDefault())
{
    const int *defLocs = ctx.GetShaderLocsDefault();

    for (int i = 0; i < GL_MAX_SHADER_LOCATIONS; i++)
    {
        locs[i] = defLocs[i];
    }
}

_gl_impl::Shader::Shader(gl::Context& ctx, const std::string& vs, const std::string& fs, bool isData)
: Contextual(ctx)
{
    (isData ? LoadFromMemory(vs, fs) : Load(vs, fs));
}

_gl_impl::Shader::Shader(gl::Context& ctx, const char* vs, const char* fs, bool isData)
: Contextual(ctx)
{
    (isData ? LoadFromMemory(vs, fs) : Load(vs, fs));
}

_gl_impl::Shader::~Shader()
{
    if (id > 0 && id != ctx.GetShaderIdDefault())
    {
        ctx.UnloadShaderProgram(id);
    }
}

int _gl_impl::Shader::GetLocation(gl::ShaderLocationIndex loc) const
{
    return locs[loc];
}

int _gl_impl::Shader::GetLocation(const std::string& uniformName) const
{
    return ctx.GetLocationUniform(id, uniformName);
}

int _gl_impl::Shader::GetLocationAttrib(const std::string& attribName) const
{
    return ctx.GetLocationAttrib(id, attribName);
}

void _gl_impl::Shader::SendValue(int locIndex, const void *value, gl::ShaderUniformType uniformType, int count)
{
    if (locIndex > -1)
    {
        ctx.EnableShader(id);
        ctx.SetUniform(locIndex, value, uniformType, count);
    }
}

void _gl_impl::Shader::SendMatrix(int locIndex, const math::Mat4& mat)
{
    if (locIndex > -1)
    {
        ctx.EnableShader(id);
        ctx.SetUniformMatrix(locIndex, mat.m);
    }
}

void _gl_impl::Shader::SendTexture(int locIndex, const gl::Texture& texture)
{
    if (locIndex > -1)
    {
        ctx.EnableShader(id);
        ctx.SetUniformSampler(locIndex, texture->GetID());
    }
}

void _gl_impl::Shader::SendTexture(int locIndex, Uint32 textureID)
{
    if (locIndex > -1)
    {
        ctx.EnableShader(id);
        ctx.SetUniformSampler(locIndex, textureID);
    }
}

void _gl_impl::Shader::Begin()
{
    ctx.SetShader(id, locs.data());
}

void _gl_impl::Shader::End()
{
    ctx.SetShader(ctx.GetShaderIdDefault(), ctx.GetShaderLocsDefault());
}
