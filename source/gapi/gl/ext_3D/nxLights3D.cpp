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

#include "gapi/gl/ext_3D/nxLights3D.hpp"

#include "gapi/gl/sp_model/nxMaterial.hpp"
#include "shape/2D/nxRectangle.hpp"
#include "gapi/gl/nxConfig.hpp"
#include "gapi/gl/nxEnums.hpp"
#include "math/nxMath.hpp"
#include "core/nxText.hpp"

#include <cstring>
#include <memory>
#include <string>

using namespace nexus;

/* Public Implementation ShadowMap */

constexpr char fragDebugShadowMap[] = GLSL_VERSION
#   if defined(GLSL_ES_100)
    "precision mediump float;\n"
#   endif
#   if defined(GLSL_ES_100) || defined(GLSL_120)
    "#define TEXTURE texture2D\n"
    "varying vec2 fragTexCoord;"
#   else
    "#define TEXTURE texture\n"
    "in vec2 fragTexCoord;"
    "out vec4 finalColor;"
#   endif

    "uniform sampler2D texture0;"
    "uniform float near;"
    "uniform float far;"

    "float LinearizeDepth(float depth)"
    "{"
        "float z = depth * 2.0 - 1.0;"
        "return (2.0 * near * far) / (far + near - z * (far - near));"
    "}"

    "void main()"
    "{"
        "vec2 uv = vec2(fragTexCoord.x, 1.0 - fragTexCoord.y);"
        "float depth = LinearizeDepth(TEXTURE(texture0, uv).r);"
#       if defined(GLSL_ES_100) || defined(GLSL_120)
        "gl_FragColor = vec4(vec3(depth) / far, 1.0);"
#       else
        "finalColor = vec4(vec3(depth) / far, 1.0);"
#       endif
    "}";

gl::ShadowMap::ShadowMap(nexus::gl::Context& ctx, int w, int h)
: Contextual(ctx), depth(ctx, w, h, false)
{
    fboId = ctx.LoadFramebuffer(w, h);
    ctx.EnableFramebuffer(fboId);

    ctx.TextureParameters(depth->GetID(), nexus::gl::TextureParam::Wrap_S, nexus::gl::TextureWrap::Clamp);
    ctx.TextureParameters(depth->GetID(), nexus::gl::TextureParam::Wrap_T, nexus::gl::TextureWrap::Clamp);

    ctx.FramebufferAttach(fboId, depth->GetID(), nexus::gl::FramebufferAttachType::Depth, nexus::gl::FramebufferAttachTextureType::Texture2D, 0);

    ctx.DisableFramebuffer();

    instanceCounter++;
}

gl::ShadowMap::~ShadowMap()
{
    ctx.UnloadFramebuffer(fboId);
    fboId = 0;

    if ((--instanceCounter) == 0 && debugShader != nullptr)
    {
        debugShader = nullptr;
    }
}

void gl::ShadowMap::Begin(const nexus::shape2D::Rectangle& bounds)
{
    ctx.DrawRenderBatchActive();
    ctx.EnableFramebuffer(fboId);

    // Define the area of ​​the buffer where it will be necessary to work
    ctx.EnableScissorTest();
    ctx.Scissor(bounds.x, bounds.y, bounds.w, bounds.h);

    // Set viewport to framebuffer size
    // And set orthographic projection
    prevViewport = ctx.GetViewport();
    ctx.SetViewport(bounds);

    // Disable front face rendering (to render back faces)
    // This is done to prevent Peter Panning due to bias for avoid acne
    // https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
    ctx.SetCullFace(CullMode::FaceFront);

    ctx.DisableColorBlend();

    active = true;
}

void gl::ShadowMap::End()
{
    ctx.EnableColorBlend();
    ctx.DisableTexture();

    ctx.DisableScissorTest();

    ctx.DrawRenderBatchActive();
    ctx.DisableFramebuffer();

    ctx.SetViewport(prevViewport);
    ctx.SetCullFace(CullMode::FaceBack);

    active = false;
}

