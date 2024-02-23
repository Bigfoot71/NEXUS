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

#include "gapi/gl/ext_3D/nxSkybox.hpp"

using namespace nexus;

/* Private Implementaiton Skybox */

#if defined(GRAPHICS_API_OPENGL_33) && !defined(GRAPHICS_API_OPENGL_21)

constexpr char vertCubemapHDR[] =
    "#version 330"
    "in vec3 vertexPosition;"
    "uniform mat4 matProjection;"
    "uniform mat4 matView;"
    "out vec3 fragPosition;"
    "void main()"
    "{"
        "fragPosition = vertexPosition;"
        "gl_Position = matProjection*matView*vec4(vertexPosition, 1.0);"
    "}";

constexpr char fragCubemapHDR[] =
    "#version 330\n"
    "in vec3 fragPosition;"
    "uniform sampler2D equirectangularMap;"
    "out vec4 finalColor;"
    "vec2 SampleSphericalMap(vec3 v)"
    "{"
        "vec2 uv = vec2(atan(v.z, v.x), asin(v.y));"
        "uv *= vec2(0.1591, 0.3183);"
        "uv += 0.5;"
        "return uv;"
    "}"
    "void main()"
    "{"
        "vec2 uv = SampleSphericalMap(normalize(fragPosition));"
        "vec3 color = texture(equirectangularMap, uv).rgb;"
        "finalColor = vec4(color, 1.0);"
    "}";

constexpr char vertSkybox[] =
    "#version 330\n"
    "in vec3 vertexPosition;"
    "uniform mat4 matProjection;"
    "uniform mat4 matView;"
    "out vec3 fragPosition;"
    "void main()"
    "{"
        // Calculate fragment position based on model transformations
        "fragPosition = vertexPosition;"
        // Remove translation from the view matrix
        "mat4 rotView = mat4(mat3(matView));"
        "vec4 clipPos = matProjection*rotView*vec4(vertexPosition, 1.0);"
        // Calculate final vertex position
        "gl_Position = clipPos;"
    "}";

constexpr char fragSkybox[] =
    "#version 330\n"
    "in vec3 fragPosition;"
    "uniform samplerCube environmentMap;"
    "uniform bool vflipped;"
    "uniform bool doGamma;"
    "out vec4 finalColor;"

    "void main()"
    "{"
        "vec3 color = vec3(0.0);"
        "if (vflipped)"
        "{"
            "color = texture(environmentMap, vec3(fragPosition.x, -fragPosition.y, fragPosition.z)).rgb;"
        "}"
        "else"
        "{"
            "color = texture(environmentMap, fragPosition).rgb;"
        "}"
        "if (doGamma)" // Apply gamma correction
        "{"
            "color = color/(color + vec3(1.0));"
            "color = pow(color, vec3(1.0/2.2));"
        "}"
        "finalColor = vec4(color, 1.0);"
    "}";

#else

constexpr char vertCubemapHDR[] =
    "#version 100\n"
    "attribute vec3 vertexPosition;"
    "uniform mat4 matProjection;"
    "uniform mat4 matView;"
    "varying vec3 fragPosition;"
    "void main()"
    "{"
        "fragPosition = vertexPosition;"
        "gl_Position = matProjection*matView*vec4(vertexPosition, 1.0);"
    "}";

constexpr char fragCubemapHDR[] =
    "#version 100\n"
    "precision mediump float;"
    "varying vec3 fragPosition;"
    "uniform sampler2D equirectangularMap;"
    "vec2 SampleSphericalMap(vec3 v)"
    "{"
        "vec2 uv = vec2(atan(v.z, v.x), asin(v.y));"
        "uv *= vec2(0.1591, 0.3183);"
        "uv += 0.5;"
        "return uv;"
    "}"
    "void main()"
    "{"
        "vec2 uv = SampleSphericalMap(normalize(fragPosition));"
        "vec3 color = texture2D(equirectangularMap, uv).rgb;"
        "gl_FragColor = vec4(color, 1.0);"
    "}";

constexpr char vertSkybox[] =
    "#version 100\n"
    "attribute vec3 vertexPosition;"
    "uniform mat4 matProjection;"
    "uniform mat4 matView;"
    "varying vec3 fragPosition;"
    "void main()"
    "{"
        // Calculate fragment position based on model transformations
        "fragPosition = vertexPosition;"
        // Remove translation from the view matrix
        "mat4 rotView = mat4(mat3(matView));"
        "vec4 clipPos = matProjection*rotView*vec4(vertexPosition, 1.0);"
        // Calculate final vertex position
        "gl_Position = clipPos;"
    "}";

constexpr char fragSkybox[] =
    "#version 100\n"
    "precision mediump float;"
    "varying vec3 fragPosition;"
    "uniform samplerCube environmentMap;"
    "uniform bool vflipped;"
    "uniform bool doGamma;"
    "void main()"
    "{"
        "vec4 texelColor = vec4(0.0);"
        "if (vflipped)"
        "{"
            "texelColor = textureCube(environmentMap, vec3(fragPosition.x, -fragPosition.y, fragPosition.z));"
        "}"
        "else"
        "{"
            "texelColor = textureCube(environmentMap, fragPosition);"
        "}"
        "vec3 color = vec3(texelColor.x, texelColor.y, texelColor.z);"
        "if (doGamma)" // Apply gamma correction
        "{"
            "color = color/(color + vec3(1.0));"
            "color = pow(color, vec3(1.0/2.2));"
        "}"
        "gl_FragColor = vec4(color, 1.0);"
    "}";

#endif

// Generate cubemap texture from HDR texture
void gl::Skybox::GenTextureCubemapHDR(Context& ctx, Shader& shader, const gfx::Surface& panorama)
{
    ctx.DisableBackfaceCulling();     // Disable backface culling to render inside the cube

    /* Load panorama texture */

    int size = panorama.GetWidth();
    if (size != panorama.GetHeight())
    {
        throw core::NexusException("gl::Skybox",
            "Each dimension of the HDR panorama texture must be equal.");
    }

    TextureFormat format = Texture::ConvertPixelFormat(panorama.GetPixelFormat());
    Uint32 panoramaID = ctx.LoadTexture(panorama.GetPixels(), size, size, format, 1);

    /* Setup framebuffer */

    Uint32 rbo = ctx.LoadTextureDepth(size, size, true);
    Uint32 cubemapID = ctx.LoadTextureCubemap(0, size, format);

    Uint32 fbo = ctx.LoadFramebuffer(size, size);
    ctx.FramebufferAttach(fbo, rbo, FramebufferAttachType::Depth, FramebufferAttachTextureType::RenderBuffer, 0);
    ctx.FramebufferAttach(fbo, cubemapID, FramebufferAttachType::ColorChannel0, FramebufferAttachTextureType::CubemapPositiveX, 0);

    // Check if framebuffer is complete with attachments (valid)
    if (!ctx.FramebufferComplete(fbo))
    {
        throw core::NexusException("gl::Skybox",
            "Unable to create the framebuffer when generating the HDR cubemap.");
    }

    /* Draw to framebuffer */

    // NOTE: Shader is used to convert HDR equirectangular environment map to cubemap equivalent (6 faces)
    ctx.EnableShader(shader->GetID());

    // Define projection matrix and send it to shader
    math::Mat4 matFboProjection = math::Mat4::Perspective(90.0f * math::Deg2Rad, 1.0f, 0.01f, 1000.0f);
    ctx.SetUniformMatrix(shader->locs[gl::LocMatrixProjection], matFboProjection);

    // Define view matrix for every side of the cubemap
    math::Mat4 fboViews[6] = {
        math::Mat4::LookAt(math::Vec3(0.0f, 0.0f, 0.0f), math::Vec3(1.0f,  0.0f,  0.0f), math::Vec3(0.0f, -1.0f,  0.0f)),
        math::Mat4::LookAt(math::Vec3(0.0f, 0.0f, 0.0f), math::Vec3(-1.0f,  0.0f,  0.0f), math::Vec3(0.0f, -1.0f,  0.0f)),
        math::Mat4::LookAt(math::Vec3(0.0f, 0.0f, 0.0f), math::Vec3( 0.0f,  1.0f,  0.0f), math::Vec3(0.0f,  0.0f,  1.0f)),
        math::Mat4::LookAt(math::Vec3(0.0f, 0.0f, 0.0f), math::Vec3( 0.0f, -1.0f,  0.0f), math::Vec3(0.0f,  0.0f, -1.0f)),
        math::Mat4::LookAt(math::Vec3(0.0f, 0.0f, 0.0f), math::Vec3( 0.0f,  0.0f,  1.0f), math::Vec3(0.0f, -1.0f,  0.0f)),
        math::Mat4::LookAt(math::Vec3(0.0f, 0.0f, 0.0f), math::Vec3( 0.0f,  0.0f, -1.0f), math::Vec3(0.0f, -1.0f,  0.0f))
    };

    shape2D::Rectangle prevViewport = ctx.GetViewport();
    ctx.Viewport(0, 0, size, size);   // Set viewport to current fbo dimensions
    
    // Activate and enable texture for drawing to cubemap faces
    ctx.ActiveTextureSlot(0);
    ctx.EnableTexture(panoramaID);

    for (int i = 0; i < 6; i++)
    {
        // Set the view matrix for the current cube face
        ctx.SetUniformMatrix(shader->locs[gl::LocMatrixView], fboViews[i]);
        
        // Select the current cubemap face attachment for the fbo
        // WARNING: This function by default enables->attach->disables fbo!!!
        ctx.FramebufferAttach(fbo, cubemapID, FramebufferAttachType::ColorChannel0, static_cast<FramebufferAttachTextureType>(static_cast<int>(FramebufferAttachTextureType::CubemapPositiveX) + i), 0);
        ctx.EnableFramebuffer(fbo);

        // Load and draw a cube, it uses the current enabled texture
        ctx.ClearScreenBuffers();
        ctx.LoadDrawCube();

        // ALTERNATIVE: Try to use internal batch system to draw the cube instead of rlLoadDrawCube
        // for some reason this method does not work, maybe due to cube triangles definition? normals pointing out?
        // TODO: Investigate this issue...
        //rlSetTexture(panorama.id); // WARNING: It must be called after enabling current framebuffer if using internal batch system!
        //rlClearScreenBuffers();
        //DrawCubeV(Vector3Zero(), Vector3One(), WHITE);
        //rlDrawRenderBatchActive();
    }

    /* Unload framebuffer and reset state */

    ctx.DisableShader();          // Unbind shader
    ctx.DisableTexture();         // Unbind texture
    ctx.DisableFramebuffer();     // Unbind framebuffer
    ctx.UnloadFramebuffer(fbo);   // Unload framebuffer (and automatically attached depth texture/renderbuffer)

    // Reset viewport dimensions to default
    ctx.SetViewport(prevViewport);
    ctx.EnableBackfaceCulling();

    // Create final cubamp texture
    texture = std::make_unique<Texture>(ctx, cubemapID, panorama.GetWidth(), panorama.GetHeight(), 1, format);
}