void gl::ShadowMap::Clear()
{
    if (!active)
    {
        ctx.DrawRenderBatchActive();
        ctx.EnableFramebuffer(fboId);
        prevViewport = ctx.GetViewport();
        ctx.Viewport(0, 0, depth->GetWidth(), depth->GetHeight());
    }

    ctx.ClearColor(255, 255, 255, 255);
    ctx.ClearScreenBuffers();

    if (!active)
    {
        ctx.DisableFramebuffer();
        ctx.Viewport(prevViewport);
    }
}

void gl::ShadowMap::Draw(const shape2D::Rectangle& dst, float near, float far)
{
    if (debugShader == nullptr)
    {
        debugShader = std::make_unique<Shader>(depth.GetContext(), nullptr, fragDebugShadowMap);
        locNear = (*debugShader)->GetLocation("near");
        locFar = (*debugShader)->GetLocation("far");
    }

    (*debugShader)->SendValue(locNear, &near, Shader::Uniform::Float);
    (*debugShader)->SendValue(locFar, &far, Shader::Uniform::Float);

    (*debugShader)->Begin();
        depth->Draw(depth->GetRectSize(), dst);
    (*debugShader)->End();
}


/* Light(s) Implementation */

constexpr char vertShadow[] = GLSL_VERSION
#   if defined(GLSL_ES_100)
    "precision mediump float;"
#   endif
#   if defined(GLSL_ES_100) || defined(GLSL_120)
    "attribute vec3 vertexPosition;"
#   else
    "in vec3 vertexPosition;"
#   endif
    "uniform mat4 mvp;"
    "void main()"
    "{"
        "gl_Position = mvp * vec4(vertexPosition, 1.0);"
    "}";

constexpr char fragShadow[] = GLSL_VERSION
#   if defined(GLSL_ES_100)
    "precision mediump float;"
#   endif
    "void main()"
    "{"
        "gl_FragDepth = gl_FragCoord.z;"
    "}";

constexpr char vertLight[] = GLSL_VERSION
#   if defined(GLSL_ES_100)
    "precision highp float;\n"
#   endif
    "#define MAX_LIGHTS %i\n"

    "struct Light {"
        "mat4 matrix;"
        "vec4 mapBounds;"
        "vec3 position;"
        "vec3 direction;"
        "vec3 color;"
        "float spotSoftness;"
        "float cutoff;"
        "float radius;"
        "int shadow;"
        "int spotlight;"
        "int enabled;"
    "};"

#   if defined(GLSL_ES_100) || defined(GLSL_120)
    "attribute vec3 vertexPosition;"
    "attribute vec2 vertexTexCoord;"
    "attribute vec3 vertexNormal;"
    "attribute vec4 vertexTangent;"
    "attribute vec4 vertexColor;"
#   else
    "in vec3 vertexPosition;"
    "in vec2 vertexTexCoord;"
    "in vec3 vertexNormal;"
    "in vec4 vertexTangent;"
    "in vec4 vertexColor;"
#   endif

    "uniform mat4 mvp;"
    "uniform mat4 matModel;"
    "uniform mat4 matNormal;"

    "uniform vec3 viewPos;"
    "uniform Light lights[MAX_LIGHTS];"

    "uniform int useSpecularMap;"
    "uniform int useNormalMap;"
    "uniform int useHeightMap;"

#   if defined(GLSL_ES_100) || defined(GLSL_120)
    "varying VS_OUT {"
#   else
    "out VS_OUT {"