/* Public Implementation Skybox */

gl::Skybox::Skybox(Context& ctx, const std::string& texPath, Texture::CubemapLayout layout, bool useHDR)
: useHDR(useHDR)
{
    if ((instanceCounter++) == 0)
    {
        model = std::make_unique<Model>(ctx, Mesh::Cube(ctx, 1.0f, 1.0f, 1.0f));

        Shader shader(ctx, vertSkybox, fragSkybox);

        locEnvironementMap = shader->GetLocation("environmentMap");
        locVFlipped = shader->GetLocation("vflipped");
        locDoGamma = shader->GetLocation("doGamma");

        (*model)->materials[0]->shader = shader;
    }

    if (useHDR)
    {
        if (shaderCubemapHDR == nullptr)
        {
            shaderCubemapHDR = std::make_unique<Shader>(ctx, vertCubemapHDR, fragCubemapHDR);
        }

        GenTextureCubemapHDR(ctx, *shaderCubemapHDR, gfx::Surface(texPath));
    }
    else
    {
        texture = std::make_unique<Texture>(ctx, gfx::Surface(texPath), layout);
    }
}

gl::Skybox::~Skybox()
{
    if ((--instanceCounter) == 0)
    {
        shaderCubemapHDR = nullptr;
        model = nullptr;
    }
}

void gl::Skybox::Draw()
{
    if (this != prevSkybox)
    {
        const int envMap = static_cast<int>(Material::MapType::Cubemap);
        const int hdr = useHDR ? 1 : 0;

        (*model)->materials[0]->shader->SendValue(locEnvironementMap, &envMap, Shader::Uniform::Int);
        (*model)->materials[0]->shader->SendValue(locVFlipped, &hdr, Shader::Uniform::Int);
        (*model)->materials[0]->shader->SendValue(locDoGamma, &hdr, Shader::Uniform::Int);
        (*model)->materials[0]->maps[Material::MapType::Cubemap].texture = *texture;
    }

    model->GetContext().DisableBackfaceCulling();
    model->GetContext().DisableDepthMask();

        (*model)->Draw({ 0, 0, 0 }, 1.0f, gfx::White);

    model->GetContext().EnableBackfaceCulling();
    model->GetContext().EnableDepthMask();
}