#   endif
        "mat3 TBN;"                         ///< For calculating normals in world space from the normalMap
        "vec4 shadowPos[MAX_LIGHTS];"       ///< Used to compute shadow aeras
        "vec3 tanFragPosition;"             ///< Used to compute heighmap displacement textures
        "vec3 tanViewPos;"                  ///< Used to compute heighmap displacement textures
        "vec3 viewPos;"
        "vec4 fragColor;"
        "vec3 fragNormal;"
        "vec3 fragPosition;"
        "vec2 fragTexCoord;"
        "flat int useSpecularMap;"
        "flat int useNormalMap;"
        "flat int useHeightMap;"
    "} vsOut;"

    "const float normalOffset = 0.1;"

    "void main()"
    "{"
        "vsOut.viewPos = viewPos;"
        "vsOut.fragColor = vertexColor;"
        "vsOut.fragTexCoord = vertexTexCoord;"

        "vsOut.fragPosition = vec3(matModel * vec4(vertexPosition, 1.0));"
        "vsOut.fragNormal = normalize(vec3(matNormal * vec4(vertexNormal, 0.0)));"

        "vsOut.TBN = mat3(1.0);"
        "if (useNormalMap + useHeightMap > 0)"
        "{"
            "vec3 T = normalize(vec3(matModel * vec4(vertexTangent.xyz, 0.0)));"
            "vec3 B = cross(vsOut.fragNormal, T) * vertexTangent.w;"
            "vsOut.TBN = mat3(T, B, vsOut.fragNormal);"
        "}"

        "if (useHeightMap > 0)"
        "{"
            "mat3 invTBN = transpose(vsOut.TBN);"
            "vsOut.tanViewPos = invTBN * viewPos;"
            "vsOut.tanFragPosition = invTBN * vertexPosition;"
        "}"

        "for (int i = 0; i < MAX_LIGHTS; i++)"
        "{"
            "if (lights[i].enabled > 0 && lights[i].shadow > 0)"
            "{"
                "vec3 lightDir = normalize(lights[i].position - vsOut.fragPosition);"
                "float cosAngle = clamp(1.0 - dot(lightDir, vsOut.fragNormal), 0, 1);"
                "vec3 scaledNormalOffset = vsOut.fragNormal * (normalOffset * cosAngle);"
                "vsOut.shadowPos[i] = lights[i].matrix * vec4(vsOut.fragPosition + scaledNormalOffset, 1.0);"
            "}"
        "}"

        "vsOut.useSpecularMap = useSpecularMap;"
        "vsOut.useNormalMap = useNormalMap;"
        "vsOut.useHeightMap = useHeightMap;"

        "gl_Position = mvp * vec4(vertexPosition, 1.0);"
    "}";

constexpr char fragLight[] = GLSL_VERSION
#   if defined(GLSL_ES_100)
    "precision highp float;\n"
#   endif
#   if defined(GLSL_ES_100) || defined(GLSL_120)
        "#define TEXTURE texture2D\n"
#   else
        "#define TEXTURE texture\n"
#   endif
    "#define MAX_LIGHTS %i\n"

    "struct Light {"
        "mat4 matrix;"
        "vec4 mapBounds;"
        "vec3 position;"
        "vec3 direction;"
        "vec3 color;"
        "float spotSoftness;"
        "float cutoff;"
        "float radius;"
        "int shadow;"
        "int spotlight;"
        "int enabled;"
    "};"

#   if defined(GLSL_ES_100) || defined(GLSL_120)
    "varying VS_OUT {"
#   else
    "in VS_OUT {"
#   endif
        "mat3 TBN;"
        "vec4 shadowPos[MAX_LIGHTS];"
        "vec3 tanFragPosition;"
        "vec3 tanViewPos;"
        "vec3 viewPos;"
        "vec4 fragColor;"
        "vec3 fragNormal;"
        "vec3 fragPosition;"
        "vec2 fragTexCoord;"
        "flat int useSpecularMap;"
        "flat int useNormalMap;"
        "flat int useHeightMap;"
    "} fsIn;"

    "uniform Light lights[MAX_LIGHTS];"
    "uniform vec4 colDiffuse;"

    "uniform sampler2D texture0;"               ///< DIFFUSE
    "uniform sampler2D texture1;"               ///< SPECULAR
    "uniform sampler2D texture2;"               ///< NORMAL
    "uniform sampler2D texture6;"               ///< HEIGHT

    "uniform float shadowMapTexelSize;"
    "uniform sampler2D shadowMap;"
    "uniform vec3 ambient;"

    "const float heightMapScale = 0.05;"        ///< TODO: Make it a configurable uniform directly via material value.

#   if defined(GLSL_330) || defined(GLSL_430) || defined(GLSL_ES_300)
    "out vec4 finalColor;"
#   endif

    "float ShadowCalc(int i, vec4 p, float bias)"
    "{"
        "vec3 projCoords = p.xyz / p.w;"
        "projCoords = projCoords * 0.5 + 0.5;"
        "projCoords.xy *= lights[i].mapBounds.zw;"
        "projCoords.xy += lights[i].mapBounds.xy;"
        "projCoords.z = projCoords.z - bias;"

        "float depth = projCoords.z;"
        "float shadow = 0.0;"

        "for (int x = -1; x <= 1; ++x)"
        "{"
            "for (int y = -1; y <= 1; ++y)"
            "{"
                "float pcfDepth = TEXTURE(shadowMap, projCoords.xy + vec2(x, y) * shadowMapTexelSize).r;"
                "shadow += step(depth, pcfDepth);"
            "}"
        "}"

        "shadow /= 9.0;"
        "if (projCoords.z > 1.0 || projCoords.x > 1.0 || projCoords.y > 1.0)"
        "{"
            "shadow = 1.0;"
        "}"

        "return shadow;"
    "}"

    "vec2 ParallaxMapping(vec2 texCoord, vec3 viewDir)"
    "{"
        "const float minLayers = 8, maxLayers = 32;"
        "float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir))); "

        "float layerDepth = 1.0 / numLayers;"
        "float currentLayerDepth = 0.0;"

        "vec2 P = viewDir.xy / viewDir.z * heightMapScale; "
        "vec2 deltaTexCoord = P / numLayers;"
    
        "vec2 currentTexCoord = texCoord;"
        "float currentDepthMapValue = TEXTURE(texture6, currentTexCoord).y;"
        
        "while(currentLayerDepth < currentDepthMapValue)"
        "{"
            "currentTexCoord += deltaTexCoord, currentLayerDepth += layerDepth;"
            "currentDepthMapValue = TEXTURE(texture6, currentTexCoord).y;"
        "}"

        "vec2 prevTexCoord = currentTexCoord - deltaTexCoord;"
        "float afterDepth  = currentDepthMapValue + currentLayerDepth;"
        "float beforeDepth = TEXTURE(texture6, prevTexCoord).y - currentLayerDepth - layerDepth;"

        "float weight = afterDepth / (afterDepth - beforeDepth);"
        "return prevTexCoord * weight + currentTexCoord * (1.0 - weight);"
    "}"

    "void main()"
    "{"
        "vec2 texCoord = fsIn.fragTexCoord;"
        "if (fsIn.useHeightMap > 0)"
        "{"
            "texCoord = ParallaxMapping(texCoord, normalize(fsIn.tanViewPos - fsIn.tanFragPosition));"
            "if (texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0) discard;"
        "}"

        "vec3 normal = fsIn.fragNormal;"
        "if (fsIn.useNormalMap > 0)"
        "{"
            "normal = TEXTURE(texture2, texCoord).xyz * 2.0 - 1.0;"
            "normal = normalize(fsIn.TBN * normal);"
        "}"

        "vec3 texelDiffuse = TEXTURE(texture0, texCoord).rgb * colDiffuse.rgb * fsIn.fragColor.rgb;"
        "vec3 texelSpecular = (fsIn.useSpecularMap > 0) ? TEXTURE(texture1, texCoord).rgb : vec3(1.0);"

        "vec3 viewDir = normalize(fsIn.viewPos - fsIn.fragPosition);"

        "vec3 resultColor = vec3(0.0);"
        "for (int i = 0; i < MAX_LIGHTS; i++)"
        "{"
            "if (lights[i].enabled != 1) continue;"

            "vec3 lightRaw = lights[i].position - fsIn.fragPosition;"
            "float lightDistSqr = dot(lightRaw, lightRaw);"
            "vec3 lightDir = normalize(lightRaw);"

            "float NdL = max(0.0, dot(lightDir, normal));"
            "float NdH = max(0.0, dot(normalize(lightDir + viewDir), normal));"

            "float attenuation = clamp(1.0 - lightDistSqr / (lights[i].radius * lights[i].radius), 0, 1);"
            "float diff = NdL * (attenuation * attenuation);"

            "if (lights[i].shadow > 0)"
            "{"
                "diff *= ShadowCalc(i, fsIn.shadowPos[i], 0.0);"
            "}"

            "if (lights[i].spotlight > 0)"
            "{"
                "float theta = dot(-lightDir, normalize(lights[i].direction));"
                "float epsilon = lights[i].cutoff - lights[i].spotSoftness;"
                "diff *= clamp((theta - lights[i].cutoff) / epsilon, 0.0, 1.0);"            // diff *= spot
            "}"

            "vec3 lightColor = texelDiffuse * lights[i].color;"                             // diffuse * lightColor
            "vec3 specularity = texelSpecular * (pow(NdH, 64.0) * diff);"                   // specular * specIntensity

            "resultColor += (lightColor * diff + specularity) + (lightColor * ambient);"
        "}"

#       if defined(GLSL_ES_100) || defined(GLSL_120)
        "gl_FragColor = vec4(resultColor, 1.0);"
#       else
        "finalColor = vec4(resultColor, 1.0);"
#       endif
    "}";


/* Private Implementation Light3D */

gl::Light3D::Light3D(Shader* modelShader, ShadowMap* shadowMap, Uint16 lightNum, const Camera3D& caster, const gfx::Color& color, float spotSoftness, float radius, bool spotlight, bool enabled)
: modelShader(modelShader)
, shadowMap(shadowMap)
, boundsMap(0,0,0,0)
, caster(caster)
, color(color)
, spotSoftness(spotSoftness)
, radius(radius)
, spotlight(spotlight)
, enabled(enabled)
{
    this->caster.aspect = 1.0f;

    const std::string uniform("lights[" + std::to_string(lightNum) + "].");

    locsLightModelShader[LocLightMatrix]        = (*modelShader)->GetLocation(uniform+"matrix");
    locsLightModelShader[LocLightPosition]      = (*modelShader)->GetLocation(uniform+"position");
    locsLightModelShader[LocLightDirection]     = (*modelShader)->GetLocation(uniform+"direction");
    locsLightModelShader[LocLightColor]         = (*modelShader)->GetLocation(uniform+"color");
    locsLightModelShader[LocLightCutoff]        = (*modelShader)->GetLocation(uniform+"cutoff");
    locsLightModelShader[LocLightRadius]        = (*modelShader)->GetLocation(uniform+"radius");
    locsLightModelShader[LocLightBounds]        = (*modelShader)->GetLocation(uniform+"mapBounds");
    locsLightModelShader[LocLightShadow]        = (*modelShader)->GetLocation(uniform+"shadow");
    locsLightModelShader[LocLightSpot]          = (*modelShader)->GetLocation(uniform+"spotlight");
    locsLightModelShader[LocLightSpotSoftness]  = (*modelShader)->GetLocation(uniform+"spotSoftness");
    locsLightModelShader[LocLightEnabled]       = (*modelShader)->GetLocation(uniform+"enabled");

    const int shadow = shadowMap != nullptr ? 1 : 0;
    (*modelShader)->SendValue(locsLightModelShader[LocLightShadow], &shadow, Shader::Uniform::Int);

    this->SetPosition(caster.position, false);
    this->SetTarget(caster.target, false);
    this->SetFovY(caster.fovy, false);

    this->UpdateMatrix();

    this->SetColor(color);
    this->SetRadius(radius);
    this->SetSpotlightSoftness(spotSoftness);
    this->SetSpotlight(spotlight);
    this->SetActive(enabled);
}

void gl::Light3D::SetShadowMapBounds(const shape2D::Rectangle& bounds)
{
    boundsMap = bounds;

    const float nBox[4] = {
        bounds.x / shadowMap->GetWidthF(),
        bounds.y / shadowMap->GetHeightF(),
        bounds.w / shadowMap->GetWidthF(),
        bounds.h / shadowMap->GetHeightF()
    };

    (*modelShader)->SendValue(
        locsLightModelShader[LocLightBounds],
        nBox, Shader::Uniform::Vec4);
}

void gl::Light3D::BeginShadowCast()
{
    shadowMap->Begin(boundsMap);
    caster.Begin(shadowMap->GetContext());
}

void gl::Light3D::EndShadowCast()
{
    caster.End();
    shadowMap->End();
}


/* Public Implementation Light3D */

void gl::Light3D::UpdateMatrix()
{
    (*modelShader)->SendValue(locsLightModelShader[LocLightMatrix], caster.GetViewMatrix() * caster.GetProjectionMatrix());
}

void gl::Light3D::SetPosition(const math::Vec3& position, bool updateMatrix)
{
    caster.position = position;
    math::Vec3 casterDirection = caster.GetDirection();
    (*modelShader)->SendValue(locsLightModelShader[LocLightPosition], reinterpret_cast<float*>(&caster.position), Shader::Uniform::Vec3);
    (*modelShader)->SendValue(locsLightModelShader[LocLightDirection], reinterpret_cast<float*>(&casterDirection), Shader::Uniform::Vec3);
    if (updateMatrix) this->UpdateMatrix();
}

void gl::Light3D::SetTarget(const math::Vec3& target, bool updateMatrix)
{
    caster.target = target;
    math::Vec3 casterDirection = caster.GetDirection();
    (*modelShader)->SendValue(locsLightModelShader[LocLightDirection], reinterpret_cast<float*>(&casterDirection), Shader::Uniform::Vec3);
    if (updateMatrix) this->UpdateMatrix();
}

void gl::Light3D::SetFovY(float fovy, bool updateMatrix)
{
    caster.fovy = fovy;
    float cutoff = std::cos(math::Deg2Rad * fovy * 0.46f);
    (*modelShader)->SendValue(locsLightModelShader[LocLightCutoff], &cutoff, Shader::Uniform::Float);
    if (updateMatrix) this->UpdateMatrix();
}

void gl::Light3D::SetRadius(float radius)
{
    this->radius = radius;
    (*modelShader)->SendValue(locsLightModelShader[LocLightRadius], &radius, Shader::Uniform::Float);
}

void gl::Light3D::SetColor(const gfx::Color& color)
{
    this->color = color;
    math::Vec4 nColor = color.Normalized();
    (*modelShader)->SendValue(locsLightModelShader[LocLightColor], reinterpret_cast<float*>(&nColor), Shader::Uniform::Vec3);
}

void gl::Light3D::SetCaster(const Camera3D& caster)
{
    this->caster = caster;
    SetPosition(caster.position, false);
    SetTarget(caster.target, false);
    SetFovY(caster.fovy, false);
    UpdateMatrix();
}

void gl::Light3D::SetSpotlightSoftness(float spotSoftness)
{
    this->spotSoftness = spotSoftness;
    (*modelShader)->SendValue(locsLightModelShader[LocLightSpotSoftness], &spotSoftness, Shader::Uniform::Float);
}

void gl::Light3D::SetSpotlight(bool enabled)
{
    this->spotlight = enabled;
    const int isEnabled = static_cast<int>(enabled);
    (*modelShader)->SendValue(locsLightModelShader[LocLightSpot], &isEnabled, Shader::Uniform::Int);
}

void gl::Light3D::SetActive(bool active)
{
    enabled = active;
    const int isEnabled = static_cast<int>(active);
    (*modelShader)->SendValue(locsLightModelShader[LocLightEnabled], &isEnabled, Shader::Uniform::Int);
}

/* Public Implementation Lights */

gl::Lights3D::Lights3D(Context& ctx, gfx::Color ambient, Uint16 maxLights, Uint16 mapSize)
: modelShader(ctx, core::TextFormat(vertLight, maxLights), core::TextFormat(fragLight, maxLights))
, bufferSize(maxLights * mapSize)
, mapSize(mapSize)
, maxLights(maxLights)
, shadowNum(0)
, ambient(ambient)
{
    this->Load(ctx);
}

gl::Lights3D::Lights3D(Context& ctx, float ambient, Uint16 maxLights, Uint16 mapSize)
: modelShader(ctx, core::TextFormat(vertLight, maxLights), core::TextFormat(fragLight, maxLights))
, bufferSize(maxLights * mapSize)
, mapSize(mapSize)
, maxLights(maxLights)
, shadowNum(0)
{
    this->ambient = {
        static_cast<Uint8>(255 * ambient),
        static_cast<Uint8>(255 * ambient),
        static_cast<Uint8>(255 * ambient),
        255
    };

    this->Load(ctx);
}

void gl::Lights3D::Load(Context& ctx)
{
    // Pre-allocation for lights
    sources.reserve(maxLights);

    // Creation of the shadow map
    if (bufferSize > 0)
    {
        shadowMap = std::make_unique<ShadowMap>(ctx, bufferSize, bufferSize);
        shadowShader = std::make_unique<Shader>(ctx, vertShadow, fragShadow);
    }

    // We recover the basic shader locations
    modelShader->locs[gl::LocVectorView]        = modelShader->GetLocation("viewPos");
    modelShader->locs[gl::LocMapHeight]         = modelShader->GetLocation("texture6");

    // We recover the additional shader locations
    locsLightModelShader[LocShadowMap]          = modelShader->GetLocation("shadowMap");
    locsLightModelShader[LocAmbientColor]       = modelShader->GetLocation("ambient");
    locsLightModelShader[LocShadowMapTexelSize] = modelShader->GetLocation("shadowMapTexelSize");
    locsLightModelShader[LocUseSpecularMap]     = modelShader->GetLocation("useSpecularMap");
    locsLightModelShader[LocUseNormalMap]       = modelShader->GetLocation("useNormalMap");
    locsLightModelShader[LocUseHeightMap]       = modelShader->GetLocation("useHeightMap");

    // Application of ambient lighting
    const math::Vec4 ambientCol = ambient.Normalized();
    modelShader->SendValue(locsLightModelShader[LocAmbientColor], reinterpret_cast<const float*>(&ambientCol), Shader::Uniform::Vec3);
}

gl::Light3D* gl::Lights3D::AddLight(const Camera3D& caster, const gfx::Color& color, float spotSoftness, float radius, bool spotlight, bool enabled)
{
    if (sources.size() < maxLights)
    {
        sources.push_back({ &modelShader, nullptr, static_cast<Uint16>(sources.size()), caster, color, spotSoftness, radius, spotlight, enabled });
        return &sources.back();
    }
    return nullptr;
}

gl::Light3D* gl::Lights3D::AddShadowLight(const Camera3D& caster, const gfx::Color& color, float spotSoftness, float radius, bool spotlight, bool enabled)
{
    if (shadowMap == nullptr)
    {
        NEXUS_LOG(Warning) << "Light with shadow requested without shadow map.\n";
        return AddLight(caster, color, spotSoftness, radius, spotlight, enabled);
    }

    if (sources.size() < maxLights)
    {
        sources.push_back({ &modelShader, shadowMap.get(), static_cast<Uint16>(sources.size()), caster, color, spotSoftness, radius, spotlight, enabled });

        float targetMapSize = static_cast<float>(bufferSize) / math::NextPowerOfTwo(++shadowNum);
        shape2D::RectangleF bounds = { 0, 0, targetMapSize, targetMapSize };

        for (auto& light : sources)
        {
            light.SetShadowMapBounds(bounds);
            bounds.x += targetMapSize;

            if (bounds.x >= bufferSize)
            {
                bounds.y += targetMapSize;
                bounds.x = 0;
            }
        }

        float shadowMapTexelSize = 1.0f / targetMapSize;
        modelShader->SendValue(locsLightModelShader[LocShadowMapTexelSize], &shadowMapTexelSize, Shader::Uniform::Float);

        return &sources.back();
    }
    return nullptr;
}

void gl::Lights3D::Update(const Camera3D& camera)
{
    modelShader->SendValue(
        modelShader->locs[gl::LocVectorView],
        reinterpret_cast<const float*>(&camera.position),
        Shader::Uniform::Vec3);

    if (shadowMap) shadowMap->Clear();
}

void gl::Lights3D::ShadowCastModel(Model& model, const math::Vec3& position, const math::Vec3& rotationAxis, float rotationAngle, const math::Vec3& scale)
{
    for (auto& light : sources)
    {
        if (!light.IsActive()) continue;

        light.BeginShadowCast();
        {
            // Calculate transformation matrix from function parameters
            // Get transform matrix (rotation -> scale -> translation)
            math::Mat4 matTransform = model->transform * math::Mat4::Transform(
                position, scale, rotationAxis, math::DegToRad(rotationAngle));

            for (int i = 0; i < model->meshes.size(); i++)
            {
                auto& material = model->materials[model->meshMaterial[i]];
                material->shader = *shadowShader;

                model->meshes[i]->Draw(material, matTransform);
            }
        }
        light.EndShadowCast();
    }
}

void gl::Lights3D::DrawModel(Model& model, const math::Vec3& position, const math::Vec3& rotationAxis, float rotationAngle, const math::Vec3& scale, const gfx::Color& tint)
{
    math::Mat4 matTransform = model->transform * math::Mat4::Transform(position, scale, rotationAxis, math::DegToRad(rotationAngle));
    if (shadowMap) modelShader->SendValue(locsLightModelShader[LocShadowMap], shadowMap->GetDepthTexture());

    for (int i = 0; i < model->meshes.size(); i++)
    {
        Material &material = model->materials[model->meshMaterial[i]];
        material->shader = modelShader;

        const Texture *texSpecular = material->GetTexture(Material::MapType::Specular);
        const Texture *texNormal = material->GetTexture(Material::MapType::Normal);
        const Texture *texHeight = material->GetTexture(Material::MapType::Height);

        const int useSpecularMap = texSpecular && (*texSpecular)->GetID() > 1 ? 1 : 0;
        const int useNormalMap = texNormal && (*texNormal)->GetID() > 1 ? 1 : 0;
        const int useHeightMap = texHeight && (*texHeight)->GetID() > 1 ? 1 : 0;

        modelShader->SendValue(locsLightModelShader[LocUseSpecularMap], &useSpecularMap, Shader::Uniform::Int);
        modelShader->SendValue(locsLightModelShader[LocUseNormalMap], &useNormalMap, Shader::Uniform::Int);
        modelShader->SendValue(locsLightModelShader[LocUseHeightMap], &useHeightMap, Shader::Uniform::Int);

        const gfx::Color color = material->maps[Material::MapType::Diffuse].color;

        material->maps[Material::MapType::Diffuse].color = color * tint;
        model->meshes[i]->Draw(material, matTransform);
        material->maps[Material::MapType::Diffuse].color = color;
    }
}

void gl::Lights3D::DrawShadowMap(const shape2D::Rectangle& dst, float near, float far)
{
    if (shadowMap) shadowMap->Draw(dst, near, far);
}
